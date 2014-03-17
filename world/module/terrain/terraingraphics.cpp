/**
 *  Atlas - Volumetric terrain editor
 *  Copyright (C) 2012-2013  Ondřej Záruba
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
#include "terraingraphics.h"

#include "terrainsurface.h"
#include "../../map.h"

#include "transvoxel.cpp"

TerrainGraphics::TerrainGraphics(int type): TerrainBase(type)
{
    for(int i=0;i<maxTextures();i++)
        textures[i]=NULL;
    render_count=0;
    rebuild_count=0;

    render_list=NULL;
    rebuild_list=NULL;

    data_buffer=NULL;
    indices_buffer=NULL;

    edges=NULL;
}

TerrainGraphics::~TerrainGraphics()
{
    for(int i=0;i<maxTextures();i++)
        delete textures[i];

    delete [] render_list;
    delete [] rebuild_list;

    delete [] edges;

    delete [] data_buffer;
    delete [] indices_buffer;
}

void TerrainGraphics::build(int nodes_x, int nodes_y, int nodes_z, int node_size)
{
    this->nodes_x=nodes_x;
    this->nodes_y=nodes_y;
    this->nodes_z=nodes_z;
    this->node_size=node_size;

    grid=new TerrainRegion *[nodes_x*nodes_y*nodes_z];

    for(int i=0;i<nodes_x*nodes_y*nodes_z;i++)
        grid[i]=NULL;

    capacity=7000;
    indices_capacity=18000;
    render_count=0;
    int edges_size=regionSize()+2;
    render_list    = new TerrainSurface*[nodes_x*nodes_y*nodes_z];
    rebuild_list   = new TerrainSurface*[nodes_x*nodes_y*nodes_z];
    data_buffer    = new GLfloat[capacity];
    indices_buffer = new GLushort[indices_capacity];
    edges          = new int[3*edges_size*edges_size*edges_size];
}

void TerrainGraphics::rebuid(bool force)
{
    TerrainSurface * tmp;
    int size;
    for(int i=0;i<nodes_x;i++)
        for(int j=0;j<nodes_y;j++)
            for(int k=0;k<nodes_z;k++)
            {
                tmp=(TerrainSurface*)grid[i*nodes_z*nodes_y +j*nodes_z+k];

                if(tmp!=NULL && (tmp->dirty || force))
                {
                        size=rebuildLevel(tmp,TerrainSurface::LEVEL_0);
                        size+=rebuildLevel(tmp,TerrainSurface::LEVEL_1);
                        size+=rebuildLevel(tmp,TerrainSurface::LEVEL_2);
                        size+=rebuildLevel(tmp,TerrainSurface::LEVEL_3);
                        tmp->dirty=false;
                        if(size==0)
                        {
                            if(map->removeObject(tmp))
                            {
                                delete tmp;
                                grid[i*nodes_z*nodes_y +j*nodes_z+k]=NULL;
                            }
                        }
                }
            }
}

void TerrainGraphics::pushRender(TerrainSurface *surface)
{
    render_list[render_count++]=surface;
}

void TerrainGraphics::pushRebuild(TerrainSurface *surface)
{
    rebuild_list[rebuild_count++]=surface;
}

bool TerrainGraphics::setTexture(unsigned short id, Texture *texture)
{
    if(id<maxTextures())
    {
        if(textures[id]!=NULL)
            delete textures[id];
        textures[id]=texture;
        return true;
    }
    return false;
}

int TerrainGraphics::maxTextures() const
{
    return 8;
}

Texture *TerrainGraphics::getTexture(unsigned short id)
{
    if(id<maxTextures())
        return textures[id];
    return NULL;
}


TerrainSurface *TerrainGraphics::popRender()
{
    return render_list[--render_count];
}

void TerrainGraphics::clearRender()
{
    render_count=0;
}

int TerrainGraphics::renderCount()
{
    return render_count;
}

int TerrainGraphics::rebuildLevel(TerrainSurface * surface,TerrainSurface::LOD level)
{
    surface->clearLevel(level);
    int cnt=marchingCubes(surface,level);

    if(level>=TerrainSurface::LEVEL_1)
    {
        cnt+=transvoxel(surface,level,TerrainSurface::X_SIDE);
        cnt+=transvoxel(surface,level,TerrainSurface::Y_SIDE);
        cnt+=transvoxel(surface,level,TerrainSurface::Z_SIDE);
    }
    return cnt;
}

int TerrainGraphics::marchingCubes(TerrainSurface * surface, TerrainSurface::LOD level)
{
    int step=1;
    switch(level)
    {
    case TerrainSurface::LEVEL_0:   step=1;     break;
    case TerrainSurface::LEVEL_1:   step=2;     break;
    case TerrainSurface::LEVEL_2:   step=4;     break;
    case TerrainSurface::LEVEL_3:   step=8;     break;
    }

    int data_count=0;
    int indices_count=0;
    int size=regionSize();
    int edges_size=size+2;

    int cubeindex;
    Vertex3f tmp_normal,tmp_vertex;

    int edge_list[12];

    //position of all corners from 0,0,0
    const int voxel[][3]={{0,0,0},{step,0,0},{0,0,step},{step,0,step},{0,step,0},{step,step,0},{0,step,step},{step,step,step}};
    Vertex3i vert0,vert1;

    //reset edge field
    for(int i=0;i<edges_size*edges_size*edges_size*3;i++)
        edges[i]=-1;

    //test whole voxel field
    for(int i=0;i<size;i+=step)
        for(int j=0;j<size;j+=step)
            for(int k=0;k<size;k+=step)
            {
                cubeindex = 0;

                int exp=1;
                //test voxel position
                for(int c=0;c<8;c++)
                {
                    if (surface->field(i+voxel[c][0],j+voxel[c][1],k+voxel[c][2])->value < isolevel)
                        cubeindex |= exp;
                    exp*=2;
                }

                if (cubeindex==0 || cubeindex==255)
                   continue;

                const RegularCellData *data = &regularCellData[regularCellClass[cubeindex]];

                //get all voxel edges
                for(int id=0;id < data->GetVertexCount();id++)
                {
                    unsigned short edge_id=regularVertexData[cubeindex][id];
                    unsigned short corner0 = edge_id & 0x0F;
                    unsigned short corner1 = (edge_id & 0xF0) >> 4;
                    unsigned short twist=(edge_id>>8) & 0x0F;//edge position in array

                    vert0.set(i+voxel[corner0][0],j+voxel[corner0][1],k+voxel[corner0][2]);
                    vert1.set(i+voxel[corner1][0],j+voxel[corner1][1],k+voxel[corner1][2]);

                    if(step!=1)
                        surface->adjustVertices(vert0,vert1);

                    int * edge=&edges[(twist-1)*edges_size*edges_size*edges_size+(vert0[0]*edges_size*edges_size+vert0[1]*edges_size+vert0[2])];
                    if((*edge)==-1)
                    {
                        tmp_vertex=surface->getVertex(vert0[0],vert0[1],vert0[2],
                                             vert1[0],vert1[1],vert1[2]);

                        tmp_normal=surface->getNormal(vert0,vert1);

                        edge_list[id]=data_count/7;
                        (*edge)=edge_list[id];

                        data_buffer[data_count++] = tmp_vertex[0];
                        data_buffer[data_count++] = tmp_vertex[1];
                        data_buffer[data_count++] = tmp_vertex[2];
                        data_buffer[data_count++] = tmp_normal[0];
                        data_buffer[data_count++] = tmp_normal[1];
                        data_buffer[data_count++] = tmp_normal[2];

                        if(surface->field(vert0[0],vert0[1],vert0[2])->value > surface->field(vert1[0],vert1[1],vert1[2])->value)
                            data_buffer[data_count++]=surface->field(vert0[0],vert0[1],vert0[2])->texture_id;//*16.0+2.0;
                        else
                            data_buffer[data_count++]=surface->field(vert1[0],vert1[1],vert1[2])->texture_id;//*16.0+2.0;

                        if(data_count==capacity)
                        {
                            int n_capa=capacity*2;

                            GLfloat * tmp_buffer=new GLfloat[n_capa];

                            for(int i=0;i<data_count;i++)
                            {
                                tmp_buffer[i]=data_buffer[i];
                            }

                            delete [] data_buffer;
                            data_buffer=tmp_buffer;

                            capacity=n_capa;
                        }
                    }
                    else
                    {
                        edge_list[id]=(*edge);
                    }
                }

                //form triangles from edges
                for(int id=0;id < data->GetTriangleCount();id++)
                {
                    if(data_buffer[edge_list[data->vertexIndex[id*3  ]]*7+6]==0 &&
                       data_buffer[edge_list[data->vertexIndex[id*3+1]]*7+6]==0 &&
                       data_buffer[edge_list[data->vertexIndex[id*3+2]]*7+6]==0);
                    else
                    {
                        indices_buffer[indices_count++]=edge_list[data->vertexIndex[id*3]];
                        indices_buffer[indices_count++]=edge_list[data->vertexIndex[id*3+1]];
                        indices_buffer[indices_count++]=edge_list[data->vertexIndex[id*3+2]];

                        if(indices_count==indices_capacity)
                        {
                            int n_capa=indices_capacity*2;
                            GLushort * tmp_indices  = new GLushort[n_capa];
                            for(int i=0;i<indices_count;i++)
                            {
                                tmp_indices[i]=indices_buffer[i];
                            }
                            delete [] indices_buffer;

                            indices_buffer=tmp_indices;
                            indices_capacity=n_capa;
                        }
                    }
                }
            }

    if(data_count>0)
    {
        surface->vbo_buffer[level].create(data_count*sizeof(GLfloat), data_buffer);
        surface->ibo_buffer[level].create(indices_count*sizeof(GLushort), indices_buffer);

        surface->vertex_count[level]=data_count;
        surface->indices_count[level]=indices_count;
    }
    return data_count;

//    surface->vertex_count[level]=data_count;
  //  surface->indices_count[level]=indices_count;



//////////////////////////////////////////////////////////////////////////////////////////////  Transvoxel

/*


    int trans_data_count=0;
    int trans_indices_count=0;


    //position of all corners from 0,0,0
   // const int voxel[][3]={{0,0,0},{step,0,0},{0,0,step},{step,0,step},{0,step,0},{step,step,0},{0,step,step},{step,step,step}};
    const int transvoxel[][9][3] = {
        {{0, 0, 0},{0, step/2, 0},{0, step, 0},{0, step, step/2},{0, step, step},{0, step/2, step},{0, 0, step},{0, 0, step/2},{0, step/2, step/2}},
        {{0, 0, 0},{step/2, 0, 0},{step, 0, 0},{step, 0, step/2},{step, 0, step},{step/2, 0, step},{0, 0, step},{0, 0, step/2},{step/2, 0, step/2}},
        {{0, 0, 0},{step/2, 0, 0},{step, 0, 0},{step, step/2, 0},{step, step, 0},{step/2, step, 0},{0, step, 0},{0, step/2, 0},{step/2, step/2, 0}},
                        };
    const int corner[]={0x0,0x1,0x2,0x7,0x8,0x3,0x6,0x5,0x4,0x0,0x2,0x6,0x4};

    int min_x=0,min_y=0,min_z=0, max_x=size,max_y=size,max_z=size;

if(level>=TerrainSurface::LEVEL_1)
    for(int side=0;side<3;side++)
    {
        switch(side)
        {
        case TerrainSurface::X_SIDE: min_x=size;max_x=size+1; break;
        case TerrainSurface::Y_SIDE: min_y=size;max_y=size+1; break;
        case TerrainSurface::Z_SIDE: min_z=size;max_z=size+1; break;
        }

        //test whole voxel field
        for(int i=min_x; i<max_x; i+=step)
            for(int j=min_y; j<max_y; j+=step)
                for(int k=min_z; k<max_z; k+=step)
                {
                    cubeindex = 0;
                    unsigned int exp=1;

                    for(int c=0; c<9; c++)
                    {
                        if (surface->field(i+transvoxel[side][c][0], j+transvoxel[side][c][1], k+transvoxel[side][c][2])->value < isolevel)
                            cubeindex |= exp;

                        exp*=2;
                    }

                    if (cubeindex==0 || cubeindex==511)
                       continue;

                    unsigned int class_index = transitionCellClass[cubeindex];
                    const TransitionCellData *data = &transitionCellData[(class_index & 0x7F)];

                    for(int id=0;id < data->GetVertexCount();id++)
                    {
                        unsigned short edge_id=transitionVertexData[cubeindex][id];

                        unsigned short corner0 = edge_id & 0x0F;
                        unsigned short corner1 = (edge_id & 0xF0) >> 4;
                        unsigned short twist=(edge_id>>8) & 0x0F;//edge position in array
                        //dont know why is it switched
                        corner0=corner[corner0];
                        corner1=corner[corner1];

                        vert0.set(i+transvoxel[side][corner0][0],j+transvoxel[side][corner0][1],k+transvoxel[side][corner0][2]);
                        vert1.set(i+transvoxel[side][corner1][0],j+transvoxel[side][corner1][1],k+transvoxel[side][corner1][2]);

                        if(step!=1)
                            surface->adjustVertices(vert0,vert1);

                        if(twist==6 || twist==5 || twist==9)
                            twist=1;
                        else
                            twist=0;

                        //edge rotation problem horizonta vs vertical
                        int * edge=&edges[twist*edges_size*edges_size*edges_size + vert0[0]*edges_size*edges_size+vert0[1]*edges_size+vert0[2]];
                        if((*edge)==-1)
                        {
                            tmp_vertex=surface->getVertex(vert0[0],vert0[1],vert0[2],
                                                 vert1[0],vert1[1],vert1[2]);

                            tmp_normal=surface->getNormal(vert0,vert1);

                            edge_list[id]=(data_count+trans_data_count)/7;
                            (*edge)=edge_list[id];

                            data_buffer[data_count+trans_data_count++] = tmp_vertex[0];
                            data_buffer[data_count+trans_data_count++] = tmp_vertex[1];
                            data_buffer[data_count+trans_data_count++] = tmp_vertex[2];
                            data_buffer[data_count+trans_data_count++] = tmp_normal[0];
                            data_buffer[data_count+trans_data_count++] = tmp_normal[1];
                            data_buffer[data_count+trans_data_count++] = tmp_normal[2];

                            if(surface->field(vert0[0],vert0[1],vert0[2])->value > surface->field(vert1[0],vert1[1],vert1[2])->value)
                                data_buffer[data_count+trans_data_count++]=surface->field(vert0[0],vert0[1],vert0[2])->texture_id;
                            else
                                data_buffer[data_count+trans_data_count++]=surface->field(vert1[0],vert1[1],vert1[2])->texture_id;

                            if(data_count+trans_data_count==capacity)
                            {
                                int n_capa=capacity*2;

                                GLfloat * tmp_buffer=new GLfloat[n_capa];

                                for(int i=0;i<data_count+trans_data_count;i++)
                                {
                                    tmp_buffer[i]=data_buffer[i];
                                }

                                delete [] data_buffer;
                                data_buffer=tmp_buffer;

                                capacity=n_capa;
                            }
                        }
                        else
                        {
                            edge_list[id]=(*edge);
                        }
                    }

                    for(int id=0;id < data->GetTriangleCount();id++)
                    {
                        if(data_buffer[edge_list[data->vertexIndex[id*3  ]]*7+6]!=0 &&
                           data_buffer[edge_list[data->vertexIndex[id*3+1]]*7+6]!=0 &&
                           data_buffer[edge_list[data->vertexIndex[id*3+2]]*7+6]!=0)
                        {
                            indices_buffer[indices_count+trans_indices_count++]=edge_list[data->vertexIndex[id*3]];
                            indices_buffer[indices_count+trans_indices_count++]=edge_list[data->vertexIndex[id*3+1]];
                            indices_buffer[indices_count+trans_indices_count++]=edge_list[data->vertexIndex[id*3+2]];

                            if(trans_indices_count+indices_count==indices_capacity)
                            {
                                int n_capa=indices_capacity*2;
                                GLushort * tmp_indices  = new GLushort[n_capa];
                                for(int i=0;i<trans_indices_count+indices_count;i++)
                                {
                                    tmp_indices[i]=indices_buffer[i];
                                }
                                delete [] indices_buffer;

                                indices_buffer=tmp_indices;
                                indices_capacity=n_capa;
                            }
                        }
                    }
                }

        surface->trans_vertex_count[level][side]=trans_data_count;
        surface->trans_indices_count[level][side]=trans_indices_count;
        surface->trans_indices_offset[level][side]=data_count;

        data_count+=trans_data_count;
        indices_count+=trans_indices_count;
    }


    surface->vbo_buffer[level].create(data_count*sizeof(GLfloat), data_buffer);
    surface->ibo_buffer[level].create(indices_count*sizeof(GLushort), indices_buffer);
*/
}


int TerrainGraphics::transvoxel(TerrainSurface * surface, TerrainSurface::LOD level, TerrainSurface::Side side)
{
    int step=1;
    switch(level)
    {
    case TerrainSurface::LEVEL_0:   step=1;     break;
    case TerrainSurface::LEVEL_1:   step=2;     break;
    case TerrainSurface::LEVEL_2:   step=4;     break;
    case TerrainSurface::LEVEL_3:   step=8;     break;
    }

    int size=regionSize();
    int edges_size=size+2;

    int data_count=0;
    int indices_count=0;

    unsigned int cubeindex;
    Vertex3f tmp_normal,tmp_vertex;

    int edge_list[12];
    Vertex3i vert0,vert1;
    Vertex4f normal_vert0,normal_vert1;

    //position of all corners from 0,0,0
   // const int voxel[][3]={{0,0,0},{step,0,0},{0,0,step},{step,0,step},{0,step,0},{step,step,0},{0,step,step},{step,step,step}};
    const int voxel[][9][3] = {
        {{0, 0, 0},{0, step/2, 0},{0, step, 0},{0, step, step/2},{0, step, step},{0, step/2, step},{0, 0, step},{0, 0, step/2},{0, step/2, step/2}},
        {{0, 0, 0},{step/2, 0, 0},{step, 0, 0},{step, 0, step/2},{step, 0, step},{step/2, 0, step},{0, 0, step},{0, 0, step/2},{step/2, 0, step/2}},
        {{0, 0, 0},{step/2, 0, 0},{step, 0, 0},{step, step/2, 0},{step, step, 0},{step/2, step, 0},{0, step, 0},{0, step/2, 0},{step/2, step/2, 0}},
                        };
    const int corner[]={0x0,0x1,0x2,0x7,0x8,0x3,0x6,0x5,0x4,0x0,0x2,0x6,0x4};

    int min_x=0,min_y=0,min_z=0, max_x=size,max_y=size,max_z=size;
    switch(side)
    {
    case TerrainSurface::X_SIDE: min_x=size;max_x=size+1; break;
    case TerrainSurface::Y_SIDE: min_y=size;max_y=size+1; break;
    case TerrainSurface::Z_SIDE: min_z=size;max_z=size+1; break;
    }

    //reset edge field
    for(int i =0; i<2; i++)
       for(int j =min_x; j<=max_x; j++)
           for(int k =min_y; k<=max_y; k++)
               for(int l=min_z;l<=max_z;l++)
                   edges[i*edges_size*edges_size*edges_size+j*edges_size*edges_size+k*edges_size+l]=-1;

    //test whole voxel field
    for(int i=min_x; i<max_x; i+=step)
        for(int j=min_y; j<max_y; j+=step)
            for(int k=min_z; k<max_z; k+=step)
            {
                cubeindex = 0;
                unsigned int exp=1;

                for(int c=0; c<9; c++)
                {
                    if (surface->field(i+voxel[side][c][0], j+voxel[side][c][1], k+voxel[side][c][2])->value < isolevel)
                        cubeindex |= exp;

                    exp*=2;
                }

                if (cubeindex==0 || cubeindex==511)
                   continue;

                unsigned int class_index = transitionCellClass[cubeindex];
                const TransitionCellData *data = &transitionCellData[(class_index & 0x7F)];

                for(int id=0;id < data->GetVertexCount();id++)
                {
                    unsigned short edge_id=transitionVertexData[cubeindex][id];

                    unsigned short corner0 = edge_id & 0x0F;
                    unsigned short corner1 = (edge_id & 0xF0) >> 4;
                    unsigned short twist=(edge_id>>8) & 0x0F;//edge position in array
                    //dont know why is it switched
                    corner0=corner[corner0];
                    corner1=corner[corner1];

                    vert0.set(i+voxel[side][corner0][0],j+voxel[side][corner0][1],k+voxel[side][corner0][2]);
                    vert1.set(i+voxel[side][corner1][0],j+voxel[side][corner1][1],k+voxel[side][corner1][2]);

                    if(step!=1)
                        surface->adjustVertices(vert0,vert1);

                    if(twist==6 || twist==5 || twist==9)
                        twist=1;
                    else
                        twist=0;

                    //edge rotation problem horizonta vs vertical
                    int * edge=&edges[twist*edges_size*edges_size*edges_size + vert0[0]*edges_size*edges_size+vert0[1]*edges_size+vert0[2]];
                    if((*edge)==-1)
                    {
                        tmp_vertex=surface->getVertex(vert0[0],vert0[1],vert0[2],
                                             vert1[0],vert1[1],vert1[2]);

                        tmp_normal=surface->getNormal(vert0,vert1);

                        edge_list[id]=data_count/7;
                        (*edge)=edge_list[id];

                        data_buffer[data_count++] = tmp_vertex[0];
                        data_buffer[data_count++] = tmp_vertex[1];
                        data_buffer[data_count++] = tmp_vertex[2];
                        data_buffer[data_count++] = tmp_normal[0];
                        data_buffer[data_count++] = tmp_normal[1];
                        data_buffer[data_count++] = tmp_normal[2];

                        if(surface->field(vert0[0],vert0[1],vert0[2])->value > surface->field(vert1[0],vert1[1],vert1[2])->value)
                            data_buffer[data_count++]=surface->field(vert0[0],vert0[1],vert0[2])->texture_id;
                        else
                            data_buffer[data_count++]=surface->field(vert1[0],vert1[1],vert1[2])->texture_id;

                        if(data_count==capacity)
                        {
                            int n_capa=capacity*2;

                            GLfloat * tmp_buffer=new GLfloat[n_capa];

                            for(int i=0;i<data_count;i++)
                            {
                                tmp_buffer[i]=data_buffer[i];
                            }

                            delete [] data_buffer;
                            data_buffer=tmp_buffer;

                            capacity=n_capa;
                        }
                    }
                    else
                    {
                        edge_list[id]=(*edge);
                    }
                }

                for(int id=0;id < data->GetTriangleCount();id++)
                {
                    if(data_buffer[edge_list[data->vertexIndex[id*3  ]]*7+6]!=0 &&
                       data_buffer[edge_list[data->vertexIndex[id*3+1]]*7+6]!=0 &&
                       data_buffer[edge_list[data->vertexIndex[id*3+2]]*7+6]!=0)
                    {
                        indices_buffer[indices_count++]=edge_list[data->vertexIndex[id*3]];
                        indices_buffer[indices_count++]=edge_list[data->vertexIndex[id*3+1]];
                        indices_buffer[indices_count++]=edge_list[data->vertexIndex[id*3+2]];

                        if(indices_count==indices_capacity)
                        {
                            int n_capa=indices_capacity*2;
                            GLushort * tmp_indices  = new GLushort[n_capa];
                            for(int i=0;i<indices_count;i++)
                            {
                                tmp_indices[i]=indices_buffer[i];
                            }
                            delete [] indices_buffer;

                            indices_buffer=tmp_indices;
                            indices_capacity=n_capa;
                        }
                    }
                }
            }

    if(data_count > 0)
    {
        surface->trans_vbo_buffer[level][side].create(data_count*sizeof(GLfloat), data_buffer);
        surface->trans_ibo_buffer[level][side].create(indices_count*sizeof(GLushort), indices_buffer);

        surface->trans_vertex_count[level][side]=data_count;
        surface->trans_indices_count[level][side]=indices_count;
    }

    return data_count;
}

void TerrainGraphics::setVoxel(int x, int y, int z,signed char value, unsigned char texture_id)
{
    int i=x/node_size;
    int j=y/node_size;
    int k=z/node_size;

    if(i<nodes_x && j<nodes_y && k<nodes_z)
    {
        if(grid[i*nodes_z*nodes_y +j*nodes_z+k]==NULL)
        {
            grid[i*nodes_z*nodes_y +j*nodes_z+k]
                    =new TerrainSurface(this,i*node_size,j*node_size,k*node_size,node_size);
            map->insertObject(grid[i*nodes_z*nodes_y +j*nodes_z+k]);
        }

        grid[i*nodes_z*nodes_y +j*nodes_z+k]->setVoxel(x,y,z,value,texture_id);
    }
}

void TerrainGraphics::setVoxelTexture(int x, int y, int z, unsigned char texture_id)
{
    int a=x/node_size;
    int b=y/node_size;
    int c=z/node_size;

    if(a<nodes_x && b<nodes_y && c<nodes_z)
    {
        if(grid[a*nodes_z*nodes_y +b*nodes_z+c]==NULL)
        {
            grid[a*nodes_z*nodes_y +b*nodes_z+c]
                        =new TerrainSurface(this,a*node_size,b*node_size,c*node_size,node_size);
            map->insertObject(grid[a*nodes_z*nodes_y +b*nodes_z+c]);
        }

        grid[a*nodes_z*nodes_y +b*nodes_z+c]->setVoxelTexture(x,y,z,texture_id);
    }
}

unsigned char TerrainGraphics::materialId(unsigned char texture_vertival, unsigned char texture_horizontal)
{
    return texture_vertival*16+texture_horizontal;
}

void TerrainGraphics::setVoxelValue(int x, int y, int z, signed char value)
{
    int a=x/node_size;
    int b=y/node_size;
    int c=z/node_size;

    if(a<nodes_x && b<nodes_y && c<nodes_z)
    {
        if(grid[a*nodes_z*nodes_y +b*nodes_z+c]==NULL)
        {
            grid[a*nodes_z*nodes_y +b*nodes_z+c]
                    =new TerrainSurface(this,a*node_size,b*node_size,c*node_size,node_size);
            map->insertObject(grid[a*nodes_z*nodes_y +b*nodes_z+c]);
        }

        grid[a*nodes_z*nodes_y +b*nodes_z+c]->setVoxelValue(x,y,z,value);
    }
}
