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

#include "terrainrender.h"

TerrainRender::TerrainRender()
{
}

TerrainRender::~TerrainRender()
{
}

bool TerrainRender::init()
{
    if(!program.loadFile(":/lib/shader/Terrain.vsh",":/lib/shader/Terrain.fsh"))
        cerr<<"Unable to load terrain shader"<<endl;

    //to be rendered
    if((attribute_coord3d = program.getAttribute("coord3d"))==-1)
        return false;
    if((attribute_normal = program.getAttribute("normal"))==-1)
        return false;
    if((attribute_texture = program.getAttribute("texture"))==-1)
        return false;
    //support
    if((uniform_mvp = program.getUniform("mvp"))==-1)
        return false;

    uniform_texture_atlas= program.getUniform("texture_atlas");

    program.bind();
    program.uniform(uniform_texture_atlas,0);

    uniform_texture_normal=program.getUniform("texture_normal");
    program.uniform(uniform_texture_normal,1);

    //SHADOW
    shader_shadow.loadFile(":lib/shader/Shadow.vsh",":lib/shader/Shadow.fsh");
    if((attribute_shadow_coord3d = program.getAttribute("coord3d"))==-1)
        return false;
    if((uniform_shadow_mvp = program.getUniform("mvp"))==-1)
        return false;

    program.release();
    return true;
}

void TerrainRender::render(WorldGraphics *world, const Environment *envi, const Matrix4f &mvp, unsigned int)
{
    if(!world->terrain.isVisible()) return;

    program.bind();

    Texture::active(Texture::TEXTURE0);
    world->terrain.textures()->bind();

    Texture::active(Texture::TEXTURE1);
    world->terrain.normalMaps()->bind();

    program.uniformMatrix(uniform_mvp,mvp);

    world->terrain.mutex.lock();

    int count;
    TerrainSurface ** list=world->terrain.renderList(count);
    for(int i=0;i<count;i++)
    {
        renderSurface(&world->terrain, list[i]);
    }
    world->terrain.mutex.unlock();
    Texture::active(Texture::TEXTURE0);

    program.release();
}

void TerrainRender::renderSurface(TerrainGraphics * terrain, TerrainSurface * surface)
{
    if(!surface->drawable) return;
   // level=LEVEL_0;//HACK


    if(surface->index_cnt[surface->level]>0)
    {
        if(!surface->vao.isCreated())
            setupRender(surface);

        surface->vao.bind();

        surface->ibo.draw(IndexBuffer::TRIANGLES, surface->index_cnt[surface->level],
                IndexBuffer::UNSIGNED_SHORT,sizeof(GLushort)*surface->index_offset[surface->level]);

        static const int neighbors[][3]={{1,0,0},{0,1,0},{0,0,1}};

        int x=surface->pos[0]/surface->size, y=surface->pos[1]/surface->size, z=surface->pos[2]/surface->size;
        TerrainSurface * neighbor;
        int patch;

        for(int i=0;i<3;i++)
        {
            neighbor=(TerrainSurface*)terrain->regionAtIndex(x+neighbors[i][0],y+neighbors[i][1],z+neighbors[i][2]);
            if(neighbor!=NULL)
            {
                if(neighbor->level > surface->level)
                    patch=neighbor->level;
                else if(neighbor->level < surface->level)
                    patch=surface->level;
                else
                    patch=-1;

                if(patch!=-1 && surface->index_side_cnt[i][patch]>0)
                {
                    surface->ibo.draw(IndexBuffer::TRIANGLES, surface->index_side_cnt[i][patch],
                                      IndexBuffer::UNSIGNED_SHORT, sizeof(GLushort)*surface->index_side_offset[i][patch]);
                }
            }
        }

        surface->vao.release();
    }
}

void TerrainRender::setupRender(TerrainSurface *surface)
{
    surface->vao.create();
    surface->vao.bind();

    surface->ibo.bind();
    surface->vbo.bind();
    program.enableAttribute(attribute_coord3d);
    program.setAttribute(attribute_coord3d  , 3, GL_FLOAT, 19*sizeof(GLubyte), 0);
    program.enableAttribute(attribute_normal);
    program.setAttribute(attribute_normal   , 3, GL_BYTE,  19*sizeof(GLubyte), 3*sizeof(GLfloat));
    program.enableAttribute(attribute_texture);
    program.setAttribute(attribute_texture  , 4, GL_UNSIGNED_BYTE,  19*sizeof(GLubyte), 3*sizeof(GLfloat) + 3*sizeof(GLbyte));

    surface->vao.release();
}

void TerrainRender::renderShadow(WorldGraphics *world, const Environment *envi, const Matrix4f &mvp, unsigned int)
{
    if(!world->terrain.isVisible()) return;

    shader_shadow.bind();
    shader_shadow.uniformMatrix(uniform_shadow_mvp,mvp);

    world->terrain.mutex.lock();
    //HACK to be removed
    for(int i=0;i<world->terrain.regionsX();i++)
        for(int j=0;j<world->terrain.regionsY();j++)
            for(int k=0;k<world->terrain.regionsZ();k++)
            {
                TerrainSurface *tmp=(TerrainSurface*)world->terrain.regionAtIndex(i,j,k);
                if(tmp!=NULL)
                    renderSurfaceShadow(tmp);
            }
    world->terrain.mutex.unlock();
    shader_shadow.release();
}

void TerrainRender::renderSurfaceShadow(TerrainSurface * surface)
{
    if(surface->drawable && surface->index_cnt[surface->level]>0)
    {
        if(!surface->shadow_vao.isCreated())
            setupShadowRender(surface);

        surface->shadow_vao.bind();
        surface->ibo.draw(IndexBuffer::TRIANGLES, surface->index_cnt[surface->level],
                IndexBuffer::UNSIGNED_SHORT, sizeof(GLushort)*surface->index_offset[surface->level]);
        surface->shadow_vao.release();
    }
}

void TerrainRender::setupShadowRender(TerrainSurface * surface)
{
    surface->shadow_vao.create();
    surface->shadow_vao.bind();
    surface->ibo.bind();
    surface->vbo.bind();
    shader_shadow.enableAttribute(attribute_shadow_coord3d);
    shader_shadow.setAttribute(attribute_shadow_coord3d, 3, GL_FLOAT, 19*sizeof(GLubyte), 0);
    surface->shadow_vao.release();
}
