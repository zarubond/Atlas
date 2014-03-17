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
#ifndef TEXTURE3D_H
#define TEXTURE3D_H

#include "opengl.h"


class Texture3D
{
public:
    Texture3D();
    ~Texture3D();
    bool load(const char * file_name,const GLint min_filter=GL_LINEAR_MIPMAP_NEAREST,const GLint wrap=GL_CLAMP_TO_EDGE);
    bool load(unsigned char r,unsigned char g,unsigned char b,const GLint min_filter=GL_LINEAR_MIPMAP_NEAREST,const GLint wrap=GL_CLAMP_TO_EDGE);
    bool load(unsigned char *data, int width, int height, int depth,
              const GLint min_filter=GL_LINEAR_MIPMAP_NEAREST, const GLint wrap=GL_CLAMP_TO_EDGE);
    GLuint id();
    void setId(GLuint id, GLenum format = GL_RGBA);
    void bind();
    static void active(GLenum texture);
    void clear();
private:
    GLuint texture;
    GLenum  format;
};

#endif // TEXTURE3D_H
