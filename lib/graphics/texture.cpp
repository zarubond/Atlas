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

#include "texture.h"

Texture::Texture()
{
    texture=0;
}

Texture::~Texture()
{
    clear();
}

bool Texture::loadFile(const char *file_name, const GLint min_filter, const GLint wrap)
{
    Image b;
    if(!b.load(file_name))
    {
        cerr<<"Unable to load image "<<file_name<<endl;
        return false;
    }

    Image t=b.convertToGLFormat();
    return load(t.bits(), t.width(), t.height(), GL_RGBA, min_filter, wrap);
}

bool Texture::load(const Image *image, const GLint min_filter, const GLint wrap)
{
    Image t=image->convertToGLFormat();
    return load(t.bits(), t.width(), t.height(), GL_RGBA, min_filter, wrap);
}

bool Texture::load(const unsigned char *data, int width, int height, const GLenum format, const GLint min_filter, const GLint wrap)
{
    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );

    if(min_filter!=GL_NEAREST && min_filter!=GL_LINEAR)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    this->format=format;

    glBindTexture(GL_TEXTURE_2D, 0);
    glFinish();

    return true;
}

bool Texture::load(unsigned char r, unsigned char g, unsigned char b, const GLint min_filter, const GLint wrap)
{
    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,   min_filter);
    if(min_filter!=GL_NEAREST && min_filter!=GL_LINEAR)
    {
        //glTexParameteri (GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    }
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,wrap);
    const unsigned char tex[]={r,g,b,255};

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, 1,1, 0, GL_RGBA, GL_UNSIGNED_BYTE,tex);
    glBindTexture( GL_TEXTURE_2D, 0);
    return true;
}

GLuint Texture::id() const
{
    return texture;
}

void Texture::setId(GLuint id,GLenum format)
{
    this->texture = id;
    this->format = format;
}

void Texture::bind()
{
    glBindTexture( GL_TEXTURE_2D, texture );
}

void Texture::release()
{
    glBindTexture( GL_TEXTURE_2D, 0 );
}

void Texture::active(Active active)
{
    glActiveTexture(active);
}

void Texture::clear()
{
    glDeleteTextures(1,&texture);
}

