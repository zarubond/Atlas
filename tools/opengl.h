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
#ifndef OPENGL_H
#define OPENGL_H


#include <QtGlobal>
#include <QGLFormat>

#if QT_VERSION < 0x050000
    #include <QGLContext>
    typedef QGLContext QOpenGLContext;
#else
    #include <QOpenGLContext>
    #include <QImage>
#endif

#include <QDebug>

#include <iostream>
#include <string>

#if QT_VERSION < 0x050000
typedef QGLContext QOpenGLContext;
#endif

#ifdef Q_WS_X11
    #define Q_OS_LINUX
#endif

#ifdef Q_WS_WIN
    #define Q_OS_WIN
#endif

#ifdef Q_OS_WIN
    #include <GL/glext.h>
    typedef bool (APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);

    #undef near
    #undef far
    #undef NEAR
    #undef FAR
#endif


#define OPENGL_VERSION 2
//#define OPENGL_VERSION 3

#ifdef Q_OS_ANDROID
    #define OPENGL_ES
    #define Q_WS_ANDROID
    #define GL_TEXTURE_2D_ARRAY    0x8C1A
    #define GL_BGRA 0x80E1
    //should not be defined
    #define GL_TEXTURE_MAX_LEVEL			0x813D
    #define GL_TEXTURE_BORDER_COLOR			0x1004
    #define GL_RGBA8                        0x8058
#endif

#define STR_VALUE(arg)      #arg

#ifndef OPENGL_ES

#define OPENGL_FUNTION_LIST \
    OPENGL_FUNTION(PFNGLGENBUFFERSPROC   , glGenBuffers) \
    OPENGL_FUNTION(PFNGLDELETEBUFFERSPROC, glDeleteBuffers) \
    OPENGL_FUNTION(PFNGLBINDBUFFERPROC   , glBindBuffer) \
    OPENGL_FUNTION(PFNGLBUFFERDATAPROC   , glBufferData) \
    OPENGL_FUNTION(PFNGLBUFFERSUBDATAPROC, glBufferSubData) \
    OPENGL_FUNTION(PFNGLCREATESHADERPROC , glCreateShader) \
    OPENGL_FUNTION(PFNGLSHADERSOURCEPROC , glShaderSource) \
    OPENGL_FUNTION(PFNGLCOMPILESHADERPROC, glCompileShader) \
    OPENGL_FUNTION(PFNGLGETSHADERIVPROC  , glGetShaderiv) \
    OPENGL_FUNTION(PFNGLCREATEPROGRAMPROC, glCreateProgram) \
    OPENGL_FUNTION(PFNGLATTACHSHADERPROC , glAttachShader) \
    OPENGL_FUNTION(PFNGLLINKPROGRAMPROC  , glLinkProgram) \
    OPENGL_FUNTION(PFNGLGETPROGRAMIVPROC , glGetProgramiv) \
    OPENGL_FUNTION(PFNGLUSEPROGRAMPROC   , glUseProgram) \
    OPENGL_FUNTION(PFNGLENABLEVERTEXATTRIBARRAYPROC   ,  glEnableVertexAttribArray) \
    OPENGL_FUNTION(PFNGLVERTEXATTRIBPOINTERPROC       ,  glVertexAttribPointer) \
    OPENGL_FUNTION(PFNGLDISABLEVERTEXATTRIBARRAYPROC  ,  glDisableVertexAttribArray) \
    OPENGL_FUNTION(PFNGLGETRENDERBUFFERPARAMETERIVPROC,  glGetRenderbufferParameteriv) \
    OPENGL_FUNTION(PFNGLGETATTRIBLOCATIONPROC   ,        glGetAttribLocation) \
    OPENGL_FUNTION(PFNGLGETUNIFORMLOCATIONPROC  ,        glGetUniformLocation) \
    OPENGL_FUNTION(PFNGLUNIFORMMATRIX4FVPROC    ,        glUniformMatrix4fv) \
    OPENGL_FUNTION(PFNGLUNIFORMMATRIX3FVPROC    ,        glUniformMatrix3fv) \
    OPENGL_FUNTION(PFNGLISSHADERPROC            ,        glIsShader) \
    OPENGL_FUNTION(PFNGLISPROGRAMPROC           ,        glIsProgram) \
    OPENGL_FUNTION(PFNGLGETSHADERINFOLOGPROC    ,        glGetShaderInfoLog) \
    OPENGL_FUNTION(PFNGLGETPROGRAMINFOLOGPROC   ,        glGetProgramInfoLog) \
    OPENGL_FUNTION(PFNGLDELETESHADERPROC        ,        glDeleteShader) \
    OPENGL_FUNTION(PFNGLGETBUFFERPARAMETERIVPROC,        glGetBufferParameteriv) \
    OPENGL_FUNTION(PFNGLUNIFORM1IPROC           ,        glUniform1i) \
    OPENGL_FUNTION(PFNGLUNIFORM1UIPROC          ,        glUniform1ui) \
    OPENGL_FUNTION(PFNGLUNIFORM2IPROC           ,        glUniform2i) \
    OPENGL_FUNTION(PFNGLUNIFORM3IPROC           ,        glUniform3i) \
    OPENGL_FUNTION(PFNGLUNIFORM4IPROC           ,        glUniform4i) \
    OPENGL_FUNTION(PFNGLUNIFORM1FPROC           ,        glUniform1f) \
    OPENGL_FUNTION(PFNGLUNIFORM2FPROC           ,        glUniform2f) \
    OPENGL_FUNTION(PFNGLUNIFORM3FPROC           ,        glUniform3f) \
    OPENGL_FUNTION(PFNGLUNIFORM4FPROC           ,        glUniform4f) \
    OPENGL_FUNTION(PFNGLUNIFORM4FVPROC          ,        glUniform4fv) \
    OPENGL_FUNTION(PFNGLUNIFORM3FVPROC          ,        glUniform3fv) \
    OPENGL_FUNTION(PFNGLDELETEPROGRAMPROC       ,        glDeleteProgram) \
    OPENGL_FUNTION(PFNGLMAPBUFFERPROC           ,        glMapBuffer) \
    OPENGL_FUNTION(PFNGLUNMAPBUFFERPROC         ,        glUnmapBuffer) \
    OPENGL_FUNTION(PFNGLGENRENDERBUFFERSPROC    ,        glGenRenderbuffers) \
    OPENGL_FUNTION(PFNGLBINDRENDERBUFFEREXTPROC ,        glBindRenderbuffer) \
    OPENGL_FUNTION(PFNGLRENDERBUFFERSTORAGEPROC ,        glRenderbufferStorage) \
    OPENGL_FUNTION(PFNGLGENFRAMEBUFFERSPROC     ,        glGenFramebuffers) \
    OPENGL_FUNTION(PFNGLBINDFRAMEBUFFEREXTPROC  ,        glBindFramebuffer) \
    OPENGL_FUNTION(PFNGLFRAMEBUFFERTEXTURE2DEXTPROC    , glFramebufferTexture2D) \
    OPENGL_FUNTION(PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC , glFramebufferRenderbuffer) \
    OPENGL_FUNTION(PFNGLCHECKFRAMEBUFFERSTATUSPROC  ,    glCheckFramebufferStatus) \
    OPENGL_FUNTION(PFNGLGETACTIVEUNIFORMPROC        ,    glGetActiveUniform) \
    OPENGL_FUNTION(PFNGLDRAWBUFFERSPROC             ,    glDrawBuffers) \
    OPENGL_FUNTION(PFNGLTEXSTORAGE3DPROC            ,    glTexStorage3D)

#define OPENGL_FUNTION(PFNGL,Name) \
extern PFNGL Name;
OPENGL_FUNTION_LIST
#undef OPENGL_FUNTION

#define OPENGL_FUNTION_LIST_WIN \
    OPENGL_FUNTION(PFNGLACTIVETEXTUREPROC, glActiveTexture) \
    OPENGL_FUNTION(PFNGLTEXIMAGE3DPROC   , glTexImage3D) \
    OPENGL_FUNTION(PFNGLTEXSUBIMAGE3DPROC, glTexSubImage3D)

#ifdef Q_OS_WIN
    #define OPENGL_FUNTION(Pfngl,Name) \
        extern Pfngl Name;
    OPENGL_FUNTION_LIST_WIN
    #undef OPENGL_FUNTION
#endif

#define OPENGL_FUNTION_LIST_3 \
    OPENGL_FUNTION(PFNGLBINDVERTEXARRAYPROC    , glBindVertexArray) \
    OPENGL_FUNTION(PFNGLDELETEVERTEXARRAYSPROC , glDeleteVertexArrays) \
    OPENGL_FUNTION(PFNGLGENVERTEXARRAYSPROC    , glGenVertexArrays) \
    OPENGL_FUNTION(PFNGLISVERTEXARRAYPROC      , glIsVertexArray) \
    OPENGL_FUNTION(PFNGLBINDATTRIBLOCATIONPROC , glBindAttribLocation) \
    OPENGL_FUNTION(PFNGLVERTEXATTRIB3FPROC     , glVertexAttrib3f)

#if OPENGL_VERSION == 3
    #define OPENGL_FUNTION(Pfngl,Name) \
        extern Pfngl Name;
    OPENGL_FUNTION_LIST_3
    #undef OPENGL_FUNTION
#endif

#else

#define OPENGL_FUNTION_LIST_ES \
    OPENGL_FUNTION(PFNGLTEXSTORAGE3DEXTPROC, glTexStorage3D) \
    OPENGL_FUNTION(PFNGLTEXIMAGE3DOESPROC  , glTexImage3D)

#define OPENGL_FUNTION(Pfngl,Name) \
    extern Pfngl Name;
    OPENGL_FUNTION_LIST_ES
#undef OPENGL_FUNTION

#endif

#define OPENGL_OFFSET(i) ((char *)NULL + (i))

/*
void oglFunc(NAME)
    {
        std::cout<<"OpenGL glGenBuffers"\
    <<", file:"<<__FILE__<<", line:"<<__LINE__<<", error"<<OpenGL::Error()<<std::endl;\
    NAME##_A(__VA_ARGS__);
}

//cant use x-mactors
#define OPENGL_ERROR(NAME,...) \
    {std::cout<<"OpenGL glGenBuffers"\
    <<", file:"<<__FILE__<<", line:"<<__LINE__<<", error"<<OpenGL::Error()<<std::endl;\
    NAME##_A(__VA_ARGS__)

#define glGenBuffers(...)       OPENGL_ERROR(glGenBuffers,__VA_ARGS__)
#define glDeleteBuffers(...)    OPENGL_ERROR(glDeleteBuffers,__VA_ARGS__)
#define glBindBuffer(...)       OPENGL_ERROR(glBindBuffer,__VA_ARGS__)
#define glBufferData(...)       OPENGL_ERROR(glBufferData,__VA_ARGS__)
#define glBufferSubData(...)    OPENGL_ERROR(glBufferSubData,__VA_ARGS__)
#define glCreateShader(...)     OPENGL_ERROR(glCreateShader,__VA_ARGS__)
#define glShaderSource(...)     OPENGL_ERROR(glShaderSource,__VA_ARGS__)
#define glCompileShader(...)    OPENGL_ERROR(glCompileShader,__VA_ARGS__)
#define glGetShaderiv(...)      OPENGL_ERROR(glGetShaderiv,__VA_ARGS__)
#define glCreateProgram(...)    OPENGL_ERROR(glCreateProgram,__VA_ARGS__)
#define glAttachShader(...)     OPENGL_ERROR(glAttachShader,__VA_ARGS__)
#define glLinkProgram(...)      OPENGL_ERROR(glLinkProgram,__VA_ARGS__)
#define glGetProgramiv(...)     OPENGL_ERROR(glGetProgramiv,__VA_ARGS__)
#define glUseProgram(...)       OPENGL_ERROR(glUseProgram,__VA_ARGS__)
#define glEnableVertexAttribArray(...)      OPENGL_ERROR(glEnableVertexAttribArray,__VA_ARGS__)
#define glVertexAttribPointer(...)          OPENGL_ERROR(glVertexAttribPointer,__VA_ARGS__)
#define glDisableVertexAttribArray(...)     OPENGL_ERROR(glDisableVertexAttribArray,__VA_ARGS__)
#define glGetRenderbufferParameteriv(...)   OPENGL_ERROR(glGetRenderbufferParameteriv,__VA_ARGS__)
#define glGetAttribLocation(...)    OPENGL_ERROR(glGetAttribLocation,__VA_ARGS__)
#define glGetUniformLocation(...)   OPENGL_ERROR(glGetUniformLocation,__VA_ARGS__)
#define glUniformMatrix4fv(...)     OPENGL_ERROR(glUniformMatrix4fv,__VA_ARGS__)
#define glUniformMatrix3fv(...)     OPENGL_ERROR(glUniformMatrix3fv,__VA_ARGS__)
#define glIsShader(...)         OPENGL_ERROR(glIsShader,__VA_ARGS__)
#define glIsProgram(...)        OPENGL_ERROR(glIsProgram,__VA_ARGS__)
#define glGetShaderInfoLog(...) OPENGL_ERROR(glGetShaderInfoLog,__VA_ARGS__)
#define glGetProgramInfoLog(...)OPENGL_ERROR(glGetProgramInfoLog,__VA_ARGS__)
#define glDeleteShader(...)     OPENGL_ERROR(glDeleteShader,__VA_ARGS__)
#define glGetBufferParameteriv(...) OPENGL_ERROR(glGetBufferParameteriv,__VA_ARGS__)
#define glUniform1i(...)    OPENGL_ERROR(glUniform1i,__VA_ARGS__)
#define glUniform1ui(...)   OPENGL_ERROR(glUniform1ui,__VA_ARGS__)
#define glUniform2i(...)    OPENGL_ERROR(glUniform2i,__VA_ARGS__)
#define glUniform3i(...)    OPENGL_ERROR(glUniform3i,__VA_ARGS__)
#define glUniform4i(...)    OPENGL_ERROR(glUniform4i,__VA_ARGS__)
#define glUniform1f(...)    OPENGL_ERROR(glUniform1f,__VA_ARGS__)
#define glUniform2f(...)    OPENGL_ERROR(glUniform2f,__VA_ARGS__)
#define glUniform3f(...)    OPENGL_ERROR(glUniform3f,__VA_ARGS__)
#define glUniform4f(...)    OPENGL_ERROR(glUniform4f,__VA_ARGS__)
#define glUniform4fv(...)   OPENGL_ERROR(glUniform4fv,__VA_ARGS__)
#define glUniform3fv(...)   OPENGL_ERROR(glUniform3fv,__VA_ARGS__)
#define glDeleteProgram(...)    OPENGL_ERROR(glDeleteProgram,__VA_ARGS__)
#define glMapBuffer(...)        OPENGL_ERROR(glMapBuffer,__VA_ARGS__)
#define glUnmapBuffer(...)      OPENGL_ERROR(glUnmapBuffer,__VA_ARGS__)
#define glGenRenderbuffers(...) OPENGL_ERROR(glGenRenderbuffers,__VA_ARGS__)
#define glBindRenderbuffer(...) OPENGL_ERROR(glBindRenderbuffer,__VA_ARGS__)
#define glRenderbufferStorage(...)  OPENGL_ERROR(glRenderbufferStorage,__VA_ARGS__)
#define glGenFramebuffers(...)      OPENGL_ERROR(glGenFramebuffers,__VA_ARGS__)
#define glBindFramebuffer(...)      OPENGL_ERROR(glBindFramebuffer,__VA_ARGS__)
#define glFramebufferTexture2D(...) OPENGL_ERROR(glFramebufferTexture2D,__VA_ARGS__)
#define glFramebufferRenderbuffer(...)  OPENGL_ERROR(glFramebufferRenderbuffer,__VA_ARGS__)
#define glCheckFramebufferStatus(...)   OPENGL_ERROR(glCheckFramebufferStatus,__VA_ARGS__)
#define glGetActiveUniform(...)         OPENGL_ERROR(glGetActiveUniform,__VA_ARGS__)
#define glGetActiveUniformglDrawBuffers(...) OPENGL_ERROR(glGetActiveUniformglDrawBuffers,__VA_ARGS__)

// ALL OTHRER FUNCTIONS

#define glGenTextures(...) OPENGL_ERROR(glGenTextures,__VA_ARGS__)
#define glBindTexture(...) OPENGL_ERROR(glBindTexture,__VA_ARGS__)
#define glDeleteTextures(...) OPENGL_ERROR(glDeleteTextures,__VA_ARGS__)
#define glTexParameteri(...) OPENGL_ERROR(glTexParameteri,__VA_ARGS__)
#define glTexImage2D(...) OPENGL_ERROR(glTexImage2D,__VA_ARGS__)
#define glActiveTexture(...) OPENGL_ERROR(glActiveTexture,__VA_ARGS__)
*/



class OpenGL
{
public:
        enum OpenGLVersion{
            OPENGL_2_0=0,
            OPENGL_ES_2_0,
            OPENGL_3_0,
            OPENGL_UNKNOWN
        };

    OpenGL(const QOpenGLContext *context);
    /**
     * @brief init Initialize OpenGL
     * @return  True on success.
     */
    bool init();
    /**
     * @brief vSync Display vertical synchronization.
     * @param sync true-on false-off
     */
    void vSync(bool sync);
    /**
     * @brief Error Print OpenGL error.
     */

    static bool EXTTextureArray();

    static int Error();
    static OpenGLVersion version();

    static QImage convertToGLFormat(const QImage &img);
private:
    static void convertToGLFormatHelper(QImage &dst, const QImage &img, GLenum texture_format);

#if defined(Q_OS_LINUX)
    void (*swapInterval)(int);
#endif

#ifdef Q_OS_WIN
    PFNWGLSWAPINTERVALFARPROC swapInterval;
#endif
    void loadExtension();

    const QOpenGLContext * context;
    static bool ext_texture_array;
    static OpenGLVersion opengl_version;
    void * getProcAddress(const char * name);
};

#endif // OPENGL_H
