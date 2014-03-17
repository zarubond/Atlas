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
#include "terrainsurface.h"

#include "terraingraphics.h"

TerrainSurface::TerrainSurface(TerrainGraphics* terrain, int x, int y, int z, int size)
    :TerrainRegion(terrain,x,y,z,size)
{
    level=LEVEL_0;

    this->terrain=terrain;

    for(int i=0;i<4;i++)
    {
        vertex_count[i]=0;
        indices_count[i]=0;
        for(int k=0;k<3;k++)
        {
            trans_vertex_count[i][k]=0;
            trans_indices_count[i][k]=0;
        }
    }
    dirty=false;
}

TerrainSurface::~TerrainSurface()
{    
    clearLevel(LEVEL_0);
    clearLevel(LEVEL_1);
    clearLevel(LEVEL_2);
    clearLevel(LEVEL_3);
}


void TerrainSurface::render(GLint attribute_coord3d,GLint attribute_normal,GLint attribute_texcoord)
{
    vbo_buffer[level].bind();
    vbo_buffer[level].attribute(attribute_coord3d, 3, GL_FLOAT, 7*sizeof(GLfloat), 0);
    vbo_buffer[level].attribute(attribute_normal, 3, GL_FLOAT, 7*sizeof(GLfloat),3*sizeof(GLfloat));
    vbo_buffer[level].attribute(attribute_texcoord, 1, GL_FLOAT, 7*sizeof(GLfloat),6*sizeof(GLfloat));
    ibo_buffer[level].draw(GL_TRIANGLES, indices_count[level], GL_UNSIGNED_SHORT);
/*
    vbo_switch_buffer[level].bind();
    vbo_switch_buffer[level].attribute(attribute_coord3d, 3, GL_FLOAT, 7*sizeof(GLfloat), 0);
    vbo_switch_buffer[level].attribute(attribute_normal, 3, GL_FLOAT, 7*sizeof(GLfloat),3*sizeof(GLfloat));
    vbo_switch_buffer[level].attribute(attribute_texcoord, 1, GL_FLOAT, 7*sizeof(GLfloat),6*sizeof(GLfloat));
    ibo_switch_buffer[level].draw(GL_TRIANGLES, indices_switch_count[level], GL_UNSIGNED_SHORT);
    */

    const int neighbors[][3]={{1,0,0},{0,1,0},{0,0,1}};
    int patch;

    int x=pos_x/size,y=pos_y/size,z=pos_z/size;
    TerrainRegion * neighbor;
    for(int i=0;i<3;i++)
    {
        neighbor=terrain->regionAtIndex(x+neighbors[i][0],y+neighbors[i][1],z+neighbors[i][2]);
        if(neighbor!=NULL)
        {
            if(neighbor->level > level)
                patch=neighbor->level;
            else if(neighbor->level < level)
                patch=level;
            else
                patch=-1;

            if(patch!=-1 && trans_indices_count[patch][i]>0)
            {
                trans_vbo_buffer[patch][i].bind();
                trans_vbo_buffer[patch][i].attribute(attribute_coord3d, 3, GL_FLOAT, 7*sizeof(GLfloat), 0);
                trans_vbo_buffer[patch][i].attribute(attribute_normal, 3, GL_FLOAT, 7*sizeof(GLfloat),
                                                     3*sizeof(GLfloat));
                trans_vbo_buffer[patch][i].attribute(attribute_texcoord, 1, GL_FLOAT, 7*sizeof(GLfloat),
                                                     6*sizeof(GLfloat));
                trans_ibo_buffer[patch][i].draw(GL_TRIANGLES, trans_indices_count[patch][i], GL_UNSIGNED_SHORT);
                /*
                vbo_buffer[level].bind();
                vbo_buffer[level].attribute(attribute_coord3d, 3, GL_FLOAT, 7*sizeof(GLfloat), 0);
                vbo_buffer[level].attribute(attribute_normal, 3, GL_FLOAT, 7*sizeof(GLfloat),
                                                     3*sizeof(GLfloat));
                vbo_buffer[level].attribute(attribute_texcoord, 1, GL_FLOAT, 7*sizeof(GLfloat),
                                                     6*sizeof(GLfloat));
                ibo_buffer[level].draw(GL_TRIANGLES, trans_indices_count[patch][i], GL_UNSIGNED_SHORT,trans_indices_offset[patch][i]);
                */
            }
        }
    }
    vbo_buffer[level].unbind();
}

void TerrainSurface::renderShadow(GLint attribute_shadow_coord3d)
{
    vbo_buffer[level].bind();
    vbo_buffer[level].attribute(attribute_shadow_coord3d, 3, GL_FLOAT, 7*sizeof(GLfloat), 0);
    ibo_buffer[level].draw(GL_TRIANGLES, indices_count[level], GL_UNSIGNED_SHORT);
    vbo_buffer[level].unbind();
}

void TerrainSurface::clear()
{
    clearLevel(LEVEL_0);
    clearLevel(LEVEL_1);
    clearLevel(LEVEL_2);
    clearLevel(LEVEL_3);
}

void TerrainSurface::selectDraw(const Camera & camera)
{
    if(vertex_count[0]!=0)
    {
        Vertex3f pos(pos_x+16,pos_y+16,pos_z+16);
        pos-=camera.getPosition();
        float len=pos.length();
        if(len<65)
            level=LEVEL_0;
        else if(len<128)
        {
            level=LEVEL_1;
        }
        else if(len<256)
            level=LEVEL_2;
        else
            level=LEVEL_3;

        terrain->pushRender(this);
    }
}
/*
void TerrainSurface::rebuildLevel(TerrainSurface::LOD level)
{
    int step=1;
    switch(level)
    {
    case LEVEL_0:   step=1;     break;
    case LEVEL_1:   step=2;     break;
    case LEVEL_2:   step=4;     break;
    }

    clearLevel(level);

    GLfloat * tmp_data = NULL,* tmp_switch_data=NULL;
    GLushort * tmp_indices  = NULL,* tmp_switch_indices=NULL;

    GLint v_c=0,i_c=0,s_c=0;

    marchingCubes(tmp_data,tmp_indices,tmp_switch_data,tmp_switch_indices,v_c,i_c,s_c,step);
    if(i_c > 0)
    {
        vbo_buffer[level].create(v_c*sizeof(GLfloat), tmp_data);
        ibo_buffer[level].create(i_c*sizeof(GLushort), tmp_indices);

        vertex_count[level]=v_c;
        indices_count[level]=i_c;

        vbo_switch_buffer[level].create(s_c*sizeof(GLfloat)*7,tmp_switch_data);
        ibo_switch_buffer[level].create(s_c*sizeof(GLushort), tmp_switch_indices);

        vertex_switch_count[level]=s_c*7;
        indices_switch_count[level]=s_c;
    }

    delete [] tmp_data;
    delete [] tmp_indices;

    if(level>=LEVEL_1)
    {
        for(int side=0;side<3;side++)
        {
            tmp_data=NULL;
            tmp_indices  = NULL;
            v_c=0;
            i_c=0;

            transvoxel(tmp_data,tmp_indices,v_c,i_c,(Side)side,step);//2
            if(v_c > 0)
            {
                trans_vbo_buffer[level][side].create(v_c*sizeof(GLfloat), tmp_data);
                trans_ibo_buffer[level][side].create(i_c*sizeof(GLushort), tmp_indices);

                trans_vertex_count[level][side]=v_c;
                trans_indices_count[level][side]=i_c;
            }
            delete [] tmp_data;
            delete [] tmp_indices;
        }
    }
}
*/
void TerrainSurface::clearLevel(TerrainRegion::LOD level)
{
    if(vertex_count[level]>0)
    {
        vbo_buffer[level].clear();
        ibo_buffer[level].clear();

        vertex_count[level]=0;
        indices_count[level]=0;

        if(level>=LEVEL_1)
        {
            for(int side=0;side<3;side++)
            {
                trans_vbo_buffer[level][side].clear();
                trans_ibo_buffer[level][side].clear();

                trans_vertex_count[level][side]=0;
                trans_indices_count[level][side]=0;
            }
        }
    }
}

int TerrainSurface::type()
{
    return 1;
}
/*
void TerrainSurface::rebuild(bool force)
{
    if(dirty || force)
    {
        if(data_grid!=NULL)
        {
            rebuildLevel(LEVEL_0);
            rebuildLevel(LEVEL_1);
            rebuildLevel(LEVEL_2);
        }
        //voxel field change switch
        dirty=false;
    }
}
*/
