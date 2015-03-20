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

#ifndef FOGRENDER_H
#define FOGRENDER_H

#include "../graphics/graphics.h"
#include "../tools/camera.h"

#include "gbuffer.h"
#include "../environment/fog.h"
#include "../worldgraphics.h"

class FogRender: public OpenGL
{
public:
    FogRender();
    bool init();
    void render(const WorldGraphics * world, const Fog *fog, const Camera &camera, GBuffer *gbuffer);
private:
    Shader program;
    VertexBuffer canvas_buffer;
    Uniform uniform_color, uniform_tex_coord, uniform_density, uniform_cam_pos, uniform_height;
    Attribute attribute_coord;
    VertexArray vao;
};

#endif // FOGRENDER_H
