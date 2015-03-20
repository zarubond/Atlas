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

#include "indexbuffer.h"

IndexBuffer::IndexBuffer()
{
    buffer=0;
}

IndexBuffer::~IndexBuffer()
{
    destroy();
}

void IndexBuffer::draw(IndexBuffer::Mode mode, GLsizei count, IndexBuffer::Type type, GLint offset)
{
    glDrawElements(mode, count, type, OPENGL_OFFSET(offset));
}

void IndexBuffer::create(GLsizeiptr size,const GLvoid * data,GLenum usage)
{
    destroy();
    glGenBuffers(1,&buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glFinish();
}

void IndexBuffer::bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
}

void IndexBuffer::release()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::destroy()
{
    if(buffer!=0)
    {
        glDeleteBuffers(1,&buffer);
        buffer=0;
    }
}

bool IndexBuffer::valid() const
{
    return buffer!=0;
}

GLuint IndexBuffer::id() const
{
    return buffer;
}

