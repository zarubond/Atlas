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

#include "vertexarray.h"

VertexArray::VertexArray()
{
    array=0;
}

VertexArray::~VertexArray()
{
    destroy();
}

void VertexArray::bind() const
{
    glBindVertexArray(array);
}

void VertexArray::release() const
{
    glBindVertexArray(0);
}

void VertexArray::create()
{
    glGenVertexArrays(1, &array);
}

bool VertexArray::isCreated() const
{
    return array!=0;
}

void VertexArray::destroy()
{
    if(array!=0)
    {
        glDeleteVertexArrays(1, &array);
        array=0;
    }
}

void VertexArray::enableAttribute(Attribute attrib)
{
    glEnableVertexAttribArray(attrib);
}

void VertexArray::disableAttribute(Attribute attrib)
{
    glDisableVertexAttribArray(attrib);
}

void VertexArray::setAttribute(Attribute index, GLint size, GLenum type, GLsizei stride, GLuint offset)
{
    glVertexAttribPointer(index, size, type, GL_FALSE, stride,OPENGL_OFFSET(offset));
}

GLuint VertexArray::id() const
{
    return array;
}

