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
#ifndef GRASSRENDER_H
#define GRASSRENDER_H

#include "../../../tools/graphics.h"
#include "../partdrawer.h"

#include "grassgraphics.h"

/**
 * @brief The GrassRender class test class to be extended in future.
 */
class GrassRender: public ModuleDrawer
{
public:
    GrassRender();

    bool init();
    void load(GrassGraphics *base);//temporrary hack

    void render(const Matrix4f &mvp, GLuint elapsed);
    void renderShadow(const Matrix4f &mvp,unsigned int elapsed);

    void clearRenderList();

    void setTexture(const char * file_name);
private:
    Shader shader;
    GLint uniform_mvp,uniform_pos,uniform_scale,uniform_texture,uniform_texcoord,
            uniform_time;
    GLint attribute_coord3d,attribute_texcoord,attribute_normal;
    GLuint vbo_vertices,ibo_indices;
    IndexBuffer indices;
    VertexBuffer vertices;

    int triangle_num;

    GrassGraphics * base;
};

#endif // GRASSRENDER_H
