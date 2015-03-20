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

#ifndef LIGHTAMBIENTRENDER_H
#define LIGHTAMBIENTRENDER_H

#include "../graphics/graphics.h"
#include "../environment/lightambient.h"
#include "gbuffer.h"

class LightAmbientRender: public OpenGL
{
public:
    LightAmbientRender();
    bool init();
    void render(LightAmbient * light, GBuffer * gbuffer);
private:
    Shader program;
    VertexBuffer canvas_buffer;
    Uniform uniform_color, uniform_tex_color;
    Attribute attribute_coord;
    VertexArray vao;
};

#endif // LIGHTAMBIENTRENDER_H
