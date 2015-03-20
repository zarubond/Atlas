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
#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include "opengl.h"

class VertexArray : public OpenGL
{
public:
    VertexArray();
    ~VertexArray();
    void bind() const;
    void release() const;
    void create();
    void destroy();
    GLuint id() const;
    bool isCreated() const;
    void enableAttribute(Attribute attrib);
    void disableAttribute(Attribute attrib);
    void setAttribute(Attribute index, GLint size, GLenum type, GLsizei stride=0, GLuint offset=0);
private:
    GLuint array;
};

#endif // VERTEXARRAY_H
