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
#ifndef TEXTUREATLAS_H
#define TEXTUREATLAS_H

#include <QImage>
#include "opengl.h"
#include "opengldebug.h"
#include <cmath>


/**
 * @brief The TextureArray class encapsulates GL_TEXTURE_2D_ARRAY. Unfortunately this feature is not supported under
 * OpenGL ES 2.0. As an emulation was used array of GL_TEXTURE_2D. In your shader you have to use samething like sampler2D textures[layer_count]
 */
class TextureArray: public OpenGL
{
public:
    enum Active{
        TEXTURE0=GL_TEXTURE0,
        TEXTURE1=GL_TEXTURE1,
        TEXTURE2=GL_TEXTURE2,
        TEXTURE3=GL_TEXTURE3,
        TEXTURE4=GL_TEXTURE4,
        TEXTURE5=GL_TEXTURE5,
        TEXTURE6=GL_TEXTURE6,
        TEXTURE7=GL_TEXTURE7
    };
    TextureArray();
    ~TextureArray();
    bool create(unsigned short width, unsigned short height, unsigned short layer_count,
              const GLint min_filter=GL_LINEAR_MIPMAP_NEAREST, const GLint wrap=GL_CLAMP_TO_EDGE);
    bool loadSub(const char * file_name, int id);
    /**
     * @brief id This feature is supported only if OpenGL supports GL_TEXTURE_2D_ARRAY.
     * @return OpenGL id od the texture.
     */
    GLuint id() const;
    void bind();
    static void active(Active texture);
    void clear();
    void release();
    /**
     * @brief layerCount
     * @return NUmber of textures in array.
     */
    int layerCount() const;
    /**
     * @brief generateMipmap Create texture mipmaps.
     */
    void generateMipmap();
private:
    enum Type{
        ARRAY=0,
        ATLAS,
        STACK
    };
    // there is no support for this in android :(
    double log2( double n )
    {
        // log(n)/log(2) is log2.
        return log( n ) / log( 2 );
    }
    int width;
    int height;
    int depth;
    int cells;
    int layer_count;
    GLuint texture;
    GLuint * textures;//for stack of textures
    GLenum  format;
    Type type;
    GLint min_filter;
    GLint wrap;
};

#endif // TEXTUREATLAS_H
