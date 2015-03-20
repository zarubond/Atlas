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

#ifndef VRSIMPLERENDER_H
#define VRSIMPLERENDER_H

#include "vrframebuffer.h"

#include <QDebug>
#include <QScreen>
#include <QGuiApplication>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

class VrSimpleRender
{
public:
    VrSimpleRender();
    void render(QOpenGLFunctions *f, GLuint texture);
    bool init();

private:

    QOpenGLShaderProgram program;
    QOpenGLBuffer canvas_buffer;
    int u_tex_color;
    int a_coord, a_texcoord, a_texoffset;
    QOpenGLVertexArrayObject vao;

    int u_texRange, u_lensCenterOffset, u_distortion, u_aspect, u_fillScale;
    QScreen * screen;
    void createVAO(QOpenGLFunctions *f);

    int uniform_color, uniform_tex_color;
    int attribute_coord;
};

#endif // VRSIMPLERENDER_H
