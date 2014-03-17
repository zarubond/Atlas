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
#ifndef TEXTUREATLAS_H
#define TEXTUREATLAS_H

#include "opengl.h"
#include <cmath>

class TextureArray
{
public:
    TextureArray();
    ~TextureArray();
    bool load(unsigned short width,unsigned short height,unsigned short layer_count,
              const GLint min_filter=GL_LINEAR_MIPMAP_NEAREST,const GLint wrap=GL_CLAMP_TO_EDGE);
    bool loadSub(const char * file_name, int id);
    GLuint id();
    void bind();
    static void active(GLenum texture);
    void clear();
    void unbind();
private:
    int width;
    int height;
    int layer_count;
    GLuint texture;
    GLenum  format;
};

#endif // TEXTUREATLAS_H
