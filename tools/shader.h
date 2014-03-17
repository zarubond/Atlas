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
#ifndef SHADER_H
#define SHADER_H

#include <QFile>
#include <QDebug>

#include <iostream>
#include "math.h"
#include "opengl.h"
#include "../world/complement/light.h"
/**
 * @brief The Shader class
 */
class Shader
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
    GLint getAttribute(const char * name);
    /**
     * @brief getUniform Get shader uniform.
     * @param name Uniform name.
     * @return Uniform location.
     */
    GLint getUniform(const char * name);

    bool  getUniformLight(LightUniform & uniform, int id);
    /**
     * @brief uniform Set uniform to shader program.
     * @param location
     * @param v0
     */
    void uniform(GLint location, const GLfloat v0);
    void uniform(GLint location, GLint v0);
    void uniform(GLint location, GLuint v0);
    void uniform(GLint location, GLfloat v0,GLfloat v1);
    void uniform(GLint location, GLfloat v0,GLfloat v1,GLfloat v2);
    void uniform(GLint location, GLfloat v0,GLfloat v1,GLfloat v2,GLfloat v3);
    void uniform(GLint location, const Vertex3f & vert) const;
    void uniform(GLint location, Vertex3f vert);
    void uniform(GLint location, const Vertex4f & vert) const;
    void uniform(GLint location, Vertex4f vert);
    void uniformMatrix(GLint location,const Matrix4f & matrix) const;
    void uniformMatrix(GLint location,const Matrix3f & matrix) const;
    void uniformLight(const LightUniform &location, const Light &light) const;
    /**
     * @brief bind Bind shader program.
     */
    void bind();
    /**
     * @brief unbind Unbind shader program.
     */
    void unbind();
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
