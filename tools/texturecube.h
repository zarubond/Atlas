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
#ifndef TEXTURECUBE_H
#define TEXTURECUBE_H

#include "./opengl.h"

class TextureCube
{
public:
    TextureCube(const GLint min_filter,const GLint wrap);
    TextureCube();
    ~TextureCube();

    void init(const GLint min_filter=GL_LINEAR_MIPMAP_NEAREST,const GLint wrap=GL_CLAMP_TO_EDGE);
    bool loadLeft(const char * file_name);
    bool loadRight(const char * file_name);
    bool loadFront(const char * file_name);
    bool loadBack(const char * file_name);
    bool loadTop(const char * file_name);
    bool loadBottom(const char * file_name);

    void bind();
private:
    GLuint texture;
    bool genTexture(const char * file_name, int type);
};

#endif // TEXTURECUBE_H
