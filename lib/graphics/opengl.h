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
#ifndef OPENGL_H
#define OPENGL_H

#include "../tools/envi.h"
#include <QImage>
#include <QOpenGLContext>
#include <string>

#include "../tools/shell.h"
#include "openglfunctions.h"
#include "opengldefine.h"


#define OPENGL_OFFSET(i) ((char *)NULL + (i))

#ifndef OS_WINDOWS
#define __stdcall__
#endif

typedef GLint Uniform;
typedef GLint Attribute;

class OpenGL
{
public:
    enum OpenGLVersion{
            OPENGL_3_0=0,
            OPENGL_ES_2_0,
            OPENGL_ES_3_0,
            OPENGL_UNKNOWN
    };

    OpenGL();

    /**
     * @brief init Initialize OpenGL functions and extensions. Must be called from rendering thread with context!
     * @return  True on success.
     */
    static bool glInit(QOpenGLContext *context, OpenGLVersion preferred=OPENGL_UNKNOWN);
    /**
     * @brief vSync Display vertical synchronization.
     * @param sync true-on false-off
     */
    static void vSync(bool sync);

    static OpenGLVersion glVersion();

protected:
    static bool isTextureArray();
    static bool isDepth24();
    /**
     * @brief Error Print OpenGL error.
     * @return Erro number.
     */
    static int  glError();
    static void glErrorPrint(const char * file, int line, const char *function, const char * params);
    static int glMajorVersion();
    static int glMinorVersion();
    static QImage convertToGLFormat(const QImage &img);
    static bool isLoaded();

#define OPENGL_FUNCTION(Return,Name, ...) \
    static Return (__attribute__((__stdcall__))* Name)(__VA_ARGS__);
    GL_FUNCTION_LIST_ES_20
    GL_FUNCTION_LIST_ES_30
    GL_FUNCTION_LIST_ES_MISSING
#undef OPENGL_FUNCTION

private:
    static void convertToGLFormatHelper(QImage &dst, const QImage &img, GLenum texture_format);
    static void debugCallback(GLuint source, GLuint type, GLuint id, GLuint severity, GLint length, const char* message, void* userParam);

#if defined(OS_LINUX)
    static void (*swapInterval)(int);
#elif defined(OS_WINDOWS)
    static bool (*swapInterval)(int);
#else
    static void * swapInterval;
#endif
    static bool is_loaded;
    static void loadExtension();

    static bool ext_texture_array;
    static bool ext_depth24;
    static OpenGLVersion opengl_version;
    static QFunctionPointer getProcAddress(const char * name, QOpenGLContext *context);
};

#endif // OPENGL_H
