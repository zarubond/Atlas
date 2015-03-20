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
#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include "vertexbuffrer.h"
#include "opengl.h"

class IndexBuffer: public OpenGL
{
public:
    enum Mode{
        POINTS=GL_POINTS,
        LINES=GL_LINES,
        LINE_LOOP=GL_LINE_LOOP,
        LINE_STRIP=GL_LINE_STRIP,
        TRIANGLES=GL_TRIANGLES,
        TRIANGLE_STRIP=GL_TRIANGLE_STRIP,
        TRIANGLE_FAN=GL_TRIANGLE_FAN
    };

    enum Type{
        UNSIGNED_BYTE=GL_UNSIGNED_BYTE,
        UNSIGNED_SHORT=GL_UNSIGNED_SHORT
    };

    IndexBuffer();
    ~IndexBuffer();
    void draw(IndexBuffer::Mode mode, GLsizei count, IndexBuffer::Type type, GLint offset=0);
    void create(GLsizeiptr size, const GLvoid *data, GLenum usage=GL_STATIC_DRAW);
    void bind();
    void release();
    void destroy();
    bool valid() const;
    GLuint id() const;
private:
    GLuint buffer;
};

#endif // INDEXBUFFER_H
