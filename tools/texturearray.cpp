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
#include "texturearray.h"

TextureArray::TextureArray()
{
    texture=0;
    width=0;
    height=0;
    layer_count=0;
}

TextureArray::~TextureArray()
{
    glDeleteTextures(1,&texture);
}

//http://developer.download.nvidia.com/opengl/specs/GL_EXT_texture_array.txt
bool TextureArray::load(unsigned short width, unsigned short height, unsigned short layer_count,
                        const GLint min_filter, const GLint wrap)
{
    this->width=width;
    this->height=height;
    this->layer_count=layer_count;
    glGenTextures(1, &texture);

    if(OpenGL::EXTTextureArray())
    {
#ifndef OPENGL_ES
        glBindTexture(GL_TEXTURE_2D_ARRAY,texture);

        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, min_filter);
        if(min_filter!=GL_NEAREST && min_filter!=GL_LINEAR)
        {
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_GENERATE_MIPMAP, GL_TRUE);
        }
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, wrap);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, wrap);

        glTexStorage3D(GL_TEXTURE_2D_ARRAY, log2(width), GL_RGBA8, width, height, layer_count);
#endif
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
        if(min_filter!=GL_NEAREST && min_filter!=GL_LINEAR)
        {
#ifndef OPENGL_ES
            glTexParameteri (GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
#endif
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 8);

        float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
        glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color );

        unsigned char * checkImage=new unsigned char[width*height*4];
        for(int i=0;i<width*height;i++)
        {
            checkImage[i]=255;
            checkImage[i++]=255;
            checkImage[i++]=255;
            checkImage[i++]=255;
        }

        for(int i=0;i<9;i++)
        {
            glTexImage2D(GL_TEXTURE_2D, i, GL_RGBA, width, height,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
            width/=2;
            height/=2;
        }

        delete [] checkImage;
        format=GL_RGBA;
    }
    return true;
}

bool TextureArray::loadSub(const char *file_name, int id)
{
    if(id>=layer_count || id<0)
        return false;

    QImage t;
    QImage b;
    if(!b.load(file_name))
    {
        std::cerr<<"Unable to load image "<<file_name<<std::endl;
        return false;
    }

    t = OpenGL::convertToGLFormat(b).scaled(width,height);
    if(OpenGL::EXTTextureArray())
    {
        glBindTexture(GL_TEXTURE_2D_ARRAY,texture);
#ifndef OPENGL_ES
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, id, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, t.bits());
#endif
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, texture);
        int w=512;
        for(int i=0;i<9;i++)
        {
            b=t.scaled(w, w,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

            glTexSubImage2D(GL_TEXTURE_2D, i, w*int(id/4), w*int(id%4), w, w, GL_RGBA,
                            GL_UNSIGNED_BYTE, b.bits());
            w/=2;
        }
    }

    return true;
}

GLuint TextureArray::id()
{
    return texture;
}

void TextureArray::bind()
{
    if(OpenGL::EXTTextureArray())
        glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
    else
        glBindTexture(GL_TEXTURE_2D, texture );
}

void TextureArray::unbind()
{
    if(OpenGL::EXTTextureArray())
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    else
        glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureArray::active(GLenum texture)
{
    glActiveTexture(texture);
}

void TextureArray::clear()
{
    glDeleteTextures(1,&texture);
}
