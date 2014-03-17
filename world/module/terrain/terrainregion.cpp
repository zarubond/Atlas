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
#include "terrainregion.h"
#include "transvoxel.cpp"

#include "terrainbase.h"

TerrainRegion::TerrainRegion(TerrainBase *terrain, int x, int y, int z, int size):
    edges_size(size+2),size(size),terrain(terrain)
{
    pos_x=x;
    pos_y=y;
    pos_z=z;
    isolevel=0;

    dirty=true;

    data_grid=new TerrainVoxel[size*size*size];
    for(int i=0;i<size*size*size;i++)
    {
        data_grid[i].value=0;
        data_grid[i].texture_id=0;
    }
}

TerrainRegion::~TerrainRegion()
{
    delete [] data_grid;
}

//idea from
//http://www.angelfire.com/linux/myp/MCAdvanced/MCImproved.html
Vertex4f TerrainRegion::gradientNormal(int i,int j,int k,int step)
{
    Vertex3f normal;
    normal[0] = (field(i-step,j     ,k)->value - field(i+step,j     ,k)->value);
    normal[1] = (field(i     ,j-step,k)->value - field(i     ,j+step,k)->value);
    normal[2] = (field(i     ,j,k-step)->value - field(i     ,j,k+step)->value);
    normal.normalize();

    return Vertex4f(normal[0], normal[1], normal[2], field(i,j,k)->value);
}

Vertex4f TerrainRegion::gradientNormal(const Vertex3i &vertex)
{
    Vertex3f normal;
    normal[0] = (field(vertex[0]-1,vertex[1]  ,vertex[2])->value
               - field(vertex[0]+1,vertex[1]  ,vertex[2])->value);
    normal[1] = (field(vertex[0]  ,vertex[1]-1,vertex[2])->value
               - field(vertex[0]  ,vertex[1]+1,vertex[2])->value);
    normal[2] = (field(vertex[0]  ,vertex[1]  ,vertex[2]-1)->value
               - field(vertex[0]  ,vertex[1]  ,vertex[2]+1)->value);
    normal.normalize();

    return Vertex4f(normal[0], normal[1], normal[2], field(vertex[0],vertex[1],vertex[2])->value);
}

Vertex3f TerrainRegion::getNormal(const Vertex3i &vert0, const Vertex3i &vert1)
{
    Vertex3f normal0;
    normal0[0] =(field(vert0[0]-1,vert0[1]  ,vert0[2]  )->value
               - field(vert0[0]+1,vert0[1]  ,vert0[2]  )->value);
    normal0[1] =(field(vert0[0]  ,vert0[1]-1,vert0[2]  )->value
               - field(vert0[0]  ,vert0[1]+1,vert0[2]  )->value);
    normal0[2] =(field(vert0[0]  ,vert0[1]  ,vert0[2]-1)->value
               - field(vert0[0]  ,vert0[1]  ,vert0[2]+1)->value);
    normal0.normalize();

    Vertex3f normal1;
    normal1[0] =(field(vert1[0]-1,vert1[1]  ,vert1[2]  )->value
               - field(vert1[0]+1,vert1[1]  ,vert1[2]  )->value);
    normal1[1] =(field(vert1[0]  ,vert1[1]-1,vert1[2]  )->value
               - field(vert1[0]  ,vert1[1]+1,vert1[2]  )->value);
    normal1[2] =(field(vert1[0]  ,vert1[1]  ,vert1[2]-1)->value
               - field(vert1[0]  ,vert1[1]  ,vert1[2]+1)->value);
    normal1.normalize();


    Vertex3f tmp;
    float mu = 0;
    float a=field(vert0[0],vert0[1],vert0[2])->value;
    float b=field(vert1[0],vert1[1],vert1[2])->value - a;

    if(b!=0)
        mu=(isolevel - a) / (b);

    tmp[0] = normal0[0] + mu * (normal1[0] - normal0[0]);
    tmp[1] = normal0[1] + mu * (normal1[1] - normal0[1]);
    tmp[2] = normal0[2] + mu * (normal1[2] - normal0[2]);
  //  tmp=a+mu*(b-a);

    return tmp;
}

Vertex3f TerrainRegion::getVertex(float x1, float y1, float z1, float x2, float y2, float z2)
{

    float val1=field(x1,y1,z1)->value;
    float val2=field(x2,y2,z2)->value;

    x1+=pos_x; x2+=pos_x;
    y1+=pos_y; y2+=pos_y;
    z1+=pos_z; z2+=pos_z;

    Vertex3f tmp;
    float mu =0;

    if(val2 - val1!=0)
        mu=(isolevel - val1) / (val2 - val1);

    tmp[0] = x1 + mu * (x2 - x1);
    tmp[1] = y1 + mu * (y2 - y1);
    tmp[2] = z1 + mu * (z2 - z1);
  //  tmp=a+mu*(b-a);

    return tmp;
}

void TerrainRegion::adjustVertices(Vertex3i & v1,Vertex3i & v2)
{
    float val1=field(v1[0],v1[1],v1[2])->value;
    float val2=field(v2[0],v2[1],v2[2])->value;
    const int voxel[][3]={{1,0,0},{0,1,0},{0,0,1}};

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
                     float tmp_val=field(v2[0]+voxel[a][0],v2[1]+voxel[a][1],v2[2]+voxel[a][2])->value;
                     if((tmp_val<0 && val2<0) || (tmp_val>0 && val2>0))
                     {
                         v2[a]++;
                         val2=tmp_val;
                     }
                     else
                         break;
                 }
                 v1[a]=v2[a]+1;
                 val1=field(v1[0],v1[1],v1[2])->value;

             }
             else//y1<y2
             {
                 for(int i=0;i<d-1;i++)
                 {
                     float tmp_val=field(v1[0]+voxel[a][0],v1[1]+voxel[a][1],v1[2]+voxel[a][2])->value;
                     if((tmp_val<0 && val1<0) || (tmp_val>0 && val1>0))
                     {
                         v1[a]++;
                         val1=tmp_val;
                     }
                     else
                         break;
                 }
                 v2[a]=v1[a]+1;
                 val2=field(v2[0],v2[1],v2[2])->value;
             }
         }
    }
}
/*
void TerrainRegion::marchingCubes(GLfloat *&buffer, GLushort *&indices, GLfloat *&buffer_switch, GLushort *&indices_switch,
                                  GLint &data_count, GLint &indices_count,GLint &switch_count, int step)
{
    if(data_grid==NULL)
        return;

    data_count=0;
    indices_count=0;

    int capacity=7000,indices_capacity=18000;

    buffer = new GLfloat[capacity];
    indices  = new GLushort[indices_capacity];

    buffer_switch = NULL;
    indices_switch  = NULL;
    switch_count=0;

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
                    if (field(i+voxel[c][0],j+voxel[c][1],k+voxel[c][2])->value < isolevel)
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
                        adjustVertices(vert0,vert1);

                    int * edge=&edges[(twist-1)*edges_size*edges_size*edges_size+(vert0[0]*edges_size*edges_size+vert0[1]*edges_size+vert0[2])];
                    if((*edge)==-1)
                    {
                        tmp_vertex=getVertex(vert0[0],vert0[1],vert0[2],
                                             vert1[0],vert1[1],vert1[2]);

                        tmp_normal=getNormal(vert0,vert1);

                        edge_list[id]=data_count/7;
                        (*edge)=edge_list[id];

                        buffer[data_count++] = tmp_vertex[0];
                        buffer[data_count++] = tmp_vertex[1];
                        buffer[data_count++] = tmp_vertex[2];
                        buffer[data_count++] = tmp_normal[0];
                        buffer[data_count++] = tmp_normal[1];
                        buffer[data_count++] = tmp_normal[2];

                        if(field(vert0[0],vert0[1],vert0[2])->value > field(vert1[0],vert1[1],vert1[2])->value)
                            buffer[data_count++]=field(vert0[0],vert0[1],vert0[2])->texture_id;
                        else
                            buffer[data_count++]=field(vert1[0],vert1[1],vert1[2])->texture_id;

                        //buffer[data_count++]=field(vert1[0],vert1[1],vert1[2])->texture_id;

                        if(data_count==capacity)
                        {
                            int n_capa=capacity*2;

                            GLfloat * tmp_buffer=new GLfloat[n_capa];

                            for(int i=0;i<data_count;i++)
                            {
                                tmp_buffer[i]=buffer[i];
                            }

                            delete [] buffer;
                            buffer=tmp_buffer;

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
                    if(buffer[edge_list[data->vertexIndex[id*3  ]]*7+6]!=0 &&
                       buffer[edge_list[data->vertexIndex[id*3+1]]*7+6]!=0 &&
                       buffer[edge_list[data->vertexIndex[id*3+2]]*7+6]!=0)
                    {
                       // if(buffer[edge_list[data->vertexIndex[id*3  ]]*7+6] == buffer[edge_list[data->vertexIndex[id*3+1]]*7+6] &&
                         //  buffer[edge_list[data->vertexIndex[id*3+1]]*7+6] == buffer[edge_list[data->vertexIndex[id*3+2]]*7+6])
                        {
                            indices[indices_count++]=edge_list[data->vertexIndex[id*3]];
                            indices[indices_count++]=edge_list[data->vertexIndex[id*3+1]];
                            indices[indices_count++]=edge_list[data->vertexIndex[id*3+2]];

                            if(indices_count==indices_capacity)
                            {
                                int n_capa=indices_capacity*2;
                                GLushort * tmp_indices  = new GLushort[n_capa];
                                for(int i=0;i<indices_count;i++)
                                {
                                    tmp_indices[i]=indices[i];
                                }
                                delete [] indices;

                                indices=tmp_indices;
                                indices_capacity=n_capa;
                            }
                        }/*
                        else
                        {
                            int a=buffer[edge_list[data->vertexIndex[id*3+0]]*7+6];
                            int b=buffer[edge_list[data->vertexIndex[id*3+1]]*7+6];
                            int c=buffer[edge_list[data->vertexIndex[id*3+2]]*7+6];

                            float tex=a*64+b*8+c;

                            indices_switch[switch_count] = switch_count;
                            indices_switch[switch_count+1] = switch_count+1;
                            indices_switch[switch_count+2] = switch_count+2;
                            switch_count+=3;

                            for(int i=0;i<6;i++)
                                buffer_switch[buffer_switch_count++]=buffer[edge_list[data->vertexIndex[id*3]]*7+i];
                            buffer_switch[buffer_switch_count++]=tex;

                            for(int i=0;i<6;i++)
                                buffer_switch[buffer_switch_count++]=buffer[edge_list[data->vertexIndex[id*3+1]]*7+i];
                            buffer_switch[buffer_switch_count++]=tex;

                            for(int i=0;i<6;i++)
                                buffer_switch[buffer_switch_count++]=buffer[edge_list[data->vertexIndex[id*3+2]]*7+i];
                            buffer_switch[buffer_switch_count++]=tex;

                        }*

                    }
                }
            }
}

void TerrainRegion::transvoxel(GLfloat *&buffer, GLushort *&indices, GLint &data_count, GLint &indices_count, Side side, int step)
{
    if(data_grid==NULL)
        return;

    data_count=0;
    indices_count=0;

    int capacity=7000,indices_capacity=9000;

    buffer = new GLfloat[capacity];
    indices  = new GLushort[indices_capacity];

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
    case X_SIDE: min_x=size;max_x=size+1; break;
    case Y_SIDE: min_y=size;max_y=size+1; break;
    case Z_SIDE: min_z=size;max_z=size+1; break;
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
                    if (field(i+voxel[side][c][0], j+voxel[side][c][1], k+voxel[side][c][2])->value < isolevel)
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
                        adjustVertices(vert0,vert1);

                    if(twist==6 || twist==5 || twist==9)
                        twist=1;
                    else
                        twist=0;

                    //edge rotation problem horizonta vs vertical
                    int * edge=&edges[twist*edges_size*edges_size*edges_size + vert0[0]*edges_size*edges_size+vert0[1]*edges_size+vert0[2]];
                    if((*edge)==-1)
                    {
                        tmp_vertex=getVertex(vert0[0],vert0[1],vert0[2],
                                             vert1[0],vert1[1],vert1[2]);

                        tmp_normal=getNormal(vert0,vert1);

                        edge_list[id]=data_count/7;
                        (*edge)=edge_list[id];

                        buffer[data_count++] = tmp_vertex[0];
                        buffer[data_count++] = tmp_vertex[1];
                        buffer[data_count++] = tmp_vertex[2];
                        buffer[data_count++] = tmp_normal[0];
                        buffer[data_count++] = tmp_normal[1];
                        buffer[data_count++] = tmp_normal[2];

                        if(field(vert0[0],vert0[1],vert0[2])->value > field(vert1[0],vert1[1],vert1[2])->value)
                            buffer[data_count++]=field(vert0[0],vert0[1],vert0[2])->texture_id;
                        else
                            buffer[data_count++]=field(vert1[0],vert1[1],vert1[2])->texture_id;

                        if(data_count==capacity)
                        {
                            int n_capa=capacity*2;

                            GLfloat * tmp_buffer=new GLfloat[n_capa];

                            for(int i=0;i<data_count;i++)
                            {
                                tmp_buffer[i]=buffer[i];
                            }

                            delete [] buffer;
                            buffer=tmp_buffer;

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
                    if(buffer[edge_list[data->vertexIndex[id*3  ]]*7+6]!=0 &&
                       buffer[edge_list[data->vertexIndex[id*3+1]]*7+6]!=0 &&
                       buffer[edge_list[data->vertexIndex[id*3+2]]*7+6]!=0)
                    {
                        indices[indices_count++]=edge_list[data->vertexIndex[id*3]];
                        indices[indices_count++]=edge_list[data->vertexIndex[id*3+1]];
                        indices[indices_count++]=edge_list[data->vertexIndex[id*3+2]];

                        if(indices_count==indices_capacity)
                        {
                            int n_capa=indices_capacity*2;
                            GLushort * tmp_indices  = new GLushort[n_capa];
                            for(int i=0;i<indices_count;i++)
                            {
                                tmp_indices[i]=indices[i];
                            }
                            delete [] indices;

                            indices=tmp_indices;
                            indices_capacity=n_capa;
                        }
                    }
                }

            }
}

*/
bool TerrainRegion::inBox(int x, int y, int z, int w)
{
    if(pos_x>=x && pos_x+size<=x+w && pos_y>=y && pos_y+size<=y+w && pos_z>=z && pos_z+size<=z+w)
        return true;
    return false;
}

TerrainVoxel *TerrainRegion::voxelField()
{
    return data_grid;
}

Vertex3f TerrainRegion::interpolate(Vertex4f a, Vertex4f b)
{
    Vertex3f tmp;
    float mu = 0;

    if(b[3] - a[3]!=0)
        mu=(isolevel - a[3]) / (b[3] - a[3]);

    tmp[0] = a[0] + mu * (b[0] - a[0]);
    tmp[1] = a[1] + mu * (b[1] - a[1]);
    tmp[2] = a[2] + mu * (b[2] - a[2]);
  //  tmp=a+mu*(b-a);

    return tmp;
}

bool TerrainRegion::isFinite(float var)
{
    //android's std is missing isfinite!!!
#ifndef Q_WS_ANDROID
    return std::isfinite(var);
#else
    return finite(var);
#endif
}

void TerrainRegion::voxelCollision(const Vertex3i &vox, const Vertex3f &a, Collision &b)
{
    int cubeindex=0;
    int step=1;

    Vertex3f vertex_list[12];

    //position of all corners from 0,0,0
    const int voxel[][3]={{0,0,0},{step,0,0},{0,0,step},{step,0,step},{0,step,0},{step,step,0},{0,step,step},{step,step,step}};
    Vertex3i vert0,vert1;
    int i=vox[0],j=vox[1],k=vox[2];

    i%=size;
    j%=size;
    k%=size;

    int exp=1;
                //test voxel position
    for(int c=0;c<8;c++)
    {
        if (field(i+voxel[c][0],j+voxel[c][1],k+voxel[c][2])->value < isolevel)
            cubeindex |= exp;
        exp*=2;
    }

    if (cubeindex==0 || cubeindex==255)
        return;

    const RegularCellData *data = &regularCellData[regularCellClass[cubeindex]];

    //get all voxel edges
    for(int id=0;id < data->GetVertexCount();id++)
    {
        unsigned short edge_id=regularVertexData[cubeindex][id];
        unsigned short corner0 = edge_id & 0x0F;
        unsigned short corner1 = (edge_id & 0xF0) >> 4;

        vert0.set(i+voxel[corner0][0],j+voxel[corner0][1],k+voxel[corner0][2]);
        vert1.set(i+voxel[corner1][0],j+voxel[corner1][1],k+voxel[corner1][2]);

        vertex_list[id]=getVertex(vert0[0],vert0[1],vert0[2],vert1[0],vert1[1],vert1[2]);
    }

    Triangle triangle;

    //form triangles from edges
    for(int id=0;id < data->GetTriangleCount();id++)
    {
        triangle.set(vertex_list[data->vertexIndex[id*3]]
                    ,vertex_list[data->vertexIndex[id*3+1]]
                    ,vertex_list[data->vertexIndex[id*3+2]]);

        Vertex3f tmp=triangle.collision(a,b.collision);
        if(tmp!=b.collision)
        {
            b.normal=triangle.normal();
            b.collision=tmp;
            b.object=this;
        }
    }

}

int TerrainRegion::type()
{
    return terrain->type();
}

TerrainVoxel *TerrainRegion::field(int x, int y, int z)
{
    if(x<0 || y<0 || z<0 || x>=size || y>=size || z>=size)
    {
        return terrain->getVoxel(x+pos_x,y+pos_y,z+pos_z);
    }

    return &data_grid[x*(size)*(size)+y*(size)+z];
}

void TerrainRegion::setVoxel(int x, int y, int z, signed char value, unsigned char texture_id)
{
    x-=pos_x;
    y-=pos_y;
    z-=pos_z;

    data_grid[x*size*size+y*size+z].texture_id=texture_id;
    data_grid[x*size*size+y*size+z].value=value;

    dirty=true;
}

void TerrainRegion::setVoxelValue(int x, int y, int z, signed char value)
{    x-=pos_x;
    y-=pos_y;
    z-=pos_z;

    data_grid[x*size*size+y*size+z].value=value;

    dirty=true;
}

void TerrainRegion::setVoxelTexture(int x, int y, int z, int texture_id)
{
    x-=pos_x;
    y-=pos_y;
    z-=pos_z;

    data_grid[x*(size)*(size)+y*(size)+z].texture_id=texture_id;

    dirty=true;
}

TerrainVoxel TerrainRegion::getVoxel(Vertex3i point)
{
    return getVoxel(point[0],point[1],point[2]);
}

TerrainVoxel TerrainRegion::getVoxel(int x, int y, int z)
{
    TerrainVoxel vox;
    if(x >= pos_x && x < pos_x+size && y >= pos_y && y < pos_y+size
            && z >= pos_z && z < pos_z+size)
    {/*
        if(data_grid==NULL)
        {
            data_grid=new TerrainVoxel[(size)*(size)*(size)];
            for(int i=0;i<(size)*(size)*(size);i++)
            {
                data_grid[i].value=0;//blank_value;//isolevel;
                data_grid[i].texture_id=0;
            }
        }*/

        vox=*field(x-pos_x,y-pos_y,z-pos_z);
    }
    return vox;
}


float TerrainRegion::getIsolevel()
{
    return isolevel;
}

void TerrainRegion::selectDraw(const Camera & )
{
}

void TerrainRegion::collision(const Vertex3f &a, Collision &b)
{
    if(data_grid==NULL)
        return;

    Vertex3f start=a,end=b.collision;

    Vertex3f v=end-start;
    if(start[0] < pos_x)
    {
        start[0]=pos_x;
        start[1]=a[1]+((pos_x-a[0])*v[1])/v[0];
        start[2]=a[2]+((pos_x-a[0])*v[2])/v[0];
    }

    if(start[0] > pos_x+size)
    {
        start[0]=pos_x+size;
        start[1]=a[1]+((pos_x+size-a[0])*v[1])/v[0];
        start[2]=a[2]+((pos_x+size-a[0])*v[2])/v[0];
    }

    if(start[1] < pos_y)
    {
        start[0]=a[0]+((pos_y-a[1])*v[0])/v[1];
        start[1]=pos_y;
        start[2]=a[2]+((pos_y-a[1])*v[2])/v[1];
    }

    if(start[1] > pos_y+size)
    {
        start[0]=a[0]+((pos_y+size-a[1])*v[0])/v[1];
        start[1]=pos_y+size;
        start[2]=a[2]+((pos_y+size-a[1])*v[2])/v[1];
    }

    if(start[2] < pos_z)
    {
        start[0]=a[0]+((pos_z-a[2])*v[0])/v[2];
        start[1]=a[1]+((pos_z-a[2])*v[1])/v[2];
        start[2]=pos_z;
    }

    if(start[2] > pos_z+size)
    {
        start[0]=a[0]+((pos_z+size-a[2])*v[0])/v[2];
        start[1]=a[1]+((pos_z+size-a[2])*v[1])/v[2];
        start[2]=pos_z+size;
    }


    //http://www.gamerendering.com/2009/07/20/grid-traversal/
    Vertex3i st=start;
    Vertex3d dir=end;
    dir-=start;

    float len=dir.length()+15;//10;
    int maxDepth=len;
    dir/=len;

    Vertex3i step(sgn(dir[0]),sgn(dir[1]),sgn(dir[2]));

    Vertex3d cellBoundary(
                st[0] + (step[0] > 0 ? 1 : 0),
                st[1] + (step[1] > 0 ? 1 : 0),
                st[2] + (step[2] > 0 ? 1 : 0));
    Vertex3d tMax(
                (cellBoundary[0] - start[0]) / dir[0],
                (cellBoundary[1] - start[1]) / dir[1],
                (cellBoundary[2] - start[2]) / dir[2]);
    if (! isFinite(tMax[0])) tMax[0] = INFINITY;
    if (! isFinite(tMax[1])) tMax[1] = INFINITY;
    if (! isFinite(tMax[2])) tMax[2] = INFINITY;

    Vertex3d tDelta(
                step[0] / dir[0],
                step[1] / dir[1],
                step[2] / dir[2]);

    if (!isFinite(tDelta[0])) tDelta[0] = INFINITY;
    if (!isFinite(tDelta[1])) tDelta[1] = INFINITY;
    if (!isFinite(tDelta[2])) tDelta[2] = INFINITY;


    for (int i = 0; i < maxDepth; i++)
    {
        if(st[0]<pos_x || st[0]>pos_x+size || st[1]<pos_y || st[1]>pos_y+size || st[2]<pos_z || st[2]>pos_z+size)
            break;

        voxelCollision(st,a,b);

        if (tMax[0] < tMax[1] && tMax[0] < tMax[2])
        {
            st[0] += step[0];
            tMax[0] += tDelta[0];
        }
        else if (tMax[1] < tMax[2])
        {
            st[1] += step[1];
            tMax[1] += tDelta[1];
        }
        else
        {
            st[2] += step[2];
            tMax[2] += tDelta[2];
        }
    }
}

//int * TerrainRegion::edges=NULL;
