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
#ifndef SKYBOX_H
#define SKYBOX_H

#include "../../tools/graphics.h"

/**
 * @file SkyBox.hpp
 * @brief Map skybox.
 * @author Ondrej Zaruba
 */

class SkyBox
{
public:
    SkyBox();
    ~SkyBox();
    /**
     * @brief Draw skybox.
     * @param mvp Draw matrix.
     */
    void draw(const Matrix4f &mvp);
    /**
     * @brief Load skybox.
     * @param scale SkyBox size.
     */
    bool load(GLfloat scale);
    /**
     * @brief Load skybox.
     * @param top Top texture.
     * @param bottom Bottom texture.
     * @param front Front texture.
     * @param back Back texture.
     * @param right Right texture.
     * @param left Left texture.
     */
    bool load(const char * top,const char * bottom,
              const char * front,const char * back,
              const char * right, const char * left);
private:
    bool loadShader();
    float x,y,z;
    GLuint vbo_cube_vertices,ibo_cube_elements;

    TextureCube texture;
    Shader shader;

    GLint attribute_coord3d, translate;
    GLint uniform_mvp, uniform_texture;
};

#endif // SKYBOX_H
