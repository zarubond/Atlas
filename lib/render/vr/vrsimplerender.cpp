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

#include "vrsimplerender.h"

#define OPENGL_OFFSET(i) ((char *)NULL + (i))

VrSimpleRender::VrSimpleRender():
    canvas_buffer(QOpenGLBuffer::VertexBuffer)
{
}

bool VrSimpleRender::init()
{
    screen=QGuiApplication::primaryScreen();

    const char * distortion_vsh=":shader/Distortion.vsh";
    const char * distortion_fsh=":shader/Distortion.fsh";

    program.addShaderFromSourceFile(QOpenGLShader::Vertex, distortion_vsh);
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, distortion_fsh);
    if(!program.link())
        qDebug()<<"link error";

    if((uniform_color = program.uniformLocation("color"))==-1)
       return false;

    if((attribute_coord = program.attributeLocation("coord"))==-1)
        return false;

    uniform_tex_color   = program.uniformLocation("tex_color");

    program.bind();
    program.setUniformValue(uniform_tex_color,0);
    program.release();

    static const GLfloat quad_data[] = {
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
    };

    canvas_buffer.create();
    canvas_buffer.bind();
    canvas_buffer.allocate(sizeof(quad_data));
    canvas_buffer.write(0, quad_data, sizeof(quad_data));
    canvas_buffer.release();

    return true;
}

void VrSimpleRender::createVAO(QOpenGLFunctions *f)
{
    vao.create();
    vao.bind();
    program.bind();
    canvas_buffer.bind();
    program.enableAttributeArray(attribute_coord);
    //f->glVertexAttribPointer(attribute_coord, 3, GL_FLOAT,GL_FALSE,0,0);
    vao.release();
}

void VrSimpleRender::render(QOpenGLFunctions *f, GLuint texture)
{
    if(!vao.isCreated())
        createVAO(f);

    //f->glBindTexture(GL_TEXTURE_2D, texture);
    program.bind();
    program.setUniformValue(uniform_color, QVector3D(1.0,1.0,1.0));

    vao.bind();

    //f->glDrawArrays(GL_TRIANGLES, 0, 6);
    vao.release();
    program.release();
}

