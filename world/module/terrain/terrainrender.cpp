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
#include "terrainrender.h"

TerrainRender::TerrainRender()
{
    sampler_count=8;//8
}

TerrainRender::~TerrainRender()
{
}

bool TerrainRender::init()
{
    const char *vertex_file=":/shader/Terrain.vsh";
    const char *fragment_file=":/shader/Terrain.fsh";


    // overriding system locale until shaders are compiled
    setlocale(LC_NUMERIC, "C");

    program.loadFile(vertex_file,fragment_file);

    //to be rendered
    if((attribute_coord3d = program.getAttribute("coord3d"))==-1)
        return false;
    if((attribute_texcoord = program.getAttribute("tex_select"))==-1)
        return false;
    if((attribute_normal = program.getAttribute("normal"))==-1)
        return false;
    //support
    if((uniform_mvp = program.getUniform("mvp"))==-1)
        return false;

    uniform_texture_atlas= program. getUniform("texture_atlas");
    uniform_texture_normal=program.getUniform("texture_normal");

    shader_shadow.loadFile(":shader/Shadow.vsh",":shader/Shadow.fsh");
    if((attribute_shadow_coord3d = program.getAttribute("coord3d"))==-1)
        return false;
    if((uniform_shadow_mvp = program.getUniform("mvp"))==-1)
        return false;


    setlocale(LC_ALL, "");

    program.bind();

    program.uniform(uniform_texture_atlas,0);
    program.uniform(uniform_texture_normal,1);

    program.unbind();

    return true;
}

void TerrainRender::load(TerrainGraphics *base)
{
    this->base=base;
}

void TerrainRender::clearRenderList()
{
    base->clearRender();
}

void TerrainRender::render(const Matrix4f &mvp, unsigned int)
{
    program.bind();

    Texture::active(GL_TEXTURE0);
    base->atlas.bind();

    Texture::active(GL_TEXTURE1);
    base->normalmap.bind();

    program.uniformMatrix(uniform_mvp,mvp);

    glEnableVertexAttribArray(attribute_coord3d);
    glEnableVertexAttribArray(attribute_texcoord);
    glEnableVertexAttribArray(attribute_normal);

    TerrainSurface *tmp;
    while(base->renderCount()>0)
    {
        tmp=base->popRender();
        tmp->render(attribute_coord3d,attribute_normal,attribute_texcoord);
    }

    glDisableVertexAttribArray(attribute_coord3d);
    glDisableVertexAttribArray(attribute_texcoord);
    glDisableVertexAttribArray(attribute_normal);
    
    base->normalmap.unbind();
    base->atlas.unbind();
    Texture::active(GL_TEXTURE0);

    program.unbind();
}

void TerrainRender::renderShadow(const Matrix4f &mvp, unsigned int)
{
    shader_shadow.bind();
    shader_shadow.uniformMatrix(uniform_shadow_mvp,mvp);

    glEnableVertexAttribArray(attribute_shadow_coord3d);
/*
    TerrainSurface *tmp;
    while(base->renderCount()>0)
    {
        tmp=base->popRender();
        tmp->renderShadow(attribute_shadow_coord3d);
    }*/

    //HACK to be removed
    for(int i=0;i<base->regionsX();i++)
        for(int j=0;j<base->regionsY();j++)
            for(int k=0;k<base->regionsZ();k++)
            {
                TerrainSurface *tmp=(TerrainSurface*)base->regionAtIndex(i,j,k);
                if(tmp!=NULL)
                    tmp->renderShadow(attribute_shadow_coord3d);
            }

    glDisableVertexAttribArray(attribute_shadow_coord3d);

    shader_shadow.unbind();
}
