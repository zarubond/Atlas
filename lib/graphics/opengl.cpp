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

#include "opengl.h"

#define STR_VALUE(arg)      #arg

OpenGL::OpenGL()
{
}

bool OpenGL::glInit(QOpenGLContext *context, OpenGLVersion preferred)
{
    QSurfaceFormat format=context->format();
    if(format.renderableType()==QSurfaceFormat::OpenGL)
    {
        if(format.majorVersion()>=3)
            opengl_version=OpenGL::OPENGL_3_0;
        else
            opengl_version=OpenGL::OPENGL_UNKNOWN;
    }
    else if(format.renderableType()==QSurfaceFormat::OpenGLES)
    {
        if(format.majorVersion()>=3 && preferred!=OpenGL::OPENGL_ES_2_0)
            opengl_version=OPENGL_ES_3_0;
        else
            opengl_version=OpenGL::OPENGL_ES_2_0;
    }
    else
        opengl_version=OpenGL::OPENGL_UNKNOWN;

    if(opengl_version==OpenGL::OPENGL_UNKNOWN)
        cerr<<"Unsupported OpenGL version!!!"<<endl;
    if(opengl_version==OPENGL_ES_2_0)
        cout<<"Renderer OpenGL ES 2.0"<<endl;
    else if(opengl_version==OPENGL_ES_3_0)
        cout<<"Renderer OpenGL ES 3.0"<<endl;
    else if(opengl_version==OPENGL_3_0)
        cout<<"Renderer OpenGL 3.0"<<endl;

    if(opengl_version==OPENGL_ES_2_0 || opengl_version==OPENGL_ES_3_0)
    {
        #if defined(OS_ANDROID) || defined(OS_SAILFISH)
            #define OPENGL_FUNCTION(Return, Name, ...) \
                Name = (Return(__attribute__((__stdcall__))*)(__VA_ARGS__)) &::Name;
                GL_FUNCTION_LIST_ES_20
            #undef OPENGL_FUNCTION
            #define OPENGL_FUNCTION(Return, Name, ...) \
                Name = (Return(__attribute__((__stdcall__))*)(__VA_ARGS__)) getProcAddress(STR_VALUE(Name),context);
                GL_FUNCTION_LIST_ES_30
            #undef OPENGL_FUNCTION
        #else
            #define OPENGL_FUNCTION(Return, Name, ...) \
                Name = (Return(__attribute__((__stdcall__))*)(__VA_ARGS__)) &::Name;
                GL_FUNCTION_LIST_10
                GL_FUNCTION_LIST_11
            #undef OPENGL_FUNCTION

            #ifdef OS_WINDOWS
                #define OPENGL_FUNCTION(Return, Name, ...) \
                    Name = (Return(__attribute__((__stdcall__))*)(__VA_ARGS__))  getProcAddress(STR_VALUE(Name),context);
                    GL_FUNCTION_LIST_12
                    GL_FUNCTION_LIST_13
                #undef OPENGL_FUNCTION
            #else
                #define OPENGL_FUNCTION(Return, Name, ...) \
                    Name = (Return(__attribute__((__stdcall__))*)(__VA_ARGS__)) &::Name;
                    GL_FUNCTION_LIST_12
                    GL_FUNCTION_LIST_13
                #undef OPENGL_FUNCTION
            #endif

            #define OPENGL_FUNCTION(Return, Name, ...) \
                Name = (Return(__attribute__((__stdcall__))*)(__VA_ARGS__)) getProcAddress(STR_VALUE(Name),context);
                GL_FUNCTION_LIST_14
                GL_FUNCTION_LIST_15
                GL_FUNCTION_LIST_20
                GL_FUNCTION_LIST_21
                GL_FUNCTION_LIST_30
                GL_FUNCTION_LIST_31
                GL_FUNCTION_LIST_32
            #undef OPENGL_FUNCTION
        #endif
    }
    else
    {
        #if !defined(OS_ANDROID) && !defined(OS_SAILFISH)
            #define OPENGL_FUNCTION(Return, Name, ...) \
                Name = (Return(__attribute__((__stdcall__))*)(__VA_ARGS__)) &::Name;
                GL_FUNCTION_LIST_10
                GL_FUNCTION_LIST_11
                GL_FUNCTION_LIST_ES_MISSING
            #undef OPENGL_FUNCTION

            #ifdef OS_WINDOWS
                #define OPENGL_FUNCTION(Return, Name, ...) \
                    Name = (Return(__attribute__((__stdcall__))*)(__VA_ARGS__))  getProcAddress(STR_VALUE(Name),context);
                    GL_FUNCTION_LIST_12
                    GL_FUNCTION_LIST_13
                #undef OPENGL_FUNCTION
            #else
                #define OPENGL_FUNCTION(Return, Name, ...) \
                    Name = (Return(__attribute__((__stdcall__))*)(__VA_ARGS__)) &::Name;
                    GL_FUNCTION_LIST_12
                    GL_FUNCTION_LIST_13
                #undef OPENGL_FUNCTION
            #endif

            #define OPENGL_FUNCTION(Return, Name, ...) \
                Name = (Return(__attribute__((__stdcall__))*)(__VA_ARGS__)) getProcAddress(STR_VALUE(Name),context);
                GL_FUNCTION_LIST_14
                GL_FUNCTION_LIST_15
                GL_FUNCTION_LIST_20
                GL_FUNCTION_LIST_21
                GL_FUNCTION_LIST_30
                GL_FUNCTION_LIST_31
                GL_FUNCTION_LIST_32
            #undef OPENGL_FUNCTION
        #endif
    }


#ifdef OS_LINUX
    swapInterval = (void (*)(int)) context->getProcAddress("glXSwapInterval");

    if(swapInterval==NULL)
        swapInterval = (void (*)(int)) context->getProcAddress("glXSwapIntervalMESA");

    if(swapInterval==NULL)
        swapInterval = (void (*)(int)) context->getProcAddress("glXSwapIntervalSGI");
#elif defined(OS_WINDOWS)
    swapInterval = (bool(*)(int)) context->getProcAddress("wglSwapIntervalEXT");
    if(swapInterval==NULL)
        swapInterval = (bool(*)(int)) context->getProcAddress("wglSwapInterval");

#endif

    //glDebugMessageCallback(&OpenGL::debugCallback, NULL);

    loadExtension();
    is_loaded=true;

    return true;
}

void OpenGL::vSync(bool sync)
{
#if defined(OS_LINUX) || defined(OS_WINDOWS)
    if(swapInterval!=NULL)
    {
        if(sync)
            swapInterval(1);
        else
            swapInterval(0);
    }
#endif
}

OpenGL::OpenGLVersion OpenGL::glVersion()
{
    return opengl_version;
}

int OpenGL::glMajorVersion()
{
    return 0;
}

int OpenGL::glMinorVersion()
{
    return 0;
}

bool OpenGL::isTextureArray()
{
    return ext_texture_array;
}

bool OpenGL::isDepth24()
{
    return ext_depth24;
}

int OpenGL::glError()
{
    int err=0;
    while((err=glGetError())!=GL_NO_ERROR)
    {
        switch(err)
        {
        case GL_NO_ERROR:
            cerr<<"GL_NO_ERROR"<<endl;break;
        case GL_INVALID_ENUM:
            cerr<<"GL_INVALID_ENUM"<<endl;break;
        case GL_INVALID_VALUE:
            cerr<<"GL_INVALID_VALUE"<<endl;break;
        case GL_INVALID_OPERATION:
            cerr<<"GL_INVALID_OPERATION"<<endl;break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            cerr<<"GL_INVALID_FRAMEBUFFER_OPERATION"<<endl;break;
        case GL_OUT_OF_MEMORY:
            cerr<<"GL_OUT_OF_MEMORY"<<endl;break;
/* #ifndef OPENGL_ES
        case GL_STACK_UNDERFLOW:
            cerr<<"GL_STACK_UNDERFLOW"<<endl;break;
        case GL_STACK_OVERFLOW:
            cerr<<"GL_STACK_OVERFLOW"<<endl;break;
#endif*/
        default:
            cerr<<"GL_UNKNOWN"<<endl;break;
        }
    }
    return err;
}

void OpenGL::glErrorPrint(const char * file, int line, const char *function, const char * params)
{
    int err=glGetError();
    while(err!=GL_NO_ERROR)
    {
        cerr<<"File:"<<file<<":"<<line<<" Function:"<<function<<" Params:"<<params<<" Error:";
        switch(err)
        {
        case GL_NO_ERROR:                       cerr<<"GL_NO_ERROR"<<endl;break;
        case GL_INVALID_ENUM:                   cerr<<"GL_INVALID_ENUM"<<endl;break;
        case GL_INVALID_VALUE:                  cerr<<"GL_INVALID_VALUE"<<endl;break;
        case GL_INVALID_OPERATION:              cerr<<"GL_INVALID_OPERATION"<<endl;break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:  cerr<<"GL_INVALID_FRAMEBUFFER_OPERATION"<<endl;break;
        case GL_OUT_OF_MEMORY:                  cerr<<"GL_OUT_OF_MEMORY"<<endl;break;
/* //#ifndef OPENGL_ES
        case GL_STACK_UNDERFLOW:                cerr<<"GL_STACK_UNDERFLOW"<<endl;break;
        case GL_STACK_OVERFLOW:                 cerr<<"GL_STACK_OVERFLOW"<<endl;break;
//#endif*/
        default:                                cerr<<"GL_UNKNOWN"<<endl;break;
        }
        err=glGetError();
    }
}

void OpenGL::loadExtension()
{
    const char *ext=reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS));
    if(ext==NULL)
    {
        cerr<<"Unable to load OpenGL extensings!!!"<<endl;
    }
    else
    {
        if(opengl_version==OPENGL_ES_3_0)
        {
            ext_texture_array=true;
            ext_depth24=true;
        }
        else if(opengl_version==OPENGL_ES_2_0)
        {
            ext_texture_array=false;
            if(strstr(ext,"GL_OES_depth24")==NULL)
            {
                cerr<<"GL_OES_depth24"<<" not available!"<<endl;
                ext_depth24=false;
            }
            else
                ext_depth24=true;
        }
        else if(opengl_version==OPENGL_3_0)
        {
            ext_depth24=true;
            if(strstr(ext,"GL_EXT_texture_array")==NULL)
            {
                cerr<<"GL_EXT_texture_array"<<" not available!"<<endl;
                ext_texture_array=false;
            }
            else
                ext_texture_array=true;
        }
    }
}

QFunctionPointer OpenGL::getProcAddress(const char * name,QOpenGLContext *context)
{
    QFunctionPointer tmp=context->getProcAddress(name);
    if(tmp==NULL)
    {
        std::string nname=name;
        nname+="ARB";
        tmp=context->getProcAddress(nname.c_str());
        if(tmp==NULL)
        {
            std::string nname=name;
            nname+="EXT";
            tmp=context->getProcAddress(nname.c_str());
            if(tmp==NULL)
            {
                cerr<<"Unable to load OpenGL function "<<name<<"!"<<endl;
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

bool OpenGL::isLoaded()
{
    return is_loaded;
}

void OpenGL::convertToGLFormatHelper(QImage &dst, const QImage &img, GLenum texture_format)

{
    Q_ASSERT(dst.depth() == 32);

    Q_ASSERT(img.depth() == 32);

    if (dst.size() != img.size()) {

        int target_width = dst.width();

        int target_height = dst.height();

        float sx = target_width / float(img.width());

        float sy = target_height / float(img.height());

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

void OpenGL::debugCallback(GLuint source, GLuint type, GLuint id, GLuint severity, GLint length, const char *message, void *userParam)
{
    cerr<<"OpenGL Error:"<<message<<endl;
}


bool OpenGL::ext_texture_array=false;
bool OpenGL::ext_depth24=false;
bool OpenGL::is_loaded=false;
OpenGL::OpenGLVersion OpenGL::opengl_version=OpenGL::OPENGL_UNKNOWN;

#if defined(OS_LINUX)
void (*OpenGL::swapInterval)(int)=NULL;
#elif defined(OS_WINDOWS)
bool (*OpenGL::swapInterval)(int)=NULL;
#else
void * OpenGL::swapInterval=NULL;
#endif


#define OPENGL_FUNCTION(Return,Name, ...) \
    Return (__attribute__((__stdcall__))*OpenGL::Name)(__VA_ARGS__)=NULL;
    GL_FUNCTION_LIST_ES_20
    GL_FUNCTION_LIST_ES_30
    GL_FUNCTION_LIST_ES_MISSING
#undef OPENGL_FUNCTION
