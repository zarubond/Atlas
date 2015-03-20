/**
 *  Atlas - Volumetric terrain editor
 *  Copyright (C) 2012-2015  Ondřej Záruba
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software Foundation,
 *  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 */

#include "terrainmaker.h"
#include "terraingraphics.h"

namespace Transvoxel {
    #include "transvoxel.cpp"
}

TerrainMaker::TerrainMaker(int region_size)
    :region_size(region_size)
{
    int edges_size=region_size+2;
    edges = new int[3*edges_size*edges_size*edges_size];

    byte_count=19;
    data_capacity=byte_count*1000;
    indices_capacity=18000;
    data_buffer    = new unsigned char[data_capacity];
    indices_buffer = new unsigned short[indices_capacity];

    buffer=new TerrainVoxel[34*34*34];
}

TerrainMaker::~TerrainMaker()
{
    delete [] data_buffer;
    delete [] indices_buffer;
    delete [] edges;
    delete [] buffer;
}

TerrainMaker::SurfaceData TerrainMaker::rebuid(TerrainSurface * surface)
{
    SurfaceData temp;
    if(surface->comressed_size>0 || surface->voxel_data!=NULL)
    {
        buffer_data_count=0;
        buffer_indices_count=0;

        int edges_size=region_size+2;
        //reset edge field
        std::fill_n(edges,edges_size*edges_size*edges_size*3,-1 );

        int size=rebuidSurface(surface,&temp);
        surface->dirty=false;
        if(size==0)
        {
            if(surface->voxel_data[0].value<0)
                surface->setRegionType(TerrainRegion::UNDERGROUND);
            else
                surface->setRegionType(TerrainRegion::ABOVEGROUND);
            surface->clearGraphics();
        }
        else
        {
            surface->clearGraphics();
            surface->setRegionType(TerrainRegion::GROUND);
            surface->vbo.create(buffer_data_count*sizeof(unsigned char), data_buffer, GL_DYNAMIC_DRAW);
            surface->ibo.create(buffer_indices_count*sizeof(unsigned short), indices_buffer, GL_DYNAMIC_DRAW);
            temp.copy(surface);
            surface->drawable=true;

            static int total=0;
            total+=buffer_data_count+buffer_indices_count*2;
            //cout<<total<<endl;
        }
    }
    return temp;
}

int TerrainMaker::rebuidSurface(TerrainSurface *surface, TerrainMaker::SurfaceData *temp)
{
    int edges_size=region_size+2;
    std::fill_n(edges,edges_size*edges_size*edges_size*3,-1);

    bufferVoxels(surface);

    int cnt=0;
    cnt+=marchingCubes(surface,temp,TerrainSurface::LEVEL_0);
    cnt+=marchingCubes(surface,temp,TerrainSurface::LEVEL_1);
    cnt+=marchingCubes(surface,temp,TerrainSurface::LEVEL_2);
    cnt+=marchingCubes(surface,temp,TerrainSurface::LEVEL_3);

    cnt+=transvoxel(surface,temp,TerrainSurface::LEVEL_1,TerrainSurface::X_SIDE);
    cnt+=transvoxel(surface,temp,TerrainSurface::LEVEL_1,TerrainSurface::Y_SIDE);
    cnt+=transvoxel(surface,temp,TerrainSurface::LEVEL_1,TerrainSurface::Z_SIDE);

    cnt+=transvoxel(surface,temp,TerrainSurface::LEVEL_2,TerrainSurface::X_SIDE);
    cnt+=transvoxel(surface,temp,TerrainSurface::LEVEL_2,TerrainSurface::Y_SIDE);
    cnt+=transvoxel(surface,temp,TerrainSurface::LEVEL_2,TerrainSurface::Z_SIDE);

    cnt+=transvoxel(surface,temp,TerrainSurface::LEVEL_3,TerrainSurface::X_SIDE);
    cnt+=transvoxel(surface,temp,TerrainSurface::LEVEL_3,TerrainSurface::Y_SIDE);
    cnt+=transvoxel(surface,temp,TerrainSurface::LEVEL_3,TerrainSurface::Z_SIDE);

    return cnt;
}

int TerrainMaker::marchingCubes(TerrainSurface * surface,SurfaceData *temp, TerrainSurface::LOD level)
{
    int step=1;

    switch(level)
    {
    case TerrainSurface::LEVEL_0:   step=1;     break;
    case TerrainSurface::LEVEL_1:   step=2;     break;
    case TerrainSurface::LEVEL_2:   step=4;     break;
    case TerrainSurface::LEVEL_3:   step=8;     break;
    default:break;
    }

    int data_count=buffer_data_count;
    int indices_count=buffer_indices_count;
    int size=region_size;
    int edges_size=size+2;

    unsigned int cubeindex=0;
    Vector3f tmp_vertex;

    int edge_list[12];

    static const int voxel[][8][3]={{{0,0,0},{1,0,0},{0,0,1},{1,0,1},{0,1,0},{1,1,0},{0,1,1},{1,1,1}},
                          {{0,0,0},{2,0,0},{0,0,2},{2,0,2},{0,2,0},{2,2,0},{0,2,2},{2,2,2}},
                          {{0,0,0},{4,0,0},{0,0,4},{4,0,4},{0,4,0},{4,4,0},{0,4,4},{4,4,4}},
                          {{0,0,0},{8,0,0},{0,0,8},{8,0,8},{0,8,0},{8,8,0},{0,8,8},{8,8,8}}};

    //position of all corners from 0,0,0

    Vector3i vert0,vert1;
    unsigned short edge_id, corner0, corner1, twist;
    const TerrainVoxel * v0,*v1;
    const Transvoxel::RegularCellData *data;

    //test whole voxel field
    for(int i=0;i<size;i+=step)
        for(int j=0;j<size;j+=step)
            for(int k=0;k<size;k+=step)
            {
                //test voxel position
                cubeindex = ((bufferVoxel(i+voxel[level][0][0],j+voxel[level][0][1],k+voxel[level][0][2])->value >> 7) & 0x01)
                          | ((bufferVoxel(i+voxel[level][1][0],j+voxel[level][1][1],k+voxel[level][1][2])->value >> 6) & 0x02)
                          | ((bufferVoxel(i+voxel[level][2][0],j+voxel[level][2][1],k+voxel[level][2][2])->value >> 5) & 0x04)
                          | ((bufferVoxel(i+voxel[level][3][0],j+voxel[level][3][1],k+voxel[level][3][2])->value >> 4) & 0x08)
                          | ((bufferVoxel(i+voxel[level][4][0],j+voxel[level][4][1],k+voxel[level][4][2])->value >> 3) & 0x10)
                          | ((bufferVoxel(i+voxel[level][5][0],j+voxel[level][5][1],k+voxel[level][5][2])->value >> 2) & 0x20)
                          | ((bufferVoxel(i+voxel[level][6][0],j+voxel[level][6][1],k+voxel[level][6][2])->value >> 1) & 0x40)
                          |  (bufferVoxel(i+voxel[level][7][0],j+voxel[level][7][1],k+voxel[level][7][2])->value & 0x80);

                if (cubeindex==0 || cubeindex==255)
                    continue;

                data = &Transvoxel::regularCellData[Transvoxel::regularCellClass[cubeindex]];

                //get all voxel edges
                for(int id=0;id < data->GetVertexCount();id++)
                {
                    edge_id=Transvoxel::regularVertexData[cubeindex][id];
                    corner1 = edge_id & 0x0F;
                    corner0 = (edge_id & 0xF0) >> 4;
                    twist=((edge_id>>8) & 0x0F)-1;//edge position in array

                    vert0.set(i+voxel[level][corner0][0],j+voxel[level][corner0][1],k+voxel[level][corner0][2]);
                    vert1.set(i+voxel[level][corner1][0],j+voxel[level][corner1][1],k+voxel[level][corner1][2]);

                    if(step!=1)
                        adjustVertices(vert0, vert1);

                    int * edge=&edges[twist*edges_size*edges_size*edges_size+(vert0[0]*edges_size*edges_size+vert0[1]*edges_size+vert0[2])];
                    if((*edge)!=-1)
                        edge_list[id]=(*edge);
                    else
                    {
                        (*edge)=data_count/byte_count;
                        edge_list[id]=(*edge);

                        v0=bufferVoxel(vert0[0],vert0[1],vert0[2]);
                        v1=bufferVoxel(vert1[0],vert1[1],vert1[2]);

                        tmp_vertex=surface->position();
                        tmp_vertex+=interpolateVertex(vert0[0],vert0[1],vert0[2],v0->value,vert1[0],vert1[1],vert1[2],v1->value);

                        memcpy((void*)&data_buffer[data_count],(const void*)tmp_vertex.array(),sizeof(float)*3);
                        data_count+=3*sizeof(float);

                        memset((void*)&data_buffer[data_count], 0, sizeof(char)*3);//make space for normal
                        data_count+=3*sizeof(char);
/*
                        tmp_normal=surface->getNormal(vert0,vert1,step);
                        tmp_normal.normalize();
                        tmp_normal*=127.0;

                        Vector3c t=tmp_normal;
                        memcpy((void*)&data_buffer[data_count],(const void*)t.array(),sizeof(GLbyte)*3);
                        data_count+=3*sizeof(GLbyte);*/

                        if(v0->value>0)
                            memcpy((void*)&data_buffer[data_count],(const void*)v1->material,sizeof(unsigned char)*4);
                        else
                            memcpy((void*)&data_buffer[data_count],(const void*)v0->material,sizeof(unsigned char)*4);
                        data_count+=4*sizeof(unsigned char);

                        resizeDataBuffer(data_count);
                    }
                }

                //form triangles from edges
                createTriangles(data,edge_list,indices_count,step==1);
            }


    if(indices_count > buffer_indices_count)
    {
        buffer_data_count=data_count;

        temp->index_cnt[level]=indices_count-buffer_indices_count;
        temp->index_offset[level]=buffer_indices_count;
        buffer_indices_count=indices_count;

    }
    return data_count;
}

int TerrainMaker::transvoxel(TerrainSurface * surface,SurfaceData * temp, TerrainSurface::LOD level, TerrainSurface::Side side)
{
    int step=1;
    switch(level)
    {
    case TerrainSurface::LEVEL_0:   step=1;     break;
    case TerrainSurface::LEVEL_1:   step=2;     break;
    case TerrainSurface::LEVEL_2:   step=4;     break;
    case TerrainSurface::LEVEL_3:   step=8;     break;
    default:break;
    }

    int size=region_size;
    int edges_size=size+2;

    int data_count=buffer_data_count;
    int indices_count=buffer_indices_count;

    unsigned int cubeindex=0;
    Vector3f tmp_normal,tmp_vertex;

    int edge_list[12];
    Vector3i vert0,vert1;
    const TerrainVoxel * v;

    //position of all corners from 0,0,0

    static const int voxel[][3][9][3] = {{
        {{0, 0, 0},{0, 1/2, 0},{0, 1, 0},{0, 1, 1/2},{0, 1, 1},{0, 1/2, 1},{0, 0, 1},{0, 0, 1/2},{0, 1/2, 1/2}},
        {{0, 0, 0},{1/2, 0, 0},{1, 0, 0},{1, 0, 1/2},{1, 0, 1},{1/2, 0, 1},{0, 0, 1},{0, 0, 1/2},{1/2, 0, 1/2}},
        {{0, 0, 0},{1/2, 0, 0},{1, 0, 0},{1, 1/2, 0},{1, 1, 0},{1/2, 1, 0},{0, 1, 0},{0, 1/2, 0},{1/2, 1/2, 0}}},
        {
        {{0, 0, 0},{0, 2/2, 0},{0, 2, 0},{0, 2, 2/2},{0, 2, 2},{0, 2/2, 2},{0, 0, 2},{0, 0, 2/2},{0, 2/2, 2/2}},
        {{0, 0, 0},{2/2, 0, 0},{2, 0, 0},{2, 0, 2/2},{2, 0, 2},{2/2, 0, 2},{0, 0, 2},{0, 0, 2/2},{2/2, 0, 2/2}},
        {{0, 0, 0},{2/2, 0, 0},{2, 0, 0},{2, 2/2, 0},{2, 2, 0},{2/2, 2, 0},{0, 2, 0},{0, 2/2, 0},{2/2, 2/2, 0}}},
        {
        {{0, 0, 0},{0, 4/2, 0},{0, 4, 0},{0, 4, 4/2},{0, 4, 4},{0, 4/2, 4},{0, 0, 4},{0, 0, 4/2},{0, 4/2, 4/2}},
        {{0, 0, 0},{4/2, 0, 0},{4, 0, 0},{4, 0, 4/2},{4, 0, 4},{4/2, 0, 4},{0, 0, 4},{0, 0, 4/2},{4/2, 0, 4/2}},
        {{0, 0, 0},{4/2, 0, 0},{4, 0, 0},{4, 4/2, 0},{4, 4, 0},{4/2, 4, 0},{0, 4, 0},{0, 4/2, 0},{4/2, 4/2, 0}}},
        {
        {{0, 0, 0},{0, 8/2, 0},{0, 8, 0},{0, 8, 8/2},{0, 8, 8},{0, 8/2, 8},{0, 0, 8},{0, 0, 8/2},{0, 8/2, 8/2}},
        {{0, 0, 0},{8/2, 0, 0},{8, 0, 0},{8, 0, 8/2},{8, 0, 8},{8/2, 0, 8},{0, 0, 8},{0, 0, 8/2},{8/2, 0, 8/2}},
        {{0, 0, 0},{8/2, 0, 0},{8, 0, 0},{8, 8/2, 0},{8, 8, 0},{8/2, 8, 0},{0, 8, 0},{0, 8/2, 0},{8/2, 8/2, 0}}
        }};

    static const int transition_corner[]={0x0,0x1,0x2,0x7,0x8,0x3,0x6,0x5,0x4,0x0,0x2,0x6,0x4};

    int min_x=0,min_y=0,min_z=0, max_x=size,max_y=size,max_z=size;
    switch(side)
    {
    case TerrainSurface::X_SIDE: min_x=size;max_x=size+1; break;
    case TerrainSurface::Y_SIDE: min_y=size;max_y=size+1; break;
    case TerrainSurface::Z_SIDE: min_z=size;max_z=size+1; break;
    default:break;
    }

    unsigned short edge_id, corner0, corner1, twist;

    //test whole voxel field
    for(int i=min_x; i<max_x; i+=step)
        for(int j=min_y; j<max_y; j+=step)
            for(int k=min_z; k<max_z; k+=step)
            {
                cubeindex = ((bufferVoxel(i+voxel[level][side][0][0],j+voxel[level][side][0][1],k+voxel[level][side][0][2])->value >> 8) & 0x01)
                          | ((bufferVoxel(i+voxel[level][side][1][0],j+voxel[level][side][1][1],k+voxel[level][side][1][2])->value >> 7) & 0x02)
                          | ((bufferVoxel(i+voxel[level][side][2][0],j+voxel[level][side][2][1],k+voxel[level][side][2][2])->value >> 6) & 0x04)
                          | ((bufferVoxel(i+voxel[level][side][3][0],j+voxel[level][side][3][1],k+voxel[level][side][3][2])->value >> 5) & 0x08)
                          | ((bufferVoxel(i+voxel[level][side][4][0],j+voxel[level][side][4][1],k+voxel[level][side][4][2])->value >> 4) & 0x10)
                          | ((bufferVoxel(i+voxel[level][side][5][0],j+voxel[level][side][5][1],k+voxel[level][side][5][2])->value >> 3) & 0x20)
                          | ((bufferVoxel(i+voxel[level][side][6][0],j+voxel[level][side][6][1],k+voxel[level][side][6][2])->value >> 2) & 0x40)
                          | ((bufferVoxel(i+voxel[level][side][7][0],j+voxel[level][side][7][1],k+voxel[level][side][7][2])->value >> 1) & 0x80)
                          |  (bufferVoxel(i+voxel[level][side][8][0],j+voxel[level][side][8][1],k+voxel[level][side][8][2])->value & 0x100);

                if (cubeindex==0 || cubeindex==511)
                   continue;

                unsigned char class_index = Transvoxel::transitionCellClass[cubeindex];
                const Transvoxel::TransitionCellData *data = &Transvoxel::transitionCellData[(class_index & 0x7F)];

                v=bufferVoxel(i,j,k);

                for(int id=0;id < data->GetVertexCount();id++)
                {
                    edge_id=Transvoxel::transitionVertexData[cubeindex][id];

                    corner1 = edge_id & 0x0F;
                    corner0 = (edge_id & 0xF0) >> 4;
                    twist=(edge_id>>8) & 0x0F;//edge position in array

                    //dont know why is it switched
                    corner0=transition_corner[corner0];
                    corner1=transition_corner[corner1];

                    vert0.set(i+voxel[level][side][corner0][0],j+voxel[level][side][corner0][1],k+voxel[level][side][corner0][2]);
                    vert1.set(i+voxel[level][side][corner1][0],j+voxel[level][side][corner1][1],k+voxel[level][side][corner1][2]);

                    if(voxel[level][side][corner0][0] - voxel[level][side][corner1][0] !=0)
                        twist=1;
                    else if(voxel[level][side][corner0][1] - voxel[level][side][corner1][1] !=0)
                        twist=2;
                    else if(voxel[level][side][corner0][2] - voxel[level][side][corner1][2] !=0)
                        twist=0;

                    adjustVertices(vert0,vert1);

                    //edge rotation problem horizontal vs vertical
                    int * edge=&edges[twist*edges_size*edges_size*edges_size + vert0[0]*edges_size*edges_size+vert0[1]*edges_size+vert0[2]];
                    if((*edge)!=-1)
                        edge_list[id]=(*edge);
                    else
                    {
                        tmp_vertex=surface->position();
                        tmp_vertex+=interpolateVertex(vert0[0],vert0[1],vert0[2],bufferVoxel(vert0[0],vert0[1],vert0[2])->value,
                                                      vert1[0],vert1[1],vert1[2],bufferVoxel(vert1[0],vert1[1],vert1[2])->value);

                        edge_list[id]=data_count/byte_count;

                        (*edge)=edge_list[id];

                        memcpy((void*)&data_buffer[data_count],(const void*)tmp_vertex.array(),sizeof(float)*3);
                        data_count+=3*sizeof(float);

                        tmp_normal=surface->getNormal(vert0,vert1,step);
                        tmp_normal.normalize();
                        tmp_normal*=127.0;

                        Vector3c t=tmp_normal;
                        memcpy((void*)&data_buffer[data_count],(const void*)t.array(),sizeof(unsigned char)*3);
                        data_count+=3*sizeof(unsigned char);


                        memcpy((void*)&data_buffer[data_count],(const void*)v->material,sizeof(unsigned char)*4);
                        data_count+=4*sizeof(unsigned char);

                        resizeDataBuffer(data_count);
                    }
                }

                createTriangles(data,edge_list,indices_count);
            }

    if(indices_count > buffer_indices_count)
    {
        buffer_data_count=data_count;

        temp->index_side_cnt[side][level]=indices_count-buffer_indices_count;
        temp->index_side_offset[side][level]=buffer_indices_count;
        buffer_indices_count=indices_count;
    }

    return data_count;
}

void TerrainMaker::createTriangles(const Transvoxel::TransitionCellData *data, const int *edge_list, int & indices_count)
{
    Vector3f vert[3];
    Vector3f normal;
    for(int id=0;id < data->GetTriangleCount();id++)
    {
        for(int b=0;b<4;b++)
        {
            for(int t=0;t<3;t++)
            {
                if(data_buffer[edge_list[data->vertexIndex[id*3+t]]*byte_count+15+b]!=0)
                {
                    memcpy((void*)vert[0].array(),(const void*)&data_buffer[edge_list[data->vertexIndex[id*3+0]]*byte_count],sizeof(GLfloat)*3);
                    memcpy((void*)vert[1].array(),(const void*)&data_buffer[edge_list[data->vertexIndex[id*3+1]]*byte_count],sizeof(GLfloat)*3);
                    memcpy((void*)vert[2].array(),(const void*)&data_buffer[edge_list[data->vertexIndex[id*3+2]]*byte_count],sizeof(GLfloat)*3);

                    normal=Triangle::normal(vert[0],vert[1],vert[2]);
                    if(normal[0]*normal[0] + normal[1]*normal[1] + normal[2]*normal[2]==0.0) //if there is no normal, than this is not triangle
                        goto end_test;

                    indices_buffer[indices_count++]=edge_list[data->vertexIndex[id*3+0]];
                    indices_buffer[indices_count++]=edge_list[data->vertexIndex[id*3+1]];
                    indices_buffer[indices_count++]=edge_list[data->vertexIndex[id*3+2]];
/*
                    normal.normalize();
                    normal*=127.0;

                    Vector3f tmp;
                    Vector3c n;
                    for(int c=0;c<3;c++)
                    {
                        memcpy((void*)n.array(),(const void*)&data_buffer[edge_list[data->vertexIndex[id*3+c]]*byte_count+sizeof(GLfloat)*3],sizeof(GLbyte)*3);
                        tmp=n;
                        if(tmp[0]*tmp[0]+tmp[1]*tmp[1]+tmp[2]*tmp[2]==0.0)
                        {
                            tmp=normal;
                            tmp+=n;
                            tmp.normalize();
                            tmp*=127.0;
                            n=tmp;
                            memcpy((void*)&data_buffer[edge_list[data->vertexIndex[id*3+c]]*byte_count+sizeof(GLfloat)*3],(const void*)n.array(),sizeof(GLbyte)*3);
                        }
                    }
*/

                    resizeIndexBuffer(indices_count);
                    goto end_test;
                }
            }
        }
        end_test:;
    }
}

void TerrainMaker::createTriangles(const Transvoxel::RegularCellData *data, const int *edge_list, int &indices_count, bool make_normal)
{
    Vector3f vert[3];
    Vector3c n;
    Vector3f tmp, normal;

    for(int id=0; id < data->GetTriangleCount(); id++)
    {
        for(int b=0;b<4;b++)
        {
            for(int t=0;t<3;t++)
            {
                if(data_buffer[edge_list[data->vertexIndex[id*3+t]]*byte_count+15+b]!=0)
                {
                    memcpy((void*)vert[0].array(),(const void*)&data_buffer[edge_list[data->vertexIndex[id*3+0]]*byte_count],sizeof(float)*3);
                    memcpy((void*)vert[1].array(),(const void*)&data_buffer[edge_list[data->vertexIndex[id*3+1]]*byte_count],sizeof(float)*3);
                    memcpy((void*)vert[2].array(),(const void*)&data_buffer[edge_list[data->vertexIndex[id*3+2]]*byte_count],sizeof(float)*3);

                    normal=Triangle::normal(vert[0],vert[1],vert[2]);
                    if(normal[0]*normal[0] + normal[1]*normal[1] + normal[2]*normal[2]==0.0) //if there is no normal, than this is not triangle
                        goto end_test;

                    indices_buffer[indices_count++]=edge_list[data->vertexIndex[id*3]];
                    indices_buffer[indices_count++]=edge_list[data->vertexIndex[id*3+1]];
                    indices_buffer[indices_count++]=edge_list[data->vertexIndex[id*3+2]];

                    normal.normalize();
/*
                    Vector3f weight = abs(normal);
                    weight = (weight-0.5)*7.0;//*7;//(weight - 0.267900);
                    weight = normalize(max( weight, vec3( 0.0)));
                    weight /= vec3(weight.x  + weight.y + weight.z);*/

                    Vector3f weight = normal;
                    weight.abs();
                    weight = (weight-Vector3f(0.5))*Vector3f(7.0);//*7;//(weight - 0.267900);
                    //weight = normalize(max( weight, vec3( 0.0)));
                    weight.normalize();
                    weight /= Vector3f(weight[0]  + weight[1] + weight[2]);



                    //normal.normalize();
                    normal*=127.0;

                    if(make_normal)
                    {
                        for(int c=0;c<3;c++)
                        {
                            memcpy((void*)n.array(),(const void*)&data_buffer[edge_list[data->vertexIndex[id*3+c]]*byte_count+sizeof(float)*3],sizeof(signed char)*3);
                            tmp=normal;
                            tmp+=n;
                            tmp.normalize();
                            tmp*=127.0;
                            n=tmp;
                            memcpy((void*)&data_buffer[edge_list[data->vertexIndex[id*3+c]]*byte_count+sizeof(float)*3],(const void*)n.array(),sizeof(signed char)*3);
                        }
                    }
                    else
                    {

                        //this is a hack, when the triangle is line, so at lower level it id not possible get normal
                        for(int c=0;c<3;c++)
                        {
                            memcpy((void*)n.array(),(const void*)&data_buffer[edge_list[data->vertexIndex[id*3+c]]*byte_count+sizeof(float)*3],sizeof(signed char)*3);
                            tmp=n;
                            if(tmp[0]*tmp[0]+tmp[1]*tmp[1]+tmp[2]*tmp[2]==0.0)
                            {
                                tmp=normal;
                                tmp+=n;
                                tmp.normalize();
                                tmp*=127.0;
                                n=tmp;
                                memcpy((void*)&data_buffer[edge_list[data->vertexIndex[id*3+c]]*byte_count+sizeof(float)*3],(const void*)n.array(),sizeof(signed char)*3);
                            }
                        }
                    }

                    resizeIndexBuffer(indices_count);
                    goto end_test;
                }
            }
        }
        end_test:;
    }
}

void TerrainMaker::resizeIndexBuffer(int size)
{
    if(size==indices_capacity)
    {
        int n_capa=indices_capacity*2;
        unsigned short * tmp_indices  = new unsigned short[n_capa];

        memcpy(tmp_indices,indices_buffer,size*sizeof(*indices_buffer));
        delete [] indices_buffer;

        indices_buffer=tmp_indices;
        indices_capacity=n_capa;
    }
}

void TerrainMaker::resizeDataBuffer(int size)
{
    if(size==data_capacity)
    {
        int n_capa=data_capacity*2;
        unsigned char * tmp_buffer=new unsigned char[n_capa];

        memcpy(tmp_buffer,data_buffer,size*sizeof(*data_buffer));

        delete [] data_buffer;

        data_buffer=tmp_buffer;
        data_capacity=n_capa;
    }
}

TerrainMaker::SurfaceData::SurfaceData()
{
    memset(index_cnt,           0, sizeof(index_cnt));
    memset(index_offset,        0, sizeof(index_offset));
    memset(index_side_cnt,      0, sizeof(index_side_cnt));
    memset(index_side_offset,   0, sizeof(index_side_offset));
}

void TerrainMaker::SurfaceData::copy(TerrainSurface *surface)
{
    memcpy(surface->index_cnt,          index_cnt,          sizeof(index_cnt));
    memcpy(surface->index_offset,       index_offset,       sizeof(index_offset));
    memcpy(surface->index_side_cnt,     index_side_cnt,     sizeof(index_side_cnt));
    memcpy(surface->index_side_offset,  index_side_offset,  sizeof(index_side_offset));
}

Vector3f TerrainMaker::interpolateVertex(float x1, float y1, float z1, float v1, float x2, float y2, float z2, float v2) const
{
    float mu =0.0;
    if(v1 - v2!=0.0)  mu= v1 / (v1 - v2);

    return Vector3f(x1 + mu * (x2 - x1), y1 + mu * (y2 - y1), z1 + mu * (z2 - z1));
}

void TerrainMaker::adjustVertices(Vector3i & v1,Vector3i & v2) const
{
    float val1=bufferVoxel(v1[0],v1[1],v1[2])->value;
    float val2=bufferVoxel(v2[0],v2[1],v2[2])->value;
    static const int voxel[][3]={{1,0,0},{0,1,0},{0,0,1}};

    int d=0;
    for(int a=0;a<3;a++)
    {
         d=v1[a]-v2[a];
         d=abs(d);

         //find actual vertices
         if(d>1)
         {
             if(v2[a]<v1[a])
             {
                 for(int i=0;i<d-1;i++)
                 {
                     float tmp_val=bufferVoxel(v2[0]+voxel[a][0],v2[1]+voxel[a][1],v2[2]+voxel[a][2])->value;
                     if((tmp_val<0 && val2<0) || (tmp_val>0 && val2>0))
                     {
                         v2[a]++;
                         val2=tmp_val;
                     }
                     else
                         break;
                 }
                 v1[a]=v2[a]+1;
                 val1=bufferVoxel(v1[0],v1[1],v1[2])->value;

             }
             else//y1<y2
             {
                 for(int i=0;i<d-1;i++)
                 {
                     float tmp_val=bufferVoxel(v1[0]+voxel[a][0],v1[1]+voxel[a][1],v1[2]+voxel[a][2])->value;
                     if((tmp_val<0 && val1<0) || (tmp_val>0 && val1>0))
                     {
                         v1[a]++;
                         val1=tmp_val;
                     }
                     else
                         break;
                 }
                 v2[a]=v1[a]+1;
                 val2=bufferVoxel(v2[0],v2[1],v2[2])->value;
             }
         }
    }
}


void TerrainMaker::bufferVoxels(TerrainSurface * surface)
{
    surface->touchDecompress();
/*
    for(int x=-1;x<region_size+1;x++)
        for(int y=-1;y<region_size+1;y++)
            for(int z=-1;z<region_size+1;z++)
            {
                buffer[(x+1)*(34)*(34)+(y+1)*(34)+z+1]=*surface->field(x,y,z);
            }
*/
    for(int x=0;x<region_size;x++)
        for(int y=0;y<region_size;y++)
            for(int z=0;z<region_size;z++)
            {
                buffer[(x+1)*(34)*(34)+(y+1)*(34)+z+1]=*surface->voxelRaw(x,y,z);
            }

    for(int x=-1;x<region_size+1;x+=region_size+1)
        for(int y=-1;y<region_size+1;y++)
            for(int z=-1;z<region_size+1;z++)
            {
                buffer[(x+1)*(34)*(34)+(y+1)*(34)+z+1]=*surface->voxel(x,y,z);
            }

    for(int x=0;x<region_size;x++)
        for(int y=-1;y<region_size+1;y+=region_size+1)
            for(int z=-1;z<region_size+1;z++)
            {
                buffer[(x+1)*(34)*(34)+(y+1)*(34)+z+1]=*surface->voxel(x,y,z);
            }
    for(int x=0;x<region_size;x++)
        for(int y=0;y<region_size;y++)
            for(int z=-1;z<region_size+1;z+=region_size+1)
            {
                buffer[(x+1)*(34)*(34)+(y+1)*(34)+z+1]=*surface->voxel(x,y,z);
            }
}
