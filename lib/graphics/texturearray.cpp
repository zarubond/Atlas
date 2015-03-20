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

#include "texturearray.h"

TextureArray::TextureArray()
{
    texture=0;
    width=0;
    height=0;
    depth=0;
    layer_count=0;
    cells=0;
    if(isTextureArray())
        type=ARRAY;
    else
        type=STACK;
    textures=NULL;
}

TextureArray::~TextureArray()
{
    clear();
    delete [] textures;
}

//http://developer.download.nvidia.com/opengl/specs/GL_EXT_texture_array.txt
bool TextureArray::create(unsigned short width, unsigned short height, unsigned short layer_count,
                        const GLint min_filter, const GLint wrap)
{
    this->width=width;
    this->height=height;
    this->layer_count=layer_count;
    this->min_filter=min_filter;
    this->wrap=wrap;
    format=GL_RGBA;

    if(min_filter!=GL_NEAREST && min_filter!=GL_LINEAR)
    {
        depth=log2(width);
    }
    else
        depth=1;


    glGenTextures(1, &texture);

    if(type==ARRAY)
    {
        glBindTexture(GL_TEXTURE_2D_ARRAY,texture);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, wrap);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, wrap);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, min_filter);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, width, height, depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

        glBindTexture(GL_TEXTURE_2D_ARRAY,0);
    }
    else if(type==ATLAS)
    {
        cells=4;//ceil(sqrt(layer_count)); HACK
        glBindTexture(GL_TEXTURE_2D, texture);

        unsigned char * checkImage=new unsigned char[cells*cells*width*height*4];
        memset(checkImage,255,cells*cells*width*height);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cells*width, cells*height, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);

        if(min_filter!=GL_NEAREST && min_filter!=GL_LINEAR)
            glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFinish();
        glBindTexture(GL_TEXTURE_2D, 0);
        delete [] checkImage;
    }
    else if(type==STACK)
    {
        textures=new GLuint[layer_count];
        for(int i=0;i<layer_count;i++)
            textures[i]=0;
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
        cerr<<"Unable to load image "<<file_name<<endl;
        return false;
    }

    t = OpenGL::convertToGLFormat(b).scaled(width,width);
    if(type==ARRAY)
    {
        glBindTexture(GL_TEXTURE_2D_ARRAY,texture);
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, id, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, t.bits());
        glBindTexture(GL_TEXTURE_2D_ARRAY,0);
    }
    else if(type==ATLAS)
    {
        glBindTexture(GL_TEXTURE_2D, texture);
        int w=width;
        for(int i=0;i<depth;i++)
        {
            b=t.scaled(w, w,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
            glTexSubImage2D(GL_TEXTURE_2D, i, w*int(id/cells), w*int(id%cells), w, w, GL_RGBA, GL_UNSIGNED_BYTE, b.bits());
            w/=2;
        }
        glBindTexture(GL_TEXTURE_2D,0);
    }
    else if(type==STACK)
    {
        glGenTextures( 1, &textures[id] );
        glBindTexture( GL_TEXTURE_2D, textures[id] );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits() );

        if(min_filter!=GL_NEAREST && min_filter!=GL_LINEAR)
        {
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        glFinish();
    }
    glFinish();
    return true;
}

GLuint TextureArray::id() const
{
    return texture;
}

void TextureArray::bind()
{
    if(type==STACK)
    {
        for(int i=0;i<layer_count;i++)
        {
            active(Active(TEXTURE0+i));
            glBindTexture(GL_TEXTURE_2D, textures[i]);
        }
    }
    else
        glBindTexture(type==ARRAY?GL_TEXTURE_2D_ARRAY:GL_TEXTURE_2D, texture);
}

void TextureArray::release()
{
    glBindTexture(type==ARRAY?GL_TEXTURE_2D_ARRAY:GL_TEXTURE_2D, 0);
}

int TextureArray::layerCount() const
{
    return layer_count;
}

void TextureArray::generateMipmap()
{
    if(depth>1 && type==ARRAY)
    {
        glBindTexture(GL_TEXTURE_2D_ARRAY,texture);
        glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
        glBindTexture(GL_TEXTURE_2D_ARRAY,0);
        glFinish();
    }
}

void TextureArray::active(Active texture)
{
    glActiveTexture(texture);
}

void TextureArray::clear()
{
    if(type==ARRAY)
    {
        glDeleteTextures(1,&texture);
    }
    else
    {
        if(textures!=NULL)
            glDeleteTextures(layer_count,textures);
    }
}
