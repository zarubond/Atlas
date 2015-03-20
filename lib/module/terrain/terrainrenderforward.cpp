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

#include "terrainrenderforward.h"

TerrainRenderForward::TerrainRenderForward()
{
}

TerrainRenderForward::~TerrainRenderForward()
{
}

bool TerrainRenderForward::init()
{
    if(!program.loadFile(":/lib/shader/TerrainForward.vsh",":/lib/shader/TerrainForward.fsh"))
        cerr<<"Unable to load terrain forward shader"<<endl;

    //to be rendered
    if((attribute_coord = program.getAttribute("coord3d"))==-1)
        return false;
    if((attribute_normal = program.getAttribute("normal"))==-1)
        return false;
    if((attribute_texture = program.getAttribute("texture"))==-1)
        return false;
    //support
    if((uniform_mvp = program.getUniform("mvp"))==-1)
        return false;
    if((uniform_diffuse = program.getUniform("diffuse"))==-1)
        return false;
    if((uniform_ambient = program.getUniform("ambient"))==-1)
        return false;
    if((uniform_sun_dir = program.getUniform("sun_dir"))==-1)
        return false;

    u_texture_atlas[0]= program.getUniform("texture_atlas[0]");
    u_texture_atlas[1]= program.getUniform("texture_atlas[1]");
    u_texture_atlas[2]= program.getUniform("texture_atlas[2]");
    u_texture_atlas[3]= program.getUniform("texture_atlas[3]");

    program.bind();
    program.uniform(u_texture_atlas[0],0);
    program.uniform(u_texture_atlas[1],1);
    program.uniform(u_texture_atlas[2],2);
    program.uniform(u_texture_atlas[3],3);
    program.release();
    return true;
}

void TerrainRenderForward::render(WorldGraphics *world, const Environment *envi, const Matrix4f &mvp, unsigned int)
{
    if(!world->terrain.isVisible()) return;
    program.bind();

    world->terrain.textures()->bind();

    program.uniformMatrix(uniform_mvp,mvp);
    program.uniform(uniform_diffuse,envi->sun.getDiffuse());
    program.uniform(uniform_ambient,envi->ambient.getColor());
    program.uniform(uniform_sun_dir,envi->sun.getDirection());

    world->terrain.mutex.lock();
    int count;
    TerrainSurface ** list=world->terrain.renderList(count);
    for(int i=0;i<count;i++)
    {
        renderSurface(&world->terrain,list[i]);
    }
    world->terrain.mutex.unlock();

    program.release();
}

void TerrainRenderForward::renderSurface(TerrainGraphics * base ,TerrainSurface * surface)
{
    if(!surface->isDrawable()) return;

    if(!surface->vao.isCreated())
        setupRender(surface);

    surface->vao.bind();

    if(surface->index_cnt[surface->level]>0)
        surface->ibo.draw(IndexBuffer::TRIANGLES, surface->index_cnt[surface->level], IndexBuffer::UNSIGNED_SHORT,sizeof(GLushort)*surface->index_offset[surface->level]);

    const int neighbors[][3]={{1,0,0},{0,1,0},{0,0,1}};
    int patch;

    int x=surface->pos[0]/surface->size,y=surface->pos[1]/surface->size,z=surface->pos[2]/surface->size;
    TerrainSurface * neighbor;

    for(int i=0;i<3;i++)
    {
        neighbor=(TerrainSurface*)base->regionAtIndex(x+neighbors[i][0],y+neighbors[i][1],z+neighbors[i][2]);
        if(neighbor!=NULL)
        {
            if(neighbor->level > surface->level)
                patch=neighbor->level;
            else if(neighbor->level < surface->level)
                patch=surface->level;
            else
                patch=-1;

            if(patch!=-1 && surface->index_side_cnt[i][patch]>0 && surface->ibo.valid())
            {
                surface->ibo.draw(IndexBuffer::TRIANGLES, surface->index_side_cnt[i][patch], IndexBuffer::UNSIGNED_SHORT, sizeof(GLushort)*surface->index_side_offset[i][patch]);
            }
        }
    }
    surface->vao.release();
}

void TerrainRenderForward::setupRender(TerrainSurface *surface)
{
    surface->vao.create();
    surface->vao.bind();

    surface->ibo.bind();
    surface->vbo.bind();
    program.enableAttribute(attribute_coord);
    program.setAttribute(attribute_coord    , 3, GL_FLOAT, 19*sizeof(GLubyte), 0);
    program.enableAttribute(attribute_normal);
    program.setAttribute(attribute_normal   , 3, GL_BYTE,  19*sizeof(GLubyte), 3*sizeof(GLfloat));
    program.enableAttribute(attribute_texture);
    program.setAttribute(attribute_texture  , 4, GL_UNSIGNED_BYTE,  19*sizeof(GLubyte), 3*sizeof(GLfloat) + 3*sizeof(GLbyte));
    surface->vao.release();
}
