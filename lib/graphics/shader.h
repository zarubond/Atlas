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
#ifndef SHADER_H
#define SHADER_H

#include "math.h"
#include "opengl.h"
#include "opengldebug.h"
#include "../tools/shell.h"
#include "../tools/file.h"

/**
 * @brief The Shader class
 */
class Shader: public OpenGL
{
public:
    Shader();
    /**
     * @brief load Load shader from strings.
     * @param vertex_src Vertex shader source.
     * @param fragment_src Fragment shader source.
     * @return True on success.
     */
    bool load(const char *vertex_src, const char *fragment_src);
    /**
     * @brief loadFile Load shader from files.
     * @param vertex_file Vertex shader file.
     * @param fragment_file Fragment shader file.
     * @return True on success.
     */
    bool loadFile(const char * vertex_file,const char * fragment_file);
    /**
     * @brief getAttribute Get shader attribute.
     * @param name Attribute name.
     * @return Attribute location.
     */
    Attribute getAttribute(const char * name);
    /**
     * @brief getUniform Get shader uniform.
     * @param name Uniform name.
     * @return Uniform location.
     */
    Uniform getUniform(const char * name);

    void enableAttribute(Attribute attrib);
    void disableAttribute(Attribute attrib);
    void setAttribute(GLuint index, GLint size, GLenum type, GLsizei stride=0, GLuint offset=0);
    /**
     * @brief uniform Set uniform to shader program.
     * @param location
     * @param v0
     */
    void uniform(Uniform location, const GLfloat v0);
    void uniform(Uniform location, const GLint v0);
    void uniform(Uniform location, const GLuint v0);
    void uniform(Uniform location, GLfloat v0,GLfloat v1);
    void uniform(Uniform location, GLfloat v0, GLfloat v1, GLfloat v2);
    void uniform(Uniform location, GLfloat v0,GLfloat v1,GLfloat v2,GLfloat v3);
    void uniform(Uniform location, const Vector2f & vert);
    void uniform(Uniform location, const Vector3f & vert);
    void uniform(Uniform location, const Vector4f & vert);
    void uniform(Uniform location, const ColorRGBA & color);
    void uniform(Uniform location,GLsizei count,GLint * value);
    void uniformMatrix(Uniform location, const Matrix4f & matrix);
    void uniformMatrix(Uniform location,const Matrix3f & matrix);
    /**
     * @brief bind Bind shader program.
     */
    void bind();
    /**
     * @brief unbind Unbind shader program.
     */
    void release();
    /**
     * @brief printUniforms Print all active uniforms in shadder
     */
    void printUniforms();

private:
    char * readFile(const char *file_name);
    void printLog(GLuint object);
    int printSource(const char *str, int line=1);

    GLuint program;

    GLuint createShader(const char *bytes, GLenum type);
};

#endif // SHADER_H
