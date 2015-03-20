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

#include "vrrender.h"

#define OPENGL_OFFSET(i) ((char *)NULL + (i))

#define DEG_TO_RAD 0.0174532925
#define RAD_TO_DEG 57.2957795

VrRender::VrRender()
{
    m_resize=true;
}

bool VrRender::init()
{
    screen=QGuiApplication::primaryScreen();

    const char * distortion_vsh=":lib/shader/VrRender.vsh";
    const char * distortion_fsh=":lib/shader/VrRender.fsh";

    program.addShaderFromSourceFile(QOpenGLShader::Vertex, distortion_vsh);
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, distortion_fsh);
    if(!program.link())
        qDebug()<<"link error";

    uTextureSampler = program.uniformLocation("uTextureSampler");
    aPosition       = program.attributeLocation("aPosition");
    aVignette       = program.attributeLocation("aVignette");
    aTextureCoord   = program.attributeLocation("aTextureCoord");

    program.bind();
    program.setUniformValue(uTextureSampler,0);
    program.release();

    return true;
}

void VrRender::resize()
{
    m_resize=true;
}

Camera VrRender::leftEye(const Camera &normal_camera)
{
    float halfInterpupillaryDistance=params.interpupillaryDistance()/2.0;

    QMatrix4x4 left_proj=leftEyeFov.toPerspectiveMatrix(0.1,1000);

    Camera camera=normal_camera;
    Matrix4f proj_matrix(left_proj.data());
    camera.perspective(proj_matrix);
    Vector3f tmp=camera.rotation()*Vector3f(-halfInterpupillaryDistance,0,0);
    camera.translate(tmp);

    return camera;
}

Camera VrRender::rightEye(const Camera &normal_camera)
{
    float halfInterpupillaryDistance=params.interpupillaryDistance()/2.0;

    QMatrix4x4 right_proj=rightEyeFov.toPerspectiveMatrix(0.1,1000);

    Camera camera=normal_camera;
    Matrix4f proj_matrix(right_proj.data());
    camera.perspective(proj_matrix);
    Vector3f tmp=camera.rotation()*Vector3f(halfInterpupillaryDistance,0,0);
    camera.translate(tmp);

    return camera;
}

void VrRender::createProjection()
{
    left_vao.destroy();
    right_vao.destroy();

    QSizeF screen_size;
    //convert mm to m
    screen_size.setWidth(screen->physicalSize().width()/1000.0);
    screen_size.setHeight(screen->physicalSize().height()/1000.0);

    updateFov(screen_size);

    float mBorderSizeMeters = 0.0030F;
    Viewport leftViewport=viewport(leftEyeFov,0.0);
    Viewport rightViewport=viewport(rightEyeFov,leftViewport.width);

    float screenWidthM=screen_size.width();
    float screenHeightM=screen_size.height();
    float xEyeOffsetMScreen=screenWidthM / 2.0F  - params.interpupillaryDistance() / 2.0F;
    float yEyeOffsetMScreen=params.verticalDistanceToLensCenter() - mBorderSizeMeters;

    float textureWidthM = leftViewport.width + rightViewport.width;
    float textureHeightM = std::max(leftViewport.height, rightViewport.height);

    float xEyeOffsetMTexture = leftViewport.eyeX;
    float yEyeOffsetMTexture = leftViewport.eyeY;
    float viewportXMTexture  = leftViewport.x;
    float viewportYMTexture  = leftViewport.y;
    float viewportWidthMTexture=leftViewport.width;
    float viewportHeightMTexture=leftViewport.height;

    left_mesh.create(&params, screenWidthM, screenHeightM, xEyeOffsetMScreen, yEyeOffsetMScreen,
                    textureWidthM, textureHeightM, xEyeOffsetMTexture, yEyeOffsetMTexture,
                     viewportXMTexture, viewportYMTexture, viewportWidthMTexture, viewportHeightMTexture);

    xEyeOffsetMScreen = screenWidthM - xEyeOffsetMScreen;

    xEyeOffsetMTexture = rightViewport.eyeX;
    yEyeOffsetMTexture = rightViewport.eyeY;
    viewportXMTexture=rightViewport.x;
    viewportYMTexture=rightViewport.y;
    viewportWidthMTexture=rightViewport.width;
    viewportHeightMTexture=rightViewport.height;

    right_mesh.create(&params, screenWidthM, screenHeightM, xEyeOffsetMScreen, yEyeOffsetMScreen,
                        textureWidthM, textureHeightM, xEyeOffsetMTexture, yEyeOffsetMTexture,
                      viewportXMTexture, viewportYMTexture, viewportWidthMTexture, viewportHeightMTexture);
}

Viewport VrRender::viewport(FieldOfView fov,float xOffsetM)
{
    float eyeToScreenDistanceM = params.eyeToLensDistance() + params.screenToLensDistance();
    float leftM     = (float) tan(DEG_TO_RAD*(fov.left()))   * eyeToScreenDistanceM;
    float rightM    = (float) tan(DEG_TO_RAD*(fov.right()))  * eyeToScreenDistanceM;
    float bottomM   = (float) tan(DEG_TO_RAD*(fov.bottom())) * eyeToScreenDistanceM;
    float topM      = (float) tan(DEG_TO_RAD*(fov.top()))    * eyeToScreenDistanceM;

    Viewport vp;
    vp.x = xOffsetM;
    vp.y = 0.0F;
    vp.width = (leftM + rightM);
    vp.height = (bottomM + topM);
    vp.eyeX = (leftM + xOffsetM);
    vp.eyeY = bottomM;

    return vp;
}

void VrRender::updateFov(const QSizeF & screen_size)
{
    float mBorderSizeMeters = 0.0030F;
    float idealFovAngle = (float) RAD_TO_DEG*(atan2(params.lensDiameter() / 2.0F, params.eyeToLensDistance()));

    float eyeToScreenDist = params.eyeToLensDistance() + params.screenToLensDistance();
    float outerDist = (screen_size.width() - params.interpupillaryDistance()) / 2.0F;
    float innerDist = params.interpupillaryDistance() / 2.0F;
    float bottomDist = params.verticalDistanceToLensCenter() - mBorderSizeMeters;

    float topDist = screen_size.height() + mBorderSizeMeters
            - params.verticalDistanceToLensCenter();

    float outerAngle    = (float) RAD_TO_DEG*(atan2(params.distort(outerDist), eyeToScreenDist));
    float innerAngle    = (float) RAD_TO_DEG*(atan2(params.distort(innerDist), eyeToScreenDist));
    float bottomAngle   = (float) RAD_TO_DEG*(atan2(params.distort(bottomDist), eyeToScreenDist));
    float topAngle      = (float) RAD_TO_DEG*(atan2(params.distort(topDist), eyeToScreenDist));

    leftEyeFov.setLeft(std::min(outerAngle, idealFovAngle));
    leftEyeFov.setRight(std::min(innerAngle, idealFovAngle));
    leftEyeFov.setBottom(std::min(bottomAngle, idealFovAngle));
    leftEyeFov.setTop(std::min(topAngle, idealFovAngle));

    rightEyeFov.setLeft(std::min(innerAngle, idealFovAngle));
    rightEyeFov.setRight(std::min(outerAngle, idealFovAngle));
    rightEyeFov.setBottom(std::min(bottomAngle, idealFovAngle));
    rightEyeFov.setTop(std::min(topAngle, idealFovAngle));
}

void VrRender::createVAO()
{
    left_vao.create();
    left_vao.bind();
    left_mesh.arrayBuffer.bind();

    program.enableAttributeArray(aPosition);
    glVertexAttribPointer(aPosition, 3, GL_FLOAT,GL_FALSE,20,0*4);

    program.enableAttributeArray(aVignette);
    glVertexAttribPointer(aVignette, 1, GL_FLOAT,GL_FALSE,20,OPENGL_OFFSET(2*4));

    program.enableAttributeArray(aTextureCoord);
    glVertexAttribPointer(aTextureCoord, 2, GL_FLOAT,GL_FALSE,20,OPENGL_OFFSET(3*4));

    left_mesh.elementBuffer.bind();

    left_vao.release();

    /******************************************/

    right_vao.create();
    right_vao.bind();
    right_mesh.arrayBuffer.bind();

    program.enableAttributeArray(aPosition);
    glVertexAttribPointer(aPosition, 3, GL_FLOAT,GL_FALSE,20,0*4);

    program.enableAttributeArray(aVignette);
    glVertexAttribPointer(aVignette, 1, GL_FLOAT,GL_FALSE,20,OPENGL_OFFSET(2*4));

    program.enableAttributeArray(aTextureCoord);
    glVertexAttribPointer(aTextureCoord, 2, GL_FLOAT,GL_FALSE,20,OPENGL_OFFSET(3*4));

    right_mesh.elementBuffer.bind();

    right_vao.release();
}

void VrRender::render(VrFrameBuffer * frame)
{
    if(m_resize)
    {
        createProjection();
        m_resize=false;
    }

    if(!left_vao.isCreated() || !right_vao.isCreated())
        createVAO();

    program.bind();

    frame->bindTexture();

    left_vao.bind();
    glDrawElements(GL_TRIANGLE_STRIP, left_mesh.nIndices, GL_UNSIGNED_SHORT, 0);
    left_vao.release();

    right_vao.bind();
    glDrawElements(GL_TRIANGLE_STRIP, right_mesh.nIndices, GL_UNSIGNED_SHORT, 0);
    right_vao.release();

    program.release();
}

float FieldOfView::left() const
{
    return mLeft;
}

void FieldOfView::setLeft(float left)
{
    mLeft = left;
}
float FieldOfView::right() const
{
    return mRight;
}

void FieldOfView::setRight(float right)
{
    mRight = right;
}
float FieldOfView::bottom() const
{
    return mBottom;
}

void FieldOfView::setBottom(float bottom)
{
    mBottom = bottom;
}
float FieldOfView::top() const
{
    return mTop;
}

void FieldOfView::setTop(float top)
{
    mTop = top;
}

QMatrix4x4 FieldOfView::toPerspectiveMatrix(float near, float far)
{
    float l = (float) -tan(qDegreesToRadians(mLeft)) * near;
    float r = (float) tan(qDegreesToRadians(mRight)) * near;
    float b = (float) -tan(qDegreesToRadians(mBottom)) * near;
    float t = (float) tan(qDegreesToRadians(mTop)) * near;
    QMatrix4x4 matrix;
    matrix.frustum(l,r,b,t,near,far);
    return matrix;
}
