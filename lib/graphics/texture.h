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
#ifndef TEXTURE_H
#define TEXTURE_H

#include "opengl.h"
#include "opengldebug.h"
#include "../tools/image.h"
#include <string>
#include <cmath>

class Texture: public OpenGL
{
public:
    Texture();
    ~Texture();
    enum Active{
        TEXTURE0=GL_TEXTURE0,
        TEXTURE1=GL_TEXTURE1,
        TEXTURE2=GL_TEXTURE2,
        TEXTURE3=GL_TEXTURE3,
        TEXTURE4=GL_TEXTURE4
    };

    bool loadFile(const char * file_name, const GLint min_filter=GL_LINEAR_MIPMAP_NEAREST, const GLint wrap=GL_CLAMP_TO_EDGE);
    bool load(const Image * image, const GLint min_filter=GL_LINEAR_MIPMAP_NEAREST, const GLint wrap=GL_CLAMP_TO_EDGE);
    bool load(const unsigned char *data, int width, int height, const GLenum format = GL_RGBA, const GLint min_filter=GL_LINEAR_MIPMAP_NEAREST, const GLint wrap=GL_CLAMP_TO_EDGE);
    bool load(unsigned char r,unsigned char g,unsigned char b,const GLint min_filter=GL_LINEAR_MIPMAP_NEAREST,const GLint wrap=GL_CLAMP_TO_EDGE);
    GLuint id() const;
    void setId(GLuint id, GLenum format = GL_RGBA);
    void bind();
    void release();
    static void active(Active active);
    void clear();
private:
    GLuint texture;
    GLenum  format;
};


#endif // TEXTURE_H
