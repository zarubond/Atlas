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

#ifndef WATER_H
#define WATER_H

#include "../../graphics/graphics.h"
#include "../modulebase.h"

/**
 * @brief The Water class test class to be extended in future.
 */
class Water: public ModuleBase, OpenGL
{
public:
    Water(int type);
    bool init();

    void render(const Matrix4f &mvp, GLfloat time);

private:
    float water_height;

    Shader shader;
    Texture normal_map;
    Uniform uniform_mvp,uniform_time,uniform_map,uniform_water_height;
    Attribute attribute_vertex;
    GLuint vbo_vertices;
};

#endif // WATER_H
