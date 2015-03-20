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
#ifndef OPENGLFUNCTIONS_H
#define OPENGLFUNCTIONS_H

//http://www.opengl.org/registry/api/glcorearb.h

#define GL_FUNCTION_LIST_10 \
    OPENGL_FUNCTION(void, glCullFace, GLenum mode) \
    OPENGL_FUNCTION(void, glFrontFace, GLenum mode) \
    OPENGL_FUNCTION(void, glHint, GLenum target, GLenum mode) \
    OPENGL_FUNCTION(void, glLineWidth, GLfloat width) \
    OPENGL_FUNCTION(void, glScissor, GLint x, GLint y, GLsizei width, GLsizei height) \
    OPENGL_FUNCTION(void, glTexParameterf, GLenum target, GLenum pname, GLfloat param) \
    OPENGL_FUNCTION(void, glTexParameterfv, GLenum target, GLenum pname, const GLfloat *params) \
    OPENGL_FUNCTION(void, glTexParameteri, GLenum target, GLenum pname, GLint param) \
    OPENGL_FUNCTION(void, glTexParameteriv, GLenum target, GLenum pname, const GLint *params) \
    OPENGL_FUNCTION(void, glTexImage2D, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels) \
    OPENGL_FUNCTION(void, glClear, GLbitfield mask) \
    OPENGL_FUNCTION(void, glClearColor, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) \
    OPENGL_FUNCTION(void, glClearStencil, GLint s) \
    OPENGL_FUNCTION(void, glStencilMask, GLuint mask) \
    OPENGL_FUNCTION(void, glColorMask, GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha) \
    OPENGL_FUNCTION(void, glDepthMask, GLboolean flag) \
    OPENGL_FUNCTION(void, glDisable, GLenum cap) \
    OPENGL_FUNCTION(void, glEnable, GLenum cap) \
    OPENGL_FUNCTION(void, glFinish, void) \
    OPENGL_FUNCTION(void, glFlush, void) \
    OPENGL_FUNCTION(void, glBlendFunc, GLenum sfactor, GLenum dfactor) \
    OPENGL_FUNCTION(void, glStencilFunc, GLenum func, GLint ref, GLuint mask) \
    OPENGL_FUNCTION(void, glStencilOp, GLenum fail, GLenum zfail, GLenum zpass) \
    OPENGL_FUNCTION(void, glDepthFunc, GLenum func) \
    OPENGL_FUNCTION(void, glPixelStorei, GLenum pname, GLint param) \
    OPENGL_FUNCTION(void, glReadBuffer, GLenum mode) \
    OPENGL_FUNCTION(void, glReadPixels, GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels) \
    OPENGL_FUNCTION(void, glGetBooleanv, GLenum pname, GLboolean *params) \
    OPENGL_FUNCTION(GLenum, glGetError, void) \
    OPENGL_FUNCTION(void, glGetFloatv, GLenum pname, GLfloat *params) \
    OPENGL_FUNCTION(void, glGetIntegerv, GLenum pname, GLint *params) \
    OPENGL_FUNCTION(const GLubyte *, glGetString, GLenum name) \
    OPENGL_FUNCTION(void, glGetTexParameterfv, GLenum target, GLenum pname, GLfloat *params) \
    OPENGL_FUNCTION(void, glGetTexParameteriv, GLenum target, GLenum pname, GLint *params) \
    OPENGL_FUNCTION(GLboolean, glIsEnabled, GLenum cap) \
    OPENGL_FUNCTION(void, glViewport, GLint x, GLint y, GLsizei width, GLsizei height)
    /* NO ES
    OPENGL_FUNCTION(void, glPolygonMode, GLenum face, GLenum mode) \
    OPENGL_FUNCTION(void, glPointSize, GLfloat size) \
    OPENGL_FUNCTION(void, glTexImage1D, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels) \
    OPENGL_FUNCTION(void, glClearDepth, GLdouble depth) \
    OPENGL_FUNCTION(void, glLogicOp, GLenum opcode) \
    OPENGL_FUNCTION(void, glPixelStoref, GLenum pname, GLfloat param) \
    OPENGL_FUNCTION(void, glGetDoublev, GLenum pname, GLdouble *params) \
    OPENGL_FUNCTION(void, glGetTexImage, GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels) \
    OPENGL_FUNCTION(void, glGetTexLevelParameterfv, GLenum target, GLint level, GLenum pname, GLfloat *params) \
    OPENGL_FUNCTION(void, glGetTexLevelParameteriv, GLenum target, GLint level, GLenum pname, GLint *params) \
    OPENGL_FUNCTION(void, glDepthRange, GLdouble near, GLdouble far) \
    OPENGL_FUNCTION(void, glDrawBuffer, GLenum mode) \

    */


#define GL_FUNCTION_LIST_11 \
    OPENGL_FUNCTION(void, glDrawArrays, GLenum mode, GLint first, GLsizei count) \
    OPENGL_FUNCTION(void, glDrawElements, GLenum mode, GLsizei count, GLenum type, const GLvoid *indices) \
    OPENGL_FUNCTION(void, glPolygonOffset, GLfloat factor, GLfloat units) \
    OPENGL_FUNCTION(void, glCopyTexImage2D, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border) \
    OPENGL_FUNCTION(void, glCopyTexSubImage2D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height) \
    OPENGL_FUNCTION(void, glTexSubImage2D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels) \
    OPENGL_FUNCTION(void, glBindTexture, GLenum target, GLuint texture) \
    OPENGL_FUNCTION(void, glDeleteTextures, GLsizei n, const GLuint *textures) \
    OPENGL_FUNCTION(void, glGenTextures, GLsizei n, GLuint *textures) \
    OPENGL_FUNCTION(GLboolean, glIsTexture, GLuint texture)
/* NO ES
     OPENGL_FUNCTION(void, glGetPointerv, GLenum pname, GLvoid* *params) \
     OPENGL_FUNCTION(void, glCopyTexImage1D, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border) \
     OPENGL_FUNCTION(void, glTexSubImage1D, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels) \
     OPENGL_FUNCTION(void, glCopyTexSubImage1D, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width) \
  */

#define GL_FUNCTION_LIST_12 \
    OPENGL_FUNCTION(void, glBlendColor, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) \
    OPENGL_FUNCTION(void, glBlendEquation, GLenum mode) \
    OPENGL_FUNCTION(void, glDrawRangeElements, GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices) \
    OPENGL_FUNCTION(void, glTexImage3D, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels) \
    OPENGL_FUNCTION(void, glTexSubImage3D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels)
/* NO ES
    OPENGL_FUNCTION(void, glCopyTexSubImage3D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)
 */
#define GL_FUNCTION_LIST_13 \
    OPENGL_FUNCTION(void, glActiveTexture, GLenum texture) \
    OPENGL_FUNCTION(void, glSampleCoverage, GLfloat value, GLboolean invert) \
    OPENGL_FUNCTION(void, glCompressedTexImage2D, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data) \
    OPENGL_FUNCTION(void, glCompressedTexSubImage2D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data)
    /* NO ES
    OPENGL_FUNCTION(void, glCompressedTexImage1D, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *data) \
    OPENGL_FUNCTION(void, glCompressedTexSubImage1D, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *data)  \
    OPENGL_FUNCTION(void, glGetCompressedTexImage, GLenum target, GLint level, GLvoid *img) \
    OPENGL_FUNCTION(void, glCompressedTexSubImage3D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data) \
    OPENGL_FUNCTION(void, glCompressedTexImage3D, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data) \

    */

#define GL_FUNCTION_LIST_14 \
    OPENGL_FUNCTION(void, glBlendFuncSeparate, GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
    /* NO ES
    OPENGL_FUNCTION(void, glMultiDrawArrays, GLenum mode, const GLint *first, const GLsizei *count, GLsizei drawcount) \
    OPENGL_FUNCTION(void, glMultiDrawElements, GLenum mode, const GLsizei *count, GLenum type, const GLvoid* const *indices, GLsizei drawcount) \
    OPENGL_FUNCTION(void, glPointParameterf, GLenum pname, GLfloat param) \
    OPENGL_FUNCTION(void, glPointParameterfv, GLenum pname, const GLfloat *params) \
    OPENGL_FUNCTION(void, glPointParameteri, GLenum pname, GLint param) \
    OPENGL_FUNCTION(void, glPointParameteriv, GLenum pname, const GLint *params) \
    */

#define GL_FUNCTION_LIST_15 \
    OPENGL_FUNCTION(void, glGenQueries, GLsizei n, GLuint *ids) \
    OPENGL_FUNCTION(void, glDeleteQueries, GLsizei n, const GLuint *ids) \
    OPENGL_FUNCTION(GLboolean, glIsQuery, GLuint id) \
    OPENGL_FUNCTION(void, glBeginQuery, GLenum target, GLuint id) \
    OPENGL_FUNCTION(void, glEndQuery, GLenum target) \
    OPENGL_FUNCTION(void, glGetQueryiv, GLenum target, GLenum pname, GLint *params) \
    OPENGL_FUNCTION(void, glGetQueryObjectuiv, GLuint id, GLenum pname, GLuint *params) \
    OPENGL_FUNCTION(void, glBindBuffer, GLenum target, GLuint buffer) \
    OPENGL_FUNCTION(void, glDeleteBuffers, GLsizei n, const GLuint *buffers) \
    OPENGL_FUNCTION(void, glGenBuffers, GLsizei n, GLuint *buffers) \
    OPENGL_FUNCTION(GLboolean, glIsBuffer, GLuint buffer) \
    OPENGL_FUNCTION(void, glBufferData, GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage) \
    OPENGL_FUNCTION(void, glBufferSubData, GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data) \
    OPENGL_FUNCTION(GLboolean, glUnmapBuffer, GLenum target) \
    OPENGL_FUNCTION(void, glGetBufferParameteriv, GLenum target, GLenum pname, GLint *params) \
    OPENGL_FUNCTION(void, glGetBufferPointerv, GLenum target, GLenum pname, GLvoid* *params)
    /* NO ES
    OPENGL_FUNCTION(void, glGetQueryObjectiv, GLuint id, GLenum pname, GLint *params) \
    OPENGL_FUNCTION(void, glGetBufferSubData, GLenum target, GLintptr offset, GLsizeiptr size, GLvoid *data) \
    OPENGL_FUNCTION(GLvoid*, glMapBuffer, GLenum target, GLenum access) \
     */

#define GL_FUNCTION_LIST_20 \
    OPENGL_FUNCTION(void, glBlendEquationSeparate, GLenum modeRGB, GLenum modeAlpha) \
    OPENGL_FUNCTION(void, glDrawBuffers, GLsizei n, const GLenum *bufs) \
    OPENGL_FUNCTION(void, glStencilOpSeparate, GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass) \
    OPENGL_FUNCTION(void, glStencilFuncSeparate, GLenum face, GLenum func, GLint ref, GLuint mask) \
    OPENGL_FUNCTION(void, glStencilMaskSeparate, GLenum face, GLuint mask) \
    OPENGL_FUNCTION(void, glAttachShader, GLuint program, GLuint shader) \
    OPENGL_FUNCTION(void, glBindAttribLocation, GLuint program, GLuint index, const GLchar *name) \
    OPENGL_FUNCTION(void, glCompileShader, GLuint shader) \
    OPENGL_FUNCTION(GLuint, glCreateProgram, void) \
    OPENGL_FUNCTION(GLuint, glCreateShader, GLenum type) \
    OPENGL_FUNCTION(void, glDeleteProgram, GLuint program) \
    OPENGL_FUNCTION(void, glDeleteShader, GLuint shader) \
    OPENGL_FUNCTION(void, glDetachShader, GLuint program, GLuint shader) \
    OPENGL_FUNCTION(void, glDisableVertexAttribArray, GLuint index) \
    OPENGL_FUNCTION(void, glEnableVertexAttribArray, GLuint index) \
    OPENGL_FUNCTION(void, glGetActiveAttrib, GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name) \
    OPENGL_FUNCTION(void, glGetActiveUniform, GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name) \
    OPENGL_FUNCTION(void, glGetAttachedShaders, GLuint program, GLsizei maxCount, GLsizei *count, GLuint *obj) \
    OPENGL_FUNCTION(GLint, glGetAttribLocation, GLuint program, const GLchar *name) \
    OPENGL_FUNCTION(void, glGetProgramiv, GLuint program, GLenum pname, GLint *params) \
    OPENGL_FUNCTION(void, glGetProgramInfoLog, GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog) \
    OPENGL_FUNCTION(void, glGetShaderiv, GLuint shader, GLenum pname, GLint *params) \
    OPENGL_FUNCTION(void, glGetShaderInfoLog, GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog) \
    OPENGL_FUNCTION(void, glGetShaderSource, GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source) \
    OPENGL_FUNCTION(GLint, glGetUniformLocation, GLuint program, const GLchar *name) \
    OPENGL_FUNCTION(void, glGetUniformfv, GLuint program, GLint location, GLfloat *params) \
    OPENGL_FUNCTION(void, glGetUniformiv, GLuint program, GLint location, GLint *params) \
    OPENGL_FUNCTION(void, glGetVertexAttribfv, GLuint index, GLenum pname, GLfloat *params) \
    OPENGL_FUNCTION(void, glGetVertexAttribiv, GLuint index, GLenum pname, GLint *params) \
    OPENGL_FUNCTION(void, glGetVertexAttribPointerv, GLuint index, GLenum pname, GLvoid* *pointer) \
    OPENGL_FUNCTION(GLboolean, glIsProgram, GLuint program) \
    OPENGL_FUNCTION(GLboolean, glIsShader, GLuint shader) \
    OPENGL_FUNCTION(void, glLinkProgram, GLuint program) \
    OPENGL_FUNCTION(void, glShaderSource, GLuint shader, GLsizei count, const GLchar* const *string, const GLint *length) \
    OPENGL_FUNCTION(void, glUseProgram, GLuint program) \
    OPENGL_FUNCTION(void, glUniform1f, GLint location, GLfloat v0) \
    OPENGL_FUNCTION(void, glUniform2f, GLint location, GLfloat v0, GLfloat v1) \
    OPENGL_FUNCTION(void, glUniform3f, GLint location, GLfloat v0, GLfloat v1, GLfloat v2) \
    OPENGL_FUNCTION(void, glUniform4f, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) \
    OPENGL_FUNCTION(void, glUniform1i, GLint location, GLint v0) \
    OPENGL_FUNCTION(void, glUniform2i, GLint location, GLint v0, GLint v1) \
    OPENGL_FUNCTION(void, glUniform3i, GLint location, GLint v0, GLint v1, GLint v2) \
    OPENGL_FUNCTION(void, glUniform4i, GLint location, GLint v0, GLint v1, GLint v2, GLint v3) \
    OPENGL_FUNCTION(void, glUniform1fv, GLint location, GLsizei count, const GLfloat *value) \
    OPENGL_FUNCTION(void, glUniform2fv, GLint location, GLsizei count, const GLfloat *value) \
    OPENGL_FUNCTION(void, glUniform3fv, GLint location, GLsizei count, const GLfloat *value) \
    OPENGL_FUNCTION(void, glUniform4fv, GLint location, GLsizei count, const GLfloat *value) \
    OPENGL_FUNCTION(void, glUniform1iv, GLint location, GLsizei count, const GLint *value) \
    OPENGL_FUNCTION(void, glUniform2iv, GLint location, GLsizei count, const GLint *value) \
    OPENGL_FUNCTION(void, glUniform3iv, GLint location, GLsizei count, const GLint *value) \
    OPENGL_FUNCTION(void, glUniform4iv, GLint location, GLsizei count, const GLint *value) \
    OPENGL_FUNCTION(void, glUniformMatrix2fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) \
    OPENGL_FUNCTION(void, glUniformMatrix3fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) \
    OPENGL_FUNCTION(void, glUniformMatrix4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) \
    OPENGL_FUNCTION(void, glValidateProgram, GLuint program) \
    OPENGL_FUNCTION(void, glVertexAttrib1f, GLuint index, GLfloat x) \
    OPENGL_FUNCTION(void, glVertexAttrib1fv, GLuint index, const GLfloat *v) \
    OPENGL_FUNCTION(void, glVertexAttrib2f, GLuint index, GLfloat x, GLfloat y) \
    OPENGL_FUNCTION(void, glVertexAttrib2fv, GLuint index, const GLfloat *v) \
    OPENGL_FUNCTION(void, glVertexAttrib3f, GLuint index, GLfloat x, GLfloat y, GLfloat z) \
    OPENGL_FUNCTION(void, glVertexAttrib3fv, GLuint index, const GLfloat *v) \
    OPENGL_FUNCTION(void, glVertexAttrib4f, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w) \
    OPENGL_FUNCTION(void, glVertexAttrib4fv, GLuint index, const GLfloat *v) \
    OPENGL_FUNCTION(void, glVertexAttribPointer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer)
    /*NO ES
    OPENGL_FUNCTION(void, glGetVertexAttribdv, GLuint index, GLenum pname, GLdouble *params) \
    OPENGL_FUNCTION(void, glVertexAttrib1d, GLuint index, GLdouble x) \
    OPENGL_FUNCTION(void, glVertexAttrib1dv, GLuint index, const GLdouble *v) \
    OPENGL_FUNCTION(void, glVertexAttrib1s, GLuint index, GLshort x) \
    OPENGL_FUNCTION(void, glVertexAttrib1sv, GLuint index, const GLshort *v) \
    OPENGL_FUNCTION(void, glVertexAttrib2d, GLuint index, GLdouble x, GLdouble y) \
    OPENGL_FUNCTION(void, glVertexAttrib2dv, GLuint index, const GLdouble *v) \
    OPENGL_FUNCTION(void, glVertexAttrib2s, GLuint index, GLshort x, GLshort y) \
    OPENGL_FUNCTION(void, glVertexAttrib2sv, GLuint index, const GLshort *v) \
    OPENGL_FUNCTION(void, glVertexAttrib3d, GLuint index, GLdouble x, GLdouble y, GLdouble z) \
    OPENGL_FUNCTION(void, glVertexAttrib3dv, GLuint index, const GLdouble *v) \
    OPENGL_FUNCTION(void, glVertexAttrib3s, GLuint index, GLshort x, GLshort y, GLshort z) \
    OPENGL_FUNCTION(void, glVertexAttrib3sv, GLuint index, const GLshort *v) \
    OPENGL_FUNCTION(void, glVertexAttrib4Nbv, GLuint index, const GLbyte *v) \
    OPENGL_FUNCTION(void, glVertexAttrib4Niv, GLuint index, const GLint *v) \
    OPENGL_FUNCTION(void, glVertexAttrib4Nsv, GLuint index, const GLshort *v) \
    OPENGL_FUNCTION(void, glVertexAttrib4Nub, GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w) \
    OPENGL_FUNCTION(void, glVertexAttrib4Nubv, GLuint index, const GLubyte *v) \
    OPENGL_FUNCTION(void, glVertexAttrib4Nuiv, GLuint index, const GLuint *v) \
    OPENGL_FUNCTION(void, glVertexAttrib4Nusv, GLuint index, const GLushort *v) \
    OPENGL_FUNCTION(void, glVertexAttrib4bv, GLuint index, const GLbyte *v) \
    OPENGL_FUNCTION(void, glVertexAttrib4d, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w) \
    OPENGL_FUNCTION(void, glVertexAttrib4dv, GLuint index, const GLdouble *v) \
    OPENGL_FUNCTION(void, glVertexAttrib4iv, GLuint index, const GLint *v) \
    OPENGL_FUNCTION(void, glVertexAttrib4s, GLuint index, GLshort x, GLshort y, GLshort z, GLshort w) \
    OPENGL_FUNCTION(void, glVertexAttrib4sv, GLuint index, const GLshort *v) \
    OPENGL_FUNCTION(void, glVertexAttrib4ubv, GLuint index, const GLubyte *v) \
    OPENGL_FUNCTION(void, glVertexAttrib4uiv, GLuint index, const GLuint *v) \
    OPENGL_FUNCTION(void, glVertexAttrib4usv, GLuint index, const GLushort *v) \
     */

#define GL_FUNCTION_LIST_21 \
    OPENGL_FUNCTION(void, glUniformMatrix2x3fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) \
    OPENGL_FUNCTION(void, glUniformMatrix3x2fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) \
    OPENGL_FUNCTION(void, glUniformMatrix2x4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) \
    OPENGL_FUNCTION(void, glUniformMatrix4x2fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) \
    OPENGL_FUNCTION(void, glUniformMatrix3x4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) \
    OPENGL_FUNCTION(void, glUniformMatrix4x3fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
/* NO ES

*/

#define GL_FUNCTION_LIST_30 \
    OPENGL_FUNCTION(void, glGetIntegeri_v, GLenum target, GLuint index, GLint *data) \
    OPENGL_FUNCTION(void, glBeginTransformFeedback, GLenum primitiveMode) \
    OPENGL_FUNCTION(void, glEndTransformFeedback, void) \
    OPENGL_FUNCTION(void, glBindBufferRange, GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size) \
    OPENGL_FUNCTION(void, glBindBufferBase, GLenum target, GLuint index, GLuint buffer) \
    OPENGL_FUNCTION(void, glTransformFeedbackVaryings, GLuint program, GLsizei count, const GLchar* const *varyings, GLenum bufferMode) \
    OPENGL_FUNCTION(void, glGetTransformFeedbackVarying, GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name) \
    OPENGL_FUNCTION(void, glVertexAttribIPointer, GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer) \
    OPENGL_FUNCTION(void, glGetVertexAttribIiv, GLuint index, GLenum pname, GLint *params) \
    OPENGL_FUNCTION(void, glGetVertexAttribIuiv, GLuint index, GLenum pname, GLuint *params) \
    OPENGL_FUNCTION(void, glVertexAttribI4i, GLuint index, GLint x, GLint y, GLint z, GLint w) \
    OPENGL_FUNCTION(void, glVertexAttribI4ui, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w) \
    OPENGL_FUNCTION(void, glVertexAttribI4iv, GLuint index, const GLint *v) \
    OPENGL_FUNCTION(void, glVertexAttribI4uiv, GLuint index, const GLuint *v) \
    OPENGL_FUNCTION(void, glGetUniformuiv, GLuint program, GLint location, GLuint *params) \
    OPENGL_FUNCTION(GLint, glGetFragDataLocation, GLuint program, const GLchar *name) \
    OPENGL_FUNCTION(void, glUniform1ui, GLint location, GLuint v0) \
    OPENGL_FUNCTION(void, glUniform2ui, GLint location, GLuint v0, GLuint v1) \
    OPENGL_FUNCTION(void, glUniform3ui, GLint location, GLuint v0, GLuint v1, GLuint v2) \
    OPENGL_FUNCTION(void, glUniform4ui, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3) \
    OPENGL_FUNCTION(void, glUniform1uiv, GLint location, GLsizei count, const GLuint *value) \
    OPENGL_FUNCTION(void, glUniform2uiv, GLint location, GLsizei count, const GLuint *value) \
    OPENGL_FUNCTION(void, glUniform3uiv, GLint location, GLsizei count, const GLuint *value) \
    OPENGL_FUNCTION(void, glUniform4uiv, GLint location, GLsizei count, const GLuint *value) \
    OPENGL_FUNCTION(void, glClearBufferiv, GLenum buffer, GLint drawbuffer, const GLint *value) \
    OPENGL_FUNCTION(void, glClearBufferuiv, GLenum buffer, GLint drawbuffer, const GLuint *value) \
    OPENGL_FUNCTION(void, glClearBufferfv, GLenum buffer, GLint drawbuffer, const GLfloat *value) \
    OPENGL_FUNCTION(void, glClearBufferfi, GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil) \
    OPENGL_FUNCTION(const GLubyte *, glGetStringi, GLenum name, GLuint index) \
    \
    OPENGL_FUNCTION(GLboolean, glIsRenderbuffer, GLuint renderbuffer) \
    OPENGL_FUNCTION(void, glBindRenderbuffer, GLenum target, GLuint renderbuffer)\
    OPENGL_FUNCTION(void, glDeleteRenderbuffers, GLsizei n, const GLuint *renderbuffers)\
    OPENGL_FUNCTION(void, glGenRenderbuffers, GLsizei n, GLuint *renderbuffers)\
    OPENGL_FUNCTION(void, glRenderbufferStorage,GLenum target, GLenum internalformat, GLsizei width, GLsizei height)  \
    OPENGL_FUNCTION(void, glGetRenderbufferParameteriv,GLenum target, GLenum pname, GLint *params)  \
    OPENGL_FUNCTION(GLboolean, glIsFramebuffer,GLuint framebuffer)  \
    OPENGL_FUNCTION(void, glBindFramebuffer,GLenum target, GLuint framebuffer)  \
    OPENGL_FUNCTION(void, glDeleteFramebuffers,GLsizei n, const GLuint *framebuffers)  \
    OPENGL_FUNCTION(void, glGenFramebuffers,GLsizei n, GLuint *framebuffers)  \
    OPENGL_FUNCTION(GLenum, glCheckFramebufferStatus,GLenum target)  \
    OPENGL_FUNCTION(void, glFramebufferRenderbuffer,GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)  \
    OPENGL_FUNCTION(void, glGetFramebufferAttachmentParameteriv,GLenum target, GLenum attachment, GLenum pname, GLint *params)  \
    OPENGL_FUNCTION(void, glGenerateMipmap,GLenum target)  \
    \
    OPENGL_FUNCTION(void, glBindVertexArray, GLuint array)\
    OPENGL_FUNCTION(void, glDeleteVertexArrays, GLsizei n, const GLuint *arrays)\
    OPENGL_FUNCTION(void, glGenVertexArrays, GLsizei n, GLuint *arrays)\
    OPENGL_FUNCTION(GLboolean, glIsVertexArray, GLuint array)
/* NO ES
    OPENGL_FUNCTION(void, glColorMaski, GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a) \
    OPENGL_FUNCTION(void, glGetBooleani_v, GLenum target, GLuint index, GLboolean *data) \
    OPENGL_FUNCTION(void, glEnablei, GLenum target, GLuint index) \
    OPENGL_FUNCTION(void, glDisablei, GLenum target, GLuint index) \
    OPENGL_FUNCTION(GLboolean, glIsEnabledi, GLenum target, GLuint index) \
    OPENGL_FUNCTION(void, glClampColor, GLenum target, GLenum clamp) \
    OPENGL_FUNCTION(void, glBeginConditionalRender, GLuint id, GLenum mode) \
    OPENGL_FUNCTION(void, glEndConditionalRender, void) \
    OPENGL_FUNCTION(void, glVertexAttribI1i, GLuint index, GLint x) \
    OPENGL_FUNCTION(void, glVertexAttribI2i, GLuint index, GLint x, GLint y) \
    OPENGL_FUNCTION(void, glVertexAttribI3i, GLuint index, GLint x, GLint y, GLint z) \
    OPENGL_FUNCTION(void, glVertexAttribI1ui, GLuint index, GLuint x) \
    OPENGL_FUNCTION(void, glVertexAttribI2ui, GLuint index, GLuint x, GLuint y) \
    OPENGL_FUNCTION(void, glVertexAttribI3ui, GLuint index, GLuint x, GLuint y, GLuint z) \
    OPENGL_FUNCTION(void, glVertexAttribI1iv, GLuint index, const GLint *v) \
    OPENGL_FUNCTION(void, glVertexAttribI2iv, GLuint index, const GLint *v) \
    OPENGL_FUNCTION(void, glVertexAttribI3iv, GLuint index, const GLint *v) \
    OPENGL_FUNCTION(void, glVertexAttribI1uiv, GLuint index, const GLuint *v) \
    OPENGL_FUNCTION(void, glVertexAttribI2uiv, GLuint index, const GLuint *v) \
    OPENGL_FUNCTION(void, glVertexAttribI3uiv, GLuint index, const GLuint *v) \
    OPENGL_FUNCTION(void, glVertexAttribI4bv, GLuint index, const GLbyte *v) \
    OPENGL_FUNCTION(void, glVertexAttribI4sv, GLuint index, const GLshort *v) \
    OPENGL_FUNCTION(void, glVertexAttribI4ubv, GLuint index, const GLubyte *v) \
    OPENGL_FUNCTION(void, glVertexAttribI4usv, GLuint index, const GLushort *v) \
    OPENGL_FUNCTION(void, glBindFragDataLocation, GLuint program, GLuint color, const GLchar *name) \
    OPENGL_FUNCTION(void, glTexParameterIiv, GLenum target, GLenum pname, const GLint *params) \
    OPENGL_FUNCTION(void, glTexParameterIuiv, GLenum target, GLenum pname, const GLuint *params) \
    OPENGL_FUNCTION(void, glGetTexParameterIiv, GLenum target, GLenum pname, GLint *params) \
    OPENGL_FUNCTION(void, glGetTexParameterIuiv, GLenum target, GLenum pname, GLuint *params) \
*/

#define GL_FUNCTION_LIST_31 \
    OPENGL_FUNCTION(void, glDrawArraysInstanced, GLenum mode, GLint first, GLsizei count, GLsizei instancecount) \
    OPENGL_FUNCTION(void, glDrawElementsInstanced, GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei instancecount)
    /* NO ES
    OPENGL_FUNCTION(void, glTexBuffer, GLenum target, GLenum internalformat, GLuint buffer) \
    OPENGL_FUNCTION(void, glPrimitiveRestartIndex, GLuint index) \
    */

#define GL_FUNCTION_LIST_32 \
    OPENGL_FUNCTION(void, glGetInteger64i_v, GLenum target, GLuint index, GLint64 *data) \
    OPENGL_FUNCTION(void, glGetBufferParameteri64v, GLenum target, GLenum pname, GLint64 *params) \
    OPENGL_FUNCTION(void, glFramebufferTexture2D,GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
    /* NO ES
    OPENGL_FUNCTION(void, glFramebufferTexture, GLenum target, GLenum attachment, GLuint texture, GLint level) \
    OPENGL_FUNCTION(void, glFramebufferTexture1D,GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)  \
    OPENGL_FUNCTION(void, glFramebufferTexture3D,GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset) \
    */

#define GL_FUNCTION_LIST_33\
    OPENGL_FUNCTION(void, glVertexAttribDivisor, GLuint index, GLuint divisor) \

#define GL_FUNCTION_LIST_ES_20 \
    OPENGL_FUNCTION(void, glActiveTexture, GLenum texture)\
    OPENGL_FUNCTION(void, glAttachShader, GLuint program, GLuint shader)\
    OPENGL_FUNCTION(void, glBindAttribLocation, GLuint program, GLuint index, const GLchar* name)\
    OPENGL_FUNCTION(void, glBindBuffer, GLenum target, GLuint buffer)\
    OPENGL_FUNCTION(void, glBindFramebuffer, GLenum target, GLuint framebuffer) \
    OPENGL_FUNCTION(void, glBindRenderbuffer, GLenum target, GLuint renderbuffer)\
    OPENGL_FUNCTION(void, glBindTexture, GLenum target, GLuint texture)\
    OPENGL_FUNCTION(void, glBlendColor, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)\
    OPENGL_FUNCTION(void, glBlendEquation, GLenum mode)\
    OPENGL_FUNCTION(void, glBlendEquationSeparate, GLenum modeRGB, GLenum modeAlpha)\
    OPENGL_FUNCTION( void, glBlendFunc, GLenum sfactor, GLenum dfactor)\
    OPENGL_FUNCTION( void, glBlendFuncSeparate, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha)\
    OPENGL_FUNCTION( void, glBufferData, GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage)\
    OPENGL_FUNCTION( void, glBufferSubData, GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data)\
    OPENGL_FUNCTION( GLenum, glCheckFramebufferStatus, GLenum target)\
    OPENGL_FUNCTION( void, glClear, GLbitfield mask) \
    OPENGL_FUNCTION( void, glClearColor, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) \
    OPENGL_FUNCTION( void, glClearDepthf, GLfloat depth) \
    OPENGL_FUNCTION( void, glClearStencil, GLint s) \
    OPENGL_FUNCTION( void, glColorMask, GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha) \
    OPENGL_FUNCTION( void, glCompileShader, GLuint shader) \
    OPENGL_FUNCTION( void, glCompressedTexImage2D, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid* data) \
    OPENGL_FUNCTION( void, glCompressedTexSubImage2D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid* data) \
    OPENGL_FUNCTION( void, glCopyTexImage2D, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border) \
    OPENGL_FUNCTION( void, glCopyTexSubImage2D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height) \
    OPENGL_FUNCTION( GLuint, glCreateProgram, void) \
    OPENGL_FUNCTION( GLuint, glCreateShader, GLenum type) \
    OPENGL_FUNCTION( void, glCullFace, GLenum mode) \
    OPENGL_FUNCTION( void, glDeleteBuffers, GLsizei n, const GLuint* buffers) \
    OPENGL_FUNCTION( void, glDeleteFramebuffers, GLsizei n, const GLuint* framebuffers) \
    OPENGL_FUNCTION( void, glDeleteProgram, GLuint program) \
    OPENGL_FUNCTION( void, glDeleteRenderbuffers, GLsizei n, const GLuint* renderbuffers) \
    OPENGL_FUNCTION( void, glDeleteShader, GLuint shader) \
    OPENGL_FUNCTION( void, glDeleteTextures, GLsizei n, const GLuint* textures) \
    OPENGL_FUNCTION( void, glDepthFunc, GLenum func) \
    OPENGL_FUNCTION( void, glDepthMask, GLboolean flag) \
    OPENGL_FUNCTION( void, glDepthRangef, GLfloat n, GLfloat f) \
    OPENGL_FUNCTION( void, glDetachShader, GLuint program, GLuint shader) \
    OPENGL_FUNCTION( void, glDisable, GLenum cap) \
    OPENGL_FUNCTION( void, glDisableVertexAttribArray, GLuint index) \
    OPENGL_FUNCTION( void, glDrawArrays, GLenum mode, GLint first, GLsizei count) \
    OPENGL_FUNCTION( void, glDrawElements, GLenum mode, GLsizei count, GLenum type, const GLvoid* indices) \
    OPENGL_FUNCTION( void, glEnable, GLenum cap) \
    OPENGL_FUNCTION( void, glEnableVertexAttribArray, GLuint index) \
    OPENGL_FUNCTION( void, glFinish, void) \
    OPENGL_FUNCTION( void, glFlush, void) \
    OPENGL_FUNCTION( void, glFramebufferRenderbuffer, GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) \
    OPENGL_FUNCTION( void, glFramebufferTexture2D, GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) \
    OPENGL_FUNCTION( void, glFrontFace, GLenum mode) \
    OPENGL_FUNCTION( void, glGenBuffers, GLsizei n, GLuint* buffers) \
    OPENGL_FUNCTION( void, glGenerateMipmap, GLenum target) \
    OPENGL_FUNCTION( void, glGenFramebuffers, GLsizei n, GLuint* framebuffers) \
    OPENGL_FUNCTION( void, glGenRenderbuffers, GLsizei n, GLuint* renderbuffers) \
    OPENGL_FUNCTION( void, glGenTextures, GLsizei n, GLuint* textures) \
    OPENGL_FUNCTION( void, glGetActiveAttrib, GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, GLchar* name) \
    OPENGL_FUNCTION( void, glGetActiveUniform, GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, GLchar* name) \
    OPENGL_FUNCTION( void, glGetAttachedShaders, GLuint program, GLsizei maxcount, GLsizei* count, GLuint* shaders) \
    OPENGL_FUNCTION( GLint, glGetAttribLocation, GLuint program, const GLchar* name) \
    OPENGL_FUNCTION( void, glGetBooleanv, GLenum pname, GLboolean* params) \
    OPENGL_FUNCTION( void, glGetBufferParameteriv, GLenum target, GLenum pname, GLint* params) \
    OPENGL_FUNCTION( GLenum, glGetError, void) \
    OPENGL_FUNCTION( void, glGetFloatv, GLenum pname, GLfloat* params) \
    OPENGL_FUNCTION( void, glGetFramebufferAttachmentParameteriv, GLenum target, GLenum attachment, GLenum pname, GLint* params) \
    OPENGL_FUNCTION( void, glGetIntegerv, GLenum pname, GLint* params) \
    OPENGL_FUNCTION( void, glGetProgramiv, GLuint program, GLenum pname, GLint* params) \
    OPENGL_FUNCTION( void, glGetProgramInfoLog, GLuint program, GLsizei bufsize, GLsizei* length, GLchar* infolog) \
    OPENGL_FUNCTION( void, glGetRenderbufferParameteriv, GLenum target, GLenum pname, GLint* params) \
    OPENGL_FUNCTION( void, glGetShaderiv, GLuint shader, GLenum pname, GLint* params) \
    OPENGL_FUNCTION( void, glGetShaderInfoLog, GLuint shader, GLsizei bufsize, GLsizei* length, GLchar* infolog) \
    OPENGL_FUNCTION( void, glGetShaderPrecisionFormat, GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision) \
    OPENGL_FUNCTION( void, glGetShaderSource, GLuint shader, GLsizei bufsize, GLsizei* length, GLchar* source) \
    OPENGL_FUNCTION( const GLubyte*, glGetString, GLenum name) \
    OPENGL_FUNCTION( void, glGetTexParameterfv, GLenum target, GLenum pname, GLfloat* params) \
    OPENGL_FUNCTION( void, glGetTexParameteriv, GLenum target, GLenum pname, GLint* params) \
    OPENGL_FUNCTION( void, glGetUniformfv, GLuint program, GLint location, GLfloat* params) \
    OPENGL_FUNCTION( void, glGetUniformiv, GLuint program, GLint location, GLint* params) \
    OPENGL_FUNCTION( GLint, glGetUniformLocation, GLuint program, const GLchar* name) \
    OPENGL_FUNCTION( void, glGetVertexAttribfv, GLuint index, GLenum pname, GLfloat* params) \
    OPENGL_FUNCTION( void, glGetVertexAttribiv, GLuint index, GLenum pname, GLint* params) \
    OPENGL_FUNCTION( void, glGetVertexAttribPointerv, GLuint index, GLenum pname, GLvoid** pointer) \
    OPENGL_FUNCTION( void, glHint, GLenum target, GLenum mode) \
    OPENGL_FUNCTION( GLboolean, glIsBuffer, GLuint buffer) \
    OPENGL_FUNCTION( GLboolean, glIsEnabled, GLenum cap) \
    OPENGL_FUNCTION( GLboolean, glIsFramebuffer, GLuint framebuffer) \
    OPENGL_FUNCTION( GLboolean, glIsProgram, GLuint program) \
    OPENGL_FUNCTION( GLboolean, glIsRenderbuffer, GLuint renderbuffer) \
    OPENGL_FUNCTION( GLboolean, glIsShader, GLuint shader) \
    OPENGL_FUNCTION( GLboolean, glIsTexture, GLuint texture) \
    OPENGL_FUNCTION( void, glLineWidth, GLfloat width) \
    OPENGL_FUNCTION( void, glLinkProgram, GLuint program) \
    OPENGL_FUNCTION( void, glPixelStorei, GLenum pname, GLint param) \
    OPENGL_FUNCTION( void, glPolygonOffset, GLfloat factor, GLfloat units) \
    OPENGL_FUNCTION( void, glReadPixels, GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* pixels) \
    OPENGL_FUNCTION( void, glReleaseShaderCompiler, void) \
    OPENGL_FUNCTION( void, glRenderbufferStorage, GLenum target, GLenum internalformat, GLsizei width, GLsizei height) \
    OPENGL_FUNCTION( void, glSampleCoverage, GLfloat value, GLboolean invert) \
    OPENGL_FUNCTION( void, glScissor, GLint x, GLint y, GLsizei width, GLsizei height) \
    OPENGL_FUNCTION( void, glShaderBinary, GLsizei n, const GLuint* shaders, GLenum binaryformat, const GLvoid* binary, GLsizei length) \
    OPENGL_FUNCTION( void, glShaderSource, GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length) \
    OPENGL_FUNCTION( void, glStencilFunc, GLenum func, GLint ref, GLuint mask) \
    OPENGL_FUNCTION( void, glStencilFuncSeparate, GLenum face, GLenum func, GLint ref, GLuint mask) \
    OPENGL_FUNCTION( void, glStencilMask, GLuint mask) \
    OPENGL_FUNCTION( void, glStencilMaskSeparate, GLenum face, GLuint mask) \
    OPENGL_FUNCTION( void, glStencilOp, GLenum fail, GLenum zfail, GLenum zpass) \
    OPENGL_FUNCTION( void, glStencilOpSeparate, GLenum face, GLenum fail, GLenum zfail, GLenum zpass) \
    OPENGL_FUNCTION( void, glTexImage2D, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels) \
    OPENGL_FUNCTION( void, glTexParameterf, GLenum target, GLenum pname, GLfloat param) \
    OPENGL_FUNCTION( void, glTexParameterfv, GLenum target, GLenum pname, const GLfloat* params) \
    OPENGL_FUNCTION( void, glTexParameteri, GLenum target, GLenum pname, GLint param) \
    OPENGL_FUNCTION( void, glTexParameteriv, GLenum target, GLenum pname, const GLint* params) \
    OPENGL_FUNCTION( void, glTexSubImage2D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels) \
    OPENGL_FUNCTION( void, glUniform1f, GLint location, GLfloat x) \
    OPENGL_FUNCTION( void, glUniform1fv, GLint location, GLsizei count, const GLfloat* v) \
    OPENGL_FUNCTION( void, glUniform1i, GLint location, GLint x) \
    OPENGL_FUNCTION( void, glUniform1iv, GLint location, GLsizei count, const GLint* v) \
    OPENGL_FUNCTION( void, glUniform2f, GLint location, GLfloat x, GLfloat y) \
    OPENGL_FUNCTION( void, glUniform2fv, GLint location, GLsizei count, const GLfloat* v) \
    OPENGL_FUNCTION( void, glUniform2i, GLint location, GLint x, GLint y) \
    OPENGL_FUNCTION( void, glUniform2iv, GLint location, GLsizei count, const GLint* v) \
    OPENGL_FUNCTION( void, glUniform3f, GLint location, GLfloat x, GLfloat y, GLfloat z) \
    OPENGL_FUNCTION( void, glUniform3fv, GLint location, GLsizei count, const GLfloat* v) \
    OPENGL_FUNCTION( void, glUniform3i, GLint location, GLint x, GLint y, GLint z) \
    OPENGL_FUNCTION( void, glUniform3iv, GLint location, GLsizei count, const GLint* v) \
    OPENGL_FUNCTION( void, glUniform4f, GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w) \
    OPENGL_FUNCTION( void, glUniform4fv, GLint location, GLsizei count, const GLfloat* v) \
    OPENGL_FUNCTION( void, glUniform4i, GLint location, GLint x, GLint y, GLint z, GLint w) \
    OPENGL_FUNCTION( void, glUniform4iv, GLint location, GLsizei count, const GLint* v) \
    OPENGL_FUNCTION( void, glUniformMatrix2fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) \
    OPENGL_FUNCTION( void, glUniformMatrix3fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) \
    OPENGL_FUNCTION( void, glUniformMatrix4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) \
    OPENGL_FUNCTION( void, glUseProgram, GLuint program) \
    OPENGL_FUNCTION( void, glValidateProgram, GLuint program) \
    OPENGL_FUNCTION( void, glVertexAttrib1f, GLuint indx, GLfloat x) \
    OPENGL_FUNCTION( void, glVertexAttrib1fv, GLuint indx, const GLfloat* values) \
    OPENGL_FUNCTION( void, glVertexAttrib2f, GLuint indx, GLfloat x, GLfloat y) \
    OPENGL_FUNCTION( void, glVertexAttrib2fv, GLuint indx, const GLfloat* values) \
    OPENGL_FUNCTION( void, glVertexAttrib3f, GLuint indx, GLfloat x, GLfloat y, GLfloat z) \
    OPENGL_FUNCTION( void, glVertexAttrib3fv, GLuint indx, const GLfloat* values) \
    OPENGL_FUNCTION( void, glVertexAttrib4f, GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w) \
    OPENGL_FUNCTION( void, glVertexAttrib4fv, GLuint indx, const GLfloat* values) \
    OPENGL_FUNCTION( void, glVertexAttribPointer, GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* ptr) \
    OPENGL_FUNCTION( void, glViewport, GLint x, GLint y, GLsizei width, GLsizei height)

/* OpenGL ES 3.0 */
#define GL_FUNCTION_LIST_ES_30 \
    OPENGL_FUNCTION( void, glReadBuffer, GLenum mode) \
    OPENGL_FUNCTION( void, glDrawRangeElements, GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid* indices) \
    OPENGL_FUNCTION( void, glTexImage3D, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid* pixels) \
    OPENGL_FUNCTION( void, glTexSubImage3D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid* pixels) \
    OPENGL_FUNCTION( void, glCopyTexSubImage3D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height) \
    OPENGL_FUNCTION( void, glCompressedTexImage3D, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid* data) \
    OPENGL_FUNCTION( void, glCompressedTexSubImage3D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid* data) \
    OPENGL_FUNCTION( void, glGenQueries, GLsizei n, GLuint* ids) \
    OPENGL_FUNCTION( void, glDeleteQueries, GLsizei n, const GLuint* ids) \
    OPENGL_FUNCTION( GLboolean, glIsQuery, GLuint id) \
    OPENGL_FUNCTION( void, glBeginQuery, GLenum target, GLuint id) \
    OPENGL_FUNCTION( void, glEndQuery, GLenum target) \
    OPENGL_FUNCTION( void, glGetQueryiv, GLenum target, GLenum pname, GLint* params) \
    OPENGL_FUNCTION( void, glGetQueryObjectuiv, GLuint id, GLenum pname, GLuint* params) \
    OPENGL_FUNCTION( GLboolean, glUnmapBuffer, GLenum target) \
    OPENGL_FUNCTION( void, glGetBufferPointerv, GLenum target, GLenum pname, GLvoid** params) \
    OPENGL_FUNCTION( void, glDrawBuffers, GLsizei n, const GLenum* bufs) \
    OPENGL_FUNCTION( void, glUniformMatrix2x3fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) \
    OPENGL_FUNCTION( void, glUniformMatrix3x2fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) \
    OPENGL_FUNCTION( void, glUniformMatrix2x4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) \
    OPENGL_FUNCTION( void, glUniformMatrix4x2fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) \
    OPENGL_FUNCTION( void, glUniformMatrix3x4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) \
    OPENGL_FUNCTION( void, glUniformMatrix4x3fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) \
    OPENGL_FUNCTION( void, glBlitFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter) \
    OPENGL_FUNCTION( void, glRenderbufferStorageMultisample, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height) \
    OPENGL_FUNCTION( void, glFramebufferTextureLayer, GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer) \
    OPENGL_FUNCTION( GLvoid*, glMapBufferRange, GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access) \
    OPENGL_FUNCTION( void, glFlushMappedBufferRange, GLenum target, GLintptr offset, GLsizeiptr length) \
    OPENGL_FUNCTION( void, glBindVertexArray, GLuint array) \
    OPENGL_FUNCTION( void, glDeleteVertexArrays, GLsizei n, const GLuint* arrays) \
    OPENGL_FUNCTION( void, glGenVertexArrays, GLsizei n, GLuint* arrays) \
    OPENGL_FUNCTION( GLboolean, glIsVertexArray, GLuint array) \
    OPENGL_FUNCTION( void, glGetIntegeri_v, GLenum target, GLuint index, GLint* data) \
    OPENGL_FUNCTION( void, glBeginTransformFeedback, GLenum primitiveMode) \
    OPENGL_FUNCTION( void, glEndTransformFeedback, void) \
    OPENGL_FUNCTION( void, glBindBufferRange, GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size) \
    OPENGL_FUNCTION( void, glBindBufferBase, GLenum target, GLuint index, GLuint buffer) \
    OPENGL_FUNCTION( void, glTransformFeedbackVaryings, GLuint program, GLsizei count, const GLchar* const* varyings, GLenum bufferMode) \
    OPENGL_FUNCTION( void, glGetTransformFeedbackVarying, GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLsizei* size, GLenum* type, GLchar* name) \
    OPENGL_FUNCTION( void, glVertexAttribIPointer, GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer) \
    OPENGL_FUNCTION( void, glGetVertexAttribIiv, GLuint index, GLenum pname, GLint* params) \
    OPENGL_FUNCTION( void, glGetVertexAttribIuiv, GLuint index, GLenum pname, GLuint* params) \
    OPENGL_FUNCTION( void, glVertexAttribI4i, GLuint index, GLint x, GLint y, GLint z, GLint w) \
    OPENGL_FUNCTION( void, glVertexAttribI4ui, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w) \
    OPENGL_FUNCTION( void, glVertexAttribI4iv, GLuint index, const GLint* v) \
    OPENGL_FUNCTION( void, glVertexAttribI4uiv, GLuint index, const GLuint* v) \
    OPENGL_FUNCTION( void, glGetUniformuiv, GLuint program, GLint location, GLuint* params) \
    OPENGL_FUNCTION( GLint, glGetFragDataLocation, GLuint program, const GLchar *name) \
    OPENGL_FUNCTION( void, glUniform1ui, GLint location, GLuint v0) \
    OPENGL_FUNCTION( void, glUniform2ui, GLint location, GLuint v0, GLuint v1) \
    OPENGL_FUNCTION( void, glUniform3ui, GLint location, GLuint v0, GLuint v1, GLuint v2) \
    OPENGL_FUNCTION( void, glUniform4ui, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3) \
    OPENGL_FUNCTION( void, glUniform1uiv, GLint location, GLsizei count, const GLuint* value) \
    OPENGL_FUNCTION( void, glUniform2uiv, GLint location, GLsizei count, const GLuint* value) \
    OPENGL_FUNCTION( void, glUniform3uiv, GLint location, GLsizei count, const GLuint* value) \
    OPENGL_FUNCTION( void, glUniform4uiv, GLint location, GLsizei count, const GLuint* value) \
    OPENGL_FUNCTION( void, glClearBufferiv, GLenum buffer, GLint drawbuffer, const GLint* value) \
    OPENGL_FUNCTION( void, glClearBufferuiv, GLenum buffer, GLint drawbuffer, const GLuint* value) \
    OPENGL_FUNCTION( void, glClearBufferfv, GLenum buffer, GLint drawbuffer, const GLfloat* value) \
    OPENGL_FUNCTION( void, glClearBufferfi, GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil) \
    OPENGL_FUNCTION( const GLubyte*, glGetStringi, GLenum name, GLuint index) \
    OPENGL_FUNCTION( void, glCopyBufferSubData, GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size) \
    OPENGL_FUNCTION( void, glGetUniformIndices, GLuint program, GLsizei uniformCount, const GLchar* const* uniformNames, GLuint* uniformIndices) \
    OPENGL_FUNCTION( void, glGetActiveUniformsiv, GLuint program, GLsizei uniformCount, const GLuint* uniformIndices, GLenum pname, GLint* params) \
    OPENGL_FUNCTION( GLuint, glGetUniformBlockIndex, GLuint program, const GLchar* uniformBlockName) \
    OPENGL_FUNCTION( void, glGetActiveUniformBlockiv, GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint* params) \
    OPENGL_FUNCTION( void, glGetActiveUniformBlockName, GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei* length, GLchar* uniformBlockName) \
    OPENGL_FUNCTION( void, glUniformBlockBinding, GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding) \
    OPENGL_FUNCTION( void, glDrawArraysInstanced, GLenum mode, GLint first, GLsizei count, GLsizei instanceCount) \
    OPENGL_FUNCTION( void, glDrawElementsInstanced, GLenum mode, GLsizei count, GLenum type, const GLvoid* indices, GLsizei instanceCount) \
    OPENGL_FUNCTION( GLsync, glFenceSync, GLenum condition, GLbitfield flags) \
    OPENGL_FUNCTION( GLboolean, glIsSync, GLsync sync) \
    OPENGL_FUNCTION( void, glDeleteSync, GLsync sync) \
    OPENGL_FUNCTION( GLenum, glClientWaitSync, GLsync sync, GLbitfield flags, GLuint64 timeout) \
    OPENGL_FUNCTION( void, glWaitSync, GLsync sync, GLbitfield flags, GLuint64 timeout) \
    OPENGL_FUNCTION( void, glGetInteger64v, GLenum pname, GLint64* params) \
    OPENGL_FUNCTION( void, glGetSynciv, GLsync sync, GLenum pname, GLsizei bufSize, GLsizei* length, GLint* values) \
    OPENGL_FUNCTION( void, glGetInteger64i_v, GLenum target, GLuint index, GLint64* data) \
    OPENGL_FUNCTION( void, glGetBufferParameteri64v, GLenum target, GLenum pname, GLint64* params) \
    OPENGL_FUNCTION( void, glGenSamplers, GLsizei count, GLuint* samplers) \
    OPENGL_FUNCTION( void, glDeleteSamplers, GLsizei count, const GLuint* samplers) \
    OPENGL_FUNCTION( GLboolean, glIsSampler, GLuint sampler) \
    OPENGL_FUNCTION( void, glBindSampler, GLuint unit, GLuint sampler) \
    OPENGL_FUNCTION( void, glSamplerParameteri, GLuint sampler, GLenum pname, GLint param) \
    OPENGL_FUNCTION( void, glSamplerParameteriv, GLuint sampler, GLenum pname, const GLint* param) \
    OPENGL_FUNCTION( void, glSamplerParameterf, GLuint sampler, GLenum pname, GLfloat param) \
    OPENGL_FUNCTION( void, glSamplerParameterfv, GLuint sampler, GLenum pname, const GLfloat* param) \
    OPENGL_FUNCTION( void, glGetSamplerParameteriv, GLuint sampler, GLenum pname, GLint* params) \
    OPENGL_FUNCTION( void, glGetSamplerParameterfv, GLuint sampler, GLenum pname, GLfloat* params) \
    OPENGL_FUNCTION( void, glVertexAttribDivisor, GLuint index, GLuint divisor) \
    OPENGL_FUNCTION( void, glBindTransformFeedback, GLenum target, GLuint id) \
    OPENGL_FUNCTION( void, glDeleteTransformFeedbacks, GLsizei n, const GLuint* ids) \
    OPENGL_FUNCTION( void, glGenTransformFeedbacks, GLsizei n, GLuint* ids) \
    OPENGL_FUNCTION( GLboolean, glIsTransformFeedback, GLuint id) \
    OPENGL_FUNCTION( void, glPauseTransformFeedback, void) \
    OPENGL_FUNCTION( void, glResumeTransformFeedback, void) \
    OPENGL_FUNCTION( void, glGetProgramBinary, GLuint program, GLsizei bufSize, GLsizei* length, GLenum* binaryFormat, GLvoid* binary) \
    OPENGL_FUNCTION( void, glProgramBinary, GLuint program, GLenum binaryFormat, const GLvoid* binary, GLsizei length) \
    OPENGL_FUNCTION( void, glProgramParameteri, GLuint program, GLenum pname, GLint value) \
    OPENGL_FUNCTION( void, glInvalidateFramebuffer, GLenum target, GLsizei numAttachments, const GLenum* attachments) \
    OPENGL_FUNCTION( void, glInvalidateSubFramebuffer, GLenum target, GLsizei numAttachments, const GLenum* attachments, GLint x, GLint y, GLsizei width, GLsizei height) \
    OPENGL_FUNCTION( void, glTexStorage2D, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height) \
    OPENGL_FUNCTION( void, glTexStorage3D, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth) \
    OPENGL_FUNCTION( void, glGetInternalformativ, GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint* params)

#define GL_FUNCTION_LIST_ES_MISSING \
    OPENGL_FUNCTION( void, glDrawBuffer, GLenum mode ) \
    OPENGL_FUNCTION( void, glPolygonMode, GLenum face,  GLenum mode )

#endif // OPENGLFUNCTIONS_H
