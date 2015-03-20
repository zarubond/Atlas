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
#ifndef OPENGLDEBUG_H
#define OPENGLDEBUG_H

//#define OPENGL_DEBUG

#ifdef OPENGL_DEBUG

#define GL_FUNCTION_DEBUG(Name,...) Name(__VA_ARGS__); glErrorPrint(__FILE__,__LINE__,#Name,#__VA_ARGS__);

#define glGenerateMipmap(...)       GL_FUNCTION_DEBUG(glGenerateMipmap,__VA_ARGS__)
#define glTexParameteri(...)        GL_FUNCTION_DEBUG(glTexParameteri,__VA_ARGS__)
#define glBindRenderbuffer(...)     GL_FUNCTION_DEBUG(glBindRenderbuffer,__VA_ARGS__)
#define glRenderbufferStorage(...)  GL_FUNCTION_DEBUG(glRenderbufferStorage,__VA_ARGS__)
#define glGenFramebuffers(...)      GL_FUNCTION_DEBUG(glGenFramebuffers,__VA_ARGS__)
#define glBindFramebuffer(...)      GL_FUNCTION_DEBUG(glBindFramebuffer,__VA_ARGS__)
#define glFramebufferTexture2D(...) GL_FUNCTION_DEBUG(glFramebufferTexture2D,__VA_ARGS__)
#define glFramebufferRenderbuffer(...) GL_FUNCTION_DEBUG(glFramebufferRenderbuffer,__VA_ARGS__)
#define glEnableVertexAttribArray(...) GL_FUNCTION_DEBUG(glEnableVertexAttribArray,__VA_ARGS__)
#define glDisableVertexAttribArray(...) GL_FUNCTION_DEBUG(glDisableVertexAttribArray,__VA_ARGS__)
#define glVertexAttribPointer(...)  GL_FUNCTION_DEBUG(glVertexAttribPointer,__VA_ARGS__)
#define glDrawBuffers(...)          GL_FUNCTION_DEBUG(glDrawBuffers,__VA_ARGS__)
#define glGenRenderbuffers(...)     GL_FUNCTION_DEBUG(glGenRenderbuffers,__VA_ARGS__)
#define glDeleteRenderbuffers(...)  GL_FUNCTION_DEBUG(glDeleteRenderbuffers,__VA_ARGS__)
#define glViewport(...)             GL_FUNCTION_DEBUG(glViewport,__VA_ARGS__)
#define glBindTexture(...)          GL_FUNCTION_DEBUG(glBindTexture,__VA_ARGS__)
#define glClear(...)                GL_FUNCTION_DEBUG(glClear,__VA_ARGS__)
#define glTexImage2D(...)           GL_FUNCTION_DEBUG(glTexImage2D,__VA_ARGS__)
#define glDrawBuffer(...)           GL_FUNCTION_DEBUG(glDrawBuffer,__VA_ARGS__)
#define glReadBuffer(...)           GL_FUNCTION_DEBUG(glReadBuffer,__VA_ARGS__)
#define glGenTextures(...)          GL_FUNCTION_DEBUG(glGenTextures,__VA_ARGS__)
#define glActiveTexture(...)        GL_FUNCTION_DEBUG(glActiveTexture,__VA_ARGS__)
#define glDeleteTextures(...)       GL_FUNCTION_DEBUG(glDeleteTextures,__VA_ARGS__)
#define glGenBuffers(...)           GL_FUNCTION_DEBUG(glGenBuffers,__VA_ARGS__)
#define glBindBuffer(...)           GL_FUNCTION_DEBUG(glBindBuffer,__VA_ARGS__)
#define glBufferData(...)           GL_FUNCTION_DEBUG(glBufferData,__VA_ARGS__)
#define glDrawElements(...)         GL_FUNCTION_DEBUG(glDrawElements,__VA_ARGS__)
#define glTexSubImage2D(...)        GL_FUNCTION_DEBUG(glTexSubImage2D,__VA_ARGS__)
#define glEnable(...)               GL_FUNCTION_DEBUG(glEnable,__VA_ARGS__)
#define glDisable(...)              GL_FUNCTION_DEBUG(glDisable,__VA_ARGS__)
#define glDepthMask(...)            GL_FUNCTION_DEBUG(glDepthMask,__VA_ARGS__)
#define glPolygonMode(...)          GL_FUNCTION_DEBUG(glPolygonMode,__VA_ARGS__)

#define glGetUniformLocation(...)   GL_FUNCTION_DEBUG(glGetUniformLocation,__VA_ARGS__)
#define glShaderSource(...)         GL_FUNCTION_DEBUG(glShaderSource,__VA_ARGS__)
#define glClearColor(...)           GL_FUNCTION_DEBUG(glClearColor,__VA_ARGS__)
#define glLinkProgram(...)          GL_FUNCTION_DEBUG(glLinkProgram,__VA_ARGS__)
#define glAttachShader(...)         GL_FUNCTION_DEBUG(glAttachShader,__VA_ARGS__)
#define glCreateProgram(...)        GL_FUNCTION_DEBUG(glCreateProgram,__VA_ARGS__)
#define glGetAttribLocation(...)    GL_FUNCTION_DEBUG(glGetAttribLocation,__VA_ARGS__)
#define glCompileShader(...)        GL_FUNCTION_DEBUG(glCompileShader,__VA_ARGS__)
#define glDeleteShader(...)         GL_FUNCTION_DEBUG(glDeleteShader,__VA_ARGS__)
#define glGenVertexArrays(...)      GL_FUNCTION_DEBUG(glGenVertexArrays,__VA_ARGS__)
#define glBindVertexArray(...)      GL_FUNCTION_DEBUG(glBindVertexArray,__VA_ARGS__)
#define glDrawArrays(...)           GL_FUNCTION_DEBUG(glDrawArrays,__VA_ARGS__)
#define glDeleteBuffers(...)        GL_FUNCTION_DEBUG(glDeleteBuffers,__VA_ARGS__)
#define glUseProgram(...)           GL_FUNCTION_DEBUG(glUseProgram,__VA_ARGS__)
#define glCreateShader(...)         GL_FUNCTION_DEBUG(glCreateShader,__VA_ARGS__)
#define glTexSubImage3D(...)        GL_FUNCTION_DEBUG(glTexSubImage3D,__VA_ARGS__)
#define glUniform1f(...)            GL_FUNCTION_DEBUG(glUniform1f,__VA_ARGS__)
#define glCheckFramebufferStatus(...) GL_FUNCTION_DEBUG(glCheckFramebufferStatus,__VA_ARGS__)
#define glTexStorage3D(...)         GL_FUNCTION_DEBUG(glTexStorage3D,__VA_ARGS__)
#define glGetShaderiv(...)          GL_FUNCTION_DEBUG(glGetShaderiv,__VA_ARGS__)
#define glTexImage3D(...)           GL_FUNCTION_DEBUG(glTexImage3D,__VA_ARGS__)

#endif

#endif // OPENGLDEBUG_H
