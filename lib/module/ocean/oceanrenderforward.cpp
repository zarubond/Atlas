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

#include "oceanrenderforward.h"

OceanRenderForward::OceanRenderForward()
{
}

OceanRenderForward::~OceanRenderForward()
{
}

bool OceanRenderForward::init()
{
    const char * vertex_file=":lib/shader/OceanForward.vsh";
    const char * fragment_file=":lib/shader/OceanForward.fsh";

    shader.loadFile(vertex_file,fragment_file);

    if((uniform_time = shader.getUniform("time"))==-1) return false;
    if((uniform_mvp = shader.getUniform("mvp"))==-1) return false;
    if((uniform_noise = shader.getUniform("noise"))==-1) return false;
    if((uniform_water_height = shader.getUniform("water_height"))==-1) return false;
    if((uniform_color = shader.getUniform("color"))==-1) return false;
    if((u_size = shader.getUniform("size"))==-1) return false;

    if((uniform_diffuse = shader.getUniform("diffuse"))==-1) return false;
    if((uniform_sun_dir = shader.getUniform("sun_dir"))==-1) return false;
    if((uniform_ambient = shader.getUniform("ambient"))==-1) return false;

    a_vertex = shader.getAttribute("vertex");

    shader.bind();
    Texture::active(Texture::TEXTURE0);
    shader.uniform(uniform_noise,0);
    shader.release();

    return true;
}

void OceanRenderForward::render(WorldGraphics *world, const Environment *envi, const Matrix4f &mvp, unsigned int elapsed)
{
    if(!world->ocean.isVisible()) return;

    if(world->ocean.isEnabled())
    {
        if(!world->ocean.vao.isCreated())
            setupRender(&world->ocean);

        shader.bind();
        world->ocean.normal_map.bind();
        float e=elapsed;
        e/=5000.0;
        shader.uniform(uniform_time,float(e));
        shader.uniformMatrix(uniform_mvp,mvp);
        shader.uniform(uniform_water_height,world->ocean.seaLevel());
        shader.uniform(u_size,world->ocean.size());
        shader.uniform(uniform_color,world->ocean.color());

        shader.uniform(uniform_diffuse, envi->sun.getDiffuse());
        shader.uniform(uniform_sun_dir, envi->sun.getDirection());
        shader.uniform(uniform_ambient, envi->ambient.getColor());

        world->ocean.vao.bind();
        world->ocean.buffer.draw(96);
        world->ocean.vao.release();

        shader.release();
    }
}

void OceanRenderForward::setupRender(OceanGraphics * ocean)
{
    ocean->vao.create();
    ocean->vao.bind();
    ocean->buffer.bind();
    shader.enableAttribute(a_vertex);
    shader.setAttribute(a_vertex, 3, GL_FLOAT);
    ocean->vao.release();
}
