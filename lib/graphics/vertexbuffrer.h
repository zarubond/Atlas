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

#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "opengl.h"
#include "opengldebug.h"

class VertexBuffer: public OpenGL
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
    VertexBuffer();
    ~VertexBuffer();
    void bind();
    void release();
    void draw(GLsizei count, VertexBuffer::Mode mode=TRIANGLES, GLint first=0);
    void destroy();
    void create(GLsizeiptr size, const GLvoid *data, GLenum usage=GL_STATIC_DRAW);
    bool isCreated()const;
    int size();

    GLuint id() const;
private:
    GLuint buffer;
    int m_size;
};

#endif // VERTEXBUFFER_H
