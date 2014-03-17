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
#include "vertexbuffrer.h"

VertexBuffer::VertexBuffer()
{
    buffer=0;
}

VertexBuffer::~VertexBuffer()
{
    clear();
}

void VertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
}

void VertexBuffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::create(GLsizeiptr size, const GLvoid *data, GLenum usage)
{
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::attribute(GLuint index, GLint size, GLenum type,GLsizei stride,GLsizei offset) const
{
    glVertexAttribPointer(index, size, type, GL_FALSE, stride,OPENGL_OFFSET(offset));
}

void VertexBuffer::attribute(Attribute *atrib)
{
    glVertexAttribPointer(atrib->index, atrib->size, atrib->type,
                          GL_FALSE, atrib->stride,OPENGL_OFFSET(atrib->offset));
}

void VertexBuffer::clear()
{
    glDeleteBuffers(1,&buffer);
}


void VertexBuffer::draw(GLsizei count,GLenum mode) const
{
    glDrawArrays(mode, 0, count);
}
