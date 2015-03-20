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

#ifndef LIGHTDIRECTIONALRENDER_H
#define LIGHTDIRECTIONALRENDER_H

#include "../graphics/graphics.h"
#include "../environment/lightdirectional.h"
#include "gbuffer.h"
#include "shadowbuffer.h"

class LightDirectionalRender: public OpenGL
{
public:
    LightDirectionalRender();
    bool init();
    void render(const LightDirectional *light, const Camera &camera, GBuffer *buffer);
    void render(const LightDirectional *light, const Camera &camera, GBuffer *buffer, ShadowBuffer * shadow, const Matrix4f &shadowMVP);
private:
    Shader directional;
    Uniform uniform_direction, uniform_diffuse, uniform_specular, uniform_shininess;
    Uniform uniform_tex_normal, uniform_tex_position, uniform_tex_color, uniform_tex_shadow, uniform_camera;
    Uniform uniform_shadowenable, uniform_shadowmvp;
    Attribute attribute_coord;
    VertexBuffer canvas_buffer;
    Matrix4f biasMatrix;
    VertexArray vao;
};

#endif // LIGHTDIRECTIONALRENDER_H
