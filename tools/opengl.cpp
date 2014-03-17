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
#include "opengl.h"

#ifndef OPENGL_ES

#define OPENGL_FUNTION(PFNGL,NAME) \
    PFNGL NAME = 0;
OPENGL_FUNTION_LIST
#undef OPENGL_FUNTION

#ifdef Q_OS_WIN
    #define OPENGL_FUNTION(PFNGL,NAME) \
        PFNGL NAME = 0;
    OPENGL_FUNTION_LIST_WIN
    #undef OPENGL_FUNTION
#endif

#if OPENGL_VERSION == 3
    #define OPENGL_FUNTION(PFNGL,NAME) \
        PFNGL NAME = 0;
    OPENGL_FUNTION_LIST_3
    #undef OPENGL_FUNTION
#endif

#else
#define OPENGL_FUNTION(PFNGL,NAME) \
    PFNGL NAME = 0;
    OPENGL_FUNTION_LIST_ES
#undef OPENGL_FUNTION
#endif

OpenGL::OpenGL(const QOpenGLContext *context):context(context)
{
    switch(QGLFormat::openGLVersionFlags())
    {
        case QGLFormat::OpenGL_Version_2_0:
        case QGLFormat::OpenGL_Version_2_1:
            opengl_version=OPENGL_2_0;
            break;
        case QGLFormat::OpenGL_ES_Version_2_0:
            opengl_version=OPENGL_ES_2_0;
            break;
        default:
            opengl_version=OPENGL_UNKNOWN;
            break;
    }
}

bool OpenGL::init()
{
#ifndef OPENGL_ES

#define OPENGL_FUNTION(PFNGL,NAME) \
    NAME = (PFNGL) getProcAddress(STR_VALUE(NAME));
OPENGL_FUNTION_LIST
#undef OPENGL_FUNTION


#ifdef Q_OS_WIN
    #define OPENGL_FUNTION(PFNGL,NAME) \
        NAME = (PFNGL) getProcAddress(STR_VALUE(NAME));
    OPENGL_FUNTION_LIST_WIN
    #undef OPENGL_FUNTION
#endif

#if OPENGL_VERSION == 3
    #define OPENGL_FUNTION(PFNGL,NAME) \
        NAME = (PFNGL) getProcAddress(STR_VALUE(NAME));
    OPENGL_FUNTION_LIST_3
    #undef OPENGL_FUNTION
#endif

#ifdef Q_OS_LINUX
    swapInterval = (void (*)(int)) context->getProcAddress("glXSwapInterval");

    if(swapInterval==NULL)
        swapInterval = (void (*)(int)) context->getProcAddress("glXSwapIntervalMESA");

    if(swapInterval==NULL)
        swapInterval = (void (*)(int)) context->getProcAddress("glXSwapIntervalSGI");
#endif

#ifdef Q_OS_WIN
    swapInterval = (PFNWGLSWAPINTERVALFARPROC) context->getProcAddress("wglSwapIntervalEXT");
    if(swapInterval==NULL)
        swapInterval = (PFNWGLSWAPINTERVALFARPROC) context->getProcAddress("wglSwapInterval");

#endif
#else

#define OPENGL_FUNTION(PFNGL,NAME) \
    NAME = (PFNGL) getProcAddress(STR_VALUE(NAME));
    OPENGL_FUNTION_LIST_ES
#undef OPENGL_FUNTION

#endif

    loadExtension();

    return true;
}

void OpenGL::vSync(bool sync)
{
#ifndef OPENGL_ES
    if(swapInterval!=NULL)
    {
        if(sync)
            swapInterval(1);
        else
            swapInterval(0);
    }
#endif
}

OpenGL::OpenGLVersion OpenGL::version()
{
    return opengl_version;
}

bool OpenGL::EXTTextureArray()
{
    return ext_texture_array;
}

int OpenGL::Error()
{
    int err;
    while((err=glGetError())!=GL_NO_ERROR)
    {
        switch(err)
        {
        case GL_NO_ERROR:
            std::cerr<<"GL_NO_ERROR"<<std::endl;break;
        case GL_INVALID_ENUM:
            std::cerr<<"GL_INVALID_ENUM"<<std::endl;break;
        case GL_INVALID_VALUE:
            std::cerr<<"GL_INVALID_VALUE"<<std::endl;break;
        case GL_INVALID_OPERATION:
            std::cerr<<"GL_INVALID_OPERATION"<<std::endl;break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            std::cerr<<"GL_INVALID_FRAMEBUFFER_OPERATION"<<std::endl;break;
        case GL_OUT_OF_MEMORY:
            std::cerr<<"GL_OUT_OF_MEMORY"<<std::endl;break;
#ifndef OPENGL_ES
        case GL_STACK_UNDERFLOW:
            std::cerr<<"GL_STACK_UNDERFLOW"<<std::endl;break;
        case GL_STACK_OVERFLOW:
            std::cerr<<"GL_STACK_OVERFLOW"<<std::endl;break;
#endif
        default:
            std::cerr<<"GL_UNKNOWN"<<std::endl;break;
        }
    }
    return err;
}

void OpenGL::loadExtension()
{
    //glGetString(GL_EXTENSIONS)
    //QByteArray extString
      //  (reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS)));
    QString ext=reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS));
    ext_texture_array=ext.contains("GL_EXT_texture_array");
    if(!ext_texture_array)
        std::cerr<<"GL_EXT_texture_array"<<" not available"<<std::endl;
}

void * OpenGL::getProcAddress(const char * name)
{
    void * tmp=(void *)context->getProcAddress(name);
    if(tmp==NULL)
    {
        std::string nname=name;
        nname+="ARB";
        tmp=(void *)context->getProcAddress(nname.c_str());
        if(tmp==NULL)
        {
            std::string nname=name;
            nname+="EXT";
            tmp=(void *)context->getProcAddress(nname.c_str());
            if(tmp==NULL)
            {
                std::cerr<<"Unable to load OpenGL function "<<name<<"!"<<std::endl;
            }
        }
    }
    return tmp;
}

static inline QRgb qt_gl_convertToGLFormatHelper(QRgb src_pixel, GLenum texture_format)

{

    if (texture_format == GL_BGRA) {

        if (QSysInfo::ByteOrder == QSysInfo::BigEndian) {

            return ((src_pixel << 24) & 0xff000000)

                   | ((src_pixel >> 24) & 0x000000ff)

                   | ((src_pixel << 8) & 0x00ff0000)

                   | ((src_pixel >> 8) & 0x0000ff00);

        } else {

            return src_pixel;

        }

    } else {  // GL_RGBA

        if (QSysInfo::ByteOrder == QSysInfo::BigEndian) {

            return (src_pixel << 8) | ((src_pixel >> 24) & 0xff);

        } else {

            return ((src_pixel << 16) & 0xff0000)

                   | ((src_pixel >> 16) & 0xff)

                   | (src_pixel & 0xff00ff00);

        }

    }
}


QRgb qt_gl_convertToGLFormat(QRgb src_pixel, GLenum texture_format)
{
    return qt_gl_convertToGLFormatHelper(src_pixel, texture_format);
}

QImage OpenGL::convertToGLFormat(const QImage& img)
{
    QImage res(img.size(), QImage::Format_ARGB32);

    convertToGLFormatHelper(res, img.convertToFormat(QImage::Format_ARGB32), GL_RGBA);
    return res;
}

void OpenGL::convertToGLFormatHelper(QImage &dst, const QImage &img, GLenum texture_format)

{
    Q_ASSERT(dst.depth() == 32);

    Q_ASSERT(img.depth() == 32);

    if (dst.size() != img.size()) {

        int target_width = dst.width();

        int target_height = dst.height();

        qreal sx = target_width / qreal(img.width());

        qreal sy = target_height / qreal(img.height());

        quint32 *dest = (quint32 *) dst.scanLine(0); // NB! avoid detach here

        uchar *srcPixels = (uchar *) img.scanLine(img.height() - 1);

        int sbpl = img.bytesPerLine();

        int dbpl = dst.bytesPerLine();

        int ix = int(0x00010000 / sx);

        int iy = int(0x00010000 / sy);

        quint32 basex = int(0.5 * ix);

        quint32 srcy = int(0.5 * iy);

        // scale, swizzle and mirror in one loop

        while (target_height--) {

            const uint *src = (const quint32 *) (srcPixels - (srcy >> 16) * sbpl);

            int srcx = basex;

            for (int x=0; x<target_width; ++x) {

                dest[x] = qt_gl_convertToGLFormatHelper(src[srcx >> 16], texture_format);

                srcx += ix;

            }

            dest = (quint32 *)(((uchar *) dest) + dbpl);

            srcy += iy;

        }

    } else {

        const int width = img.width();

        const int height = img.height();

        const uint *p = (const uint*) img.scanLine(img.height() - 1);

        uint *q = (uint*) dst.scanLine(0);

        if (texture_format == GL_BGRA) {

            if (QSysInfo::ByteOrder == QSysInfo::BigEndian) {

                // mirror + swizzle

                for (int i=0; i < height; ++i) {

                    const uint *end = p + width;

                    while (p < end) {

                        *q = ((*p << 24) & 0xff000000)

                             | ((*p >> 24) & 0x000000ff)

                             | ((*p << 8) & 0x00ff0000)

                             | ((*p >> 8) & 0x0000ff00);
                        p++;
                        q++;
                    }

                    p -= 2 * width;

                }

            } else {

                const uint bytesPerLine = img.bytesPerLine();

                for (int i=0; i < height; ++i) {

                    memcpy(q, p, bytesPerLine);

                    q += width;

                    p -= width;

                }

            }

        } else {

            if (QSysInfo::ByteOrder == QSysInfo::BigEndian) {

                for (int i=0; i < height; ++i) {

                    const uint *end = p + width;

                    while (p < end) {

                        *q = (*p << 8) | ((*p >> 24) & 0xff);

                        p++;

                        q++;

                    }

                    p -= 2 * width;

                }

            } else {

                for (int i=0; i < height; ++i) {

                    const uint *end = p + width;

                    while (p < end) {

                        *q = ((*p << 16) & 0xff0000) | ((*p >> 16) & 0xff) | (*p & 0xff00ff00);

                        p++;

                        q++;

                    }

                    p -= 2 * width;

                }

            }

        }

    }

}


bool OpenGL::ext_texture_array=false;
OpenGL::OpenGLVersion OpenGL::opengl_version=OpenGL::OPENGL_UNKNOWN;
