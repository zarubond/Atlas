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

#include "fogrender.h"

FogRender::FogRender()
{
}

bool FogRender::init()
{
    const char * ambient_vsh=":lib/shader/Fog.vsh";
    const char * ambient_fsh=":lib/shader/Fog.fsh";

    if(!program.loadFile(ambient_vsh, ambient_fsh))
        return false;

    if((uniform_color = program.getUniform("color"))==-1)
       return false;

    if((uniform_cam_pos = program.getUniform("cam_pos"))==-1)
        return false;

    if((uniform_density = program.getUniform("density"))==-1)
        return false;

    if((attribute_coord = program.getAttribute("coord"))==-1)
        return false;

    if((uniform_tex_coord = program.getUniform("tex_coord"))==-1)
        return false;

    if((uniform_height = program.getUniform("height"))==-1)
        return false;

    program.bind();
    program.uniform(uniform_tex_coord,0);
    program.release();

    static const GLfloat quad_data[] = {
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
    };

    canvas_buffer.create(sizeof(quad_data),quad_data);

    vao.create();
    vao.bind();
    program.bind();
    Texture::active(Texture::TEXTURE0);
    program.enableAttribute(attribute_coord);
    canvas_buffer.bind();
    program.setAttribute(attribute_coord, 3, GL_FLOAT);
    vao.release();

    return true;
}

void FogRender::render(const WorldGraphics *world, const Fog *fog, const Camera &camera, GBuffer *gbuffer)
{
    if(world->environment.fog.isEnabled())
    {
        program.bind();
        gbuffer->bindTexturePosition();
        program.uniform(uniform_color,fog->getColor());
        program.uniform(uniform_density, fog->getDensity()*0.01f);
        program.uniform(uniform_cam_pos, camera.position());
        program.uniform(uniform_height, float(world->map.height()));
        vao.bind();
        canvas_buffer.draw(6);
        vao.release();
        program.release();
    }
}
