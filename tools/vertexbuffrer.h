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
#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "opengl.h"
struct Attribute
{
    GLuint index;
    GLint size;
    GLenum type;
    GLsizei stride;
    GLsizei offset;
};

class VertexBuffer
{
public:
    VertexBuffer();
    ~VertexBuffer();
    void bind() const;
    void unbind() const;
    void pushAttribute();
    void draw(GLsizei count, GLenum mode=GL_TRIANGLES) const;
    void clear();
    void create(GLsizeiptr size, const GLvoid *data, GLenum usage=GL_STATIC_DRAW);
    void attribute(GLuint index, GLint size, GLenum type,GLsizei stride,GLsizei offset=0) const;
    void attribute(Attribute * atrib);
private:
    GLuint buffer;
};

#endif // VERTEXBUFFER_H
