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

#include "texturecube.h"

TextureCube::TextureCube(const GLint min_filter, const GLint wrap)
{
    init(min_filter,wrap);
}

TextureCube::TextureCube()
{
    texture=0;
}

TextureCube::~TextureCube()
{
    if(texture!=0)
        glDeleteTextures(1,&texture);
}

void TextureCube::init(const GLint min_filter, const GLint wrap)
{
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_CUBE_MAP);
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrap);
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrap);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, min_filter);

    if(min_filter!=GL_NEAREST && min_filter!=GL_LINEAR)
    {
#ifndef OPENGL_ES
        glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP, GL_TRUE);
       #endif
    }


    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

bool TextureCube::loadLeft(const char *file_name)
{
    return genTexture(file_name,GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
}

bool TextureCube::loadRight(const char *file_name)
{
    return genTexture(file_name,GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
}

bool TextureCube::loadFront(const char *file_name)
{
    return genTexture(file_name,GL_TEXTURE_CUBE_MAP_POSITIVE_X);
}

bool TextureCube::loadBack(const char *file_name)
{
    return genTexture(file_name,GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
}

bool TextureCube::loadTop(const char *file_name)
{
    return genTexture(file_name,GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
}

bool TextureCube::loadBottom(const char *file_name)
{
    return genTexture(file_name,GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
}

void TextureCube::bind()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
}

bool TextureCube::genTexture(const char *file_name,int type)
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    QImage t;
    QImage b;
    if(!b.load(file_name))
    {
        cerr<<"Unable to load image "<<file_name<<endl;
        return false;
    }
    t = OpenGL::convertToGLFormat( b );

    glTexImage2D( type, 0, GL_RGBA, t.width(),t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,t.bits());
    return true;
}
