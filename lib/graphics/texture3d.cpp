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

#include "texture3d.h"

Texture3D::Texture3D()
{
    texture=0;
}

Texture3D::~Texture3D()
{
    glDeleteTextures(1,&texture);
}

bool Texture3D::load(const char *file_name, const GLint min_filter, const GLint wrap)
{
    QImage t;
    QImage b;
    if(!b.load(file_name))
    {
        cerr<<"Unable to load image "<<file_name<<endl;
        return false;
    }

    t = OpenGL::convertToGLFormat(b);
    glGenTextures( 1, &texture );
#ifndef OPENGL_ES
    glBindTexture( GL_TEXTURE_3D, texture );
#endif
/*
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,   min_filter);

    if(min_filter!=GL_NEAREST && min_filter!=GL_LINEAR)
    {
        //glTexParameteri (GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    }

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
*/
#ifdef OPENGL_ES
   // glTexImage3D( GL_TEXTURE_2D, 0, GL_RGBA, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits() );

    if(min_filter!=GL_NEAREST && min_filter!=GL_LINEAR)
    {
        //glGenerateMipmap(GL_TEXTURE_3D);
    }

#else
    if(min_filter!=GL_NEAREST && min_filter!=GL_LINEAR)
    {
        glTexParameteri (GL_TEXTURE_3D, GL_GENERATE_MIPMAP, GL_TRUE);
    }

    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S,wrap);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T,wrap);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R,wrap);

    glTexImage3D( GL_TEXTURE_3D, 0, GL_RGBA, t.width(), t.height(),1, 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits() );
#endif
/*
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,wrap);

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, t.width(),t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,t.bits());
*/
    format=GL_RGBA;

    return true;
}

bool Texture3D::load(unsigned char r, unsigned char g, unsigned char b, const GLint min_filter, const GLint wrap)
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

bool Texture3D::load(unsigned char *data, int width, int height, int depth,
                     const GLint min_filter, const GLint wrap)
{

    glGenTextures( 1, &texture );
    //glBindTexture( GL_TEXTURE_3D, texture );

#ifdef OPENGL_ES
    //glTexImage3D( GL_TEXTURE_2D, 0, GL_RGBA, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits() );

    if(min_filter!=GL_NEAREST && min_filter!=GL_LINEAR)
    {
      //  glGenerateMipmap(GL_TEXTURE_3D);
    }

#else

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S,wrap);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T,wrap);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R,wrap);
    //min_filter=GL_LINEAR;

    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER,   GL_NEAREST);

    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    /*
    if(min_filter!=GL_NEAREST && min_filter!=GL_LINEAR)
    {
        glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_MAX_LEVEL,5);
       glTexParameteri (GL_TEXTURE_3D, GL_GENERATE_MIPMAP, GL_TRUE);

    }*/

    //glTexImage3D( GL_TEXTURE_3D, 0, GL_RGBA, width, height,depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );


/*
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
*/


    //m(GL_TEXTURE_3D,GL_TEXTURE_MAX_LEVEL,10);

    //glTexImage3D( GL_TEXTURE_3D, 0, GL_RGBA, width, height,depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
    //gluBuild3DMipmaps( GL_TEXTURE_3D, GL_RGBA, width, height,depth,
      //  GL_RGBA, GL_UNSIGNED_BYTE, data );

    /*
    glTexImage3D( GL_TEXTURE_3D, 1, GL_RGBA, width/2, height/2,depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
    glTexImage3D( GL_TEXTURE_3D, 2, GL_RGBA, width/4, height/4,depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
    glTexImage3D( GL_TEXTURE_3D, 3, GL_RGBA, width/8, height/8,depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
    glTexImage3D( GL_TEXTURE_3D, 4, GL_RGBA, width/16, height/16,depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
    glTexImage3D( GL_TEXTURE_3D, 5, GL_RGBA, width/32, height/32,depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
    glTexImage3D( GL_TEXTURE_3D, 6, GL_RGBA, width/64, height/64,depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
    glTexImage3D( GL_TEXTURE_3D, 7, GL_RGBA, width/128, height/128,depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
    glTexImage3D( GL_TEXTURE_3D, 8, GL_RGBA, width/256, height/256,depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );
    glTexImage3D( GL_TEXTURE_3D, 9, GL_RGBA, width/512, height/512,depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );

    /*int w=width;
    int h=height;
    for(int c=1;c<5;c++)
    {
        /*
        for(int i=0,j=0;i<w*h;i+=2,j++)
        {
            data[j]  =data[i];
            data[j+1]=data[i+1];
            data[j+2]=data[i+2];
            data[j+3]=data[i+3];
        }
        w/=2;
        h/=2;*

        //glTexImage3D( GL_TEXTURE_3D, c, GL_RGBA, w, h,depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );

    }*/

#endif

    format=GL_RGBA;

    return true;
}

GLuint Texture3D::id()
{
    return texture;
}

void Texture3D::setId(GLuint id,GLenum format)
{
    this->texture = id;
    this->format = format;
}

void Texture3D::bind()
{
#ifndef OPENGL_ES
    glBindTexture( GL_TEXTURE_3D, texture );
#endif
}

void Texture3D::active(GLenum texture)
{
    glActiveTexture(texture);
}

void Texture3D::clear()
{
    glDeleteTextures(1,&texture);
}
