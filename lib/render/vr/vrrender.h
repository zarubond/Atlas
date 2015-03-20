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

#ifndef DISTORTIONRENDER_H
#define DISTORTIONRENDER_H

#include <QDebug>
#include <QScreen>
#include <QGuiApplication>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QMatrix>
#include <QtMath>

#include "vrdistortionmesh.h"
#include "cardboardvrparams.h"
#include "vrframebuffer.h"

#include "../../tools/camera.h"

class FieldOfView{

public:

    float left() const;
    void setLeft(float left);

    float right() const;
    void setRight(float right);

    float bottom() const;
    void setBottom(float bottom);

    float top() const;
    void setTop(float top);

    QMatrix4x4 toPerspectiveMatrix(float near, float far);

private:
    float mLeft;
    float mRight;
    float mBottom;
    float mTop;
};

class Viewport{
public:
    float x;
    float y;
    float width;
    float height;
    float eyeX;
    float eyeY;
};

class VrRender: public OpenGL
{
public:
    VrRender();
    void render(VrFrameBuffer * frame);
    bool init();
    void resize();
    Camera leftEye(const Camera & normal_camera);
    Camera rightEye(const Camera & normal_camera);


private:
    void createProjection();
    Viewport viewport(FieldOfView fov, float xOffsetM);
    void updateFov(const QSizeF & screen_size);
    void createVAO();

    QOpenGLShaderProgram program;
    QOpenGLVertexArrayObject left_vao, right_vao;
    VrDistortionMesh left_mesh, right_mesh;

    QScreen * screen;

    FieldOfView leftEyeFov,rightEyeFov;
    CardboardVrParams params;

    int uTextureSampler;
    int aPosition, aVignette, aTextureCoord;

    bool m_resize;
};

#endif // DISTORTIONRENDER_H
