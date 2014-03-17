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
#include "texture.h"

Texture::Texture()
{
    texture=0;
}

Texture::~Texture()
{
    glDeleteTextures(1,&texture);
}

bool Texture::load(const char *file_name, const GLint min_filter, const GLint wrap)
{
#ifdef QT_VERSION
    QImage t;
    QImage b;
    if(!b.load(file_name))
    {
        std::cerr<<"Unable to load image "<<file_name<<std::endl;
        return false;
    }

    t = OpenGL::convertToGLFormat(b);
    //QImage gl_compatible_image = b.rgbSwapped ().mirrored();
    //t = gl_compatible_image.convertToFormat(QImage::Format_ARGB32);
#else
    #error Please implement image importing!
#endif

    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
#ifdef OPENGL_ES
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits() );

    if(min_filter!=GL_NEAREST && min_filter!=GL_LINEAR)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

#else
    if(min_filter!=GL_NEAREST && min_filter!=GL_LINEAR)
    {
        glTexParameteri (GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    }

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,wrap);

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits() );
#endif
/*
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,wrap);

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, t.width(),t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,t.bits());
*/
    format=GL_RGBA;

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
    return true;
}

GLuint Texture::id()
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

void Texture::active(GLenum texture)
{
    glActiveTexture(texture);
}

void Texture::clear()
{
    glDeleteTextures(1,&texture);
}

