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
#ifndef WATER_H
#define WATER_H

#include "../../../tools/opengl.h"
#include "../../../tools/shader.h"
#include "../../../tools/texture.h"
#include "../partmanager.h"

/**
 * @brief The Water class test class to be extended in future.
 */
class Water: public ModuleBase
{
public:
    Water(int type);
    bool init(const char * vert_file,const char * frag_file);

    void render(const Matrix4f &mvp, GLfloat time);

private:
    Shader shader;
    Texture normal_map;
    GLint uniform_mvp,uniform_time,uniform_map;
    GLint attribute_vertex;
    GLuint vbo_vertices;
};

#endif // WATER_H
