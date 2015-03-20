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

#include "camera.h"


#define degToRad(x) (x * 0.017453)

//for wydle
#undef near
#undef far

Camera::Camera()
{
    projection.loadIdentity();
    mvp.loadIdentity();

    setPosition(Vector3f(0.0,0.0,0.0));
    yaw=0;
    pitch=0;
    screen_height=600;
    screen_width=800;
    sight_distance=1;

    m_rotation.setValue(0,0,0,0);
    //recal();
}

void Camera::setPitch(float angle)
{
    pitch=angle;
    m_rotation.rotate(pitch,yaw,0.0);
    recalc();
}

void Camera::setYaw(float angle)
{
    yaw=angle;
    m_rotation.rotate(pitch,yaw,0.0);
    recalc();
}

float Camera::getPitch() const
{
    return pitch;
}

float Camera::getYaw() const
{
    return yaw;
}

void Camera::perspective(float view_angle, float width, float height, float near, float far)
{
    screen_height=height;
    screen_width=width;
    this->view_angle=view_angle;

    setup(view_angle,width,height,near,far);

    float size = near * tanf(degToRad(view_angle) / 2.0);
    float right = size, top = -size / (width/height);
//http://www.songho.ca/opengl/gl_projectionmatrix.html
    projection[1] = 0;
    projection[2] = 0;
    projection[3] = 0;
    projection[4] = 0;
    projection[6] = 0;
    projection[7] = 0;
    projection[8] = 0;
    projection[9] = 0;
    projection[12]= 0;
    projection[13]= 0;
    projection[15]= 0;
    // perspective formula.
    projection[0] = near / right;
    projection[5] = near / top;

    projection[10]= -(far + near) / (far - near);
    projection[11]= -1;
    projection[14]= -(2 * far * near) / (far - near);

    recalc();
}

void Camera::perspective(Matrix4f &proj)
{
    projection=proj;
    recalc();
}

Matrix4f Camera::infinitePerspectiveLook() const
{
    //http://www.terathon.com/gdc07_lengyel.pdf
    Matrix4f rotation=makeRotationMatrix();

    Matrix4f transform(1,0,0,0,
                       0,1,0,0,
                       0,0,1,0,
                       -m_position[0],-m_position[1],-m_position[2],1);
    float width=screen_width;
    float height=screen_height;

    float size = near * tanf(degToRad(view_angle) / 2.0);
    float right = size, top = -size / (width/height);
    float eps=0.000024;

    Matrix4f proj;
    proj[1] = 0;
    proj[2] = 0;
    proj[3] = 0;
    proj[4] = 0;
    proj[6] = 0;
    proj[7] = 0;
    proj[8] = 0;
    proj[9] = 0;
    proj[12]= 0;
    proj[13]= 0;
    proj[15]= 0;

    // perspective formula.
    proj[0] = near / right;
    proj[5] = near / top;

    proj[10]= eps-1;
    proj[11]= -1;
    proj[14]= (eps-2)*near;


    return transform*rotation*proj;
}

Matrix4f Camera::orthoLook() const
{
    Matrix4f rotation=makeRotationMatrix();
    cout<<rotation<<endl;


    Matrix4f transform(1,0,0,0,
                       0,1,0,0,
                       0,0,1,0,
                       -m_position[0],-m_position[1],-m_position[2],1);

    float width=screen_width;
    float height=screen_height;

    float size = near * tanf(degToRad(view_angle) / 2.0);
   // float right = size, top = -size / (width/height);
    float right=width/2.0;
    float top=height/2.0;
    float left=-width/2.0;
    float bottom=-height/2.0;

    Matrix4f proj;

    proj[0] = 2.0 / (right - left);
    proj[1] = 0.0;
    proj[2] = 0.0;
    proj[3] = 0.0;

    // Second Column
    proj[4] = 0.0;
    proj[5] = 2.0 / (top - bottom);
    proj[6] = 0.0;
    proj[7] = 0.0;

    // Third Column
    proj[8] = 0.0;
    proj[9] = 0.0;
    proj[10] = -2.0 / (far - near);
    proj[11] = 0.0;

    // Fourth Column
    proj[12] = -(right + left) / (right - left);
    proj[13] = -(top + bottom) / (top - bottom);
    proj[14] = -(far + near) / (far - near);
    proj[15] = 1;

    //rotation.transpose();

    return transform*rotation*proj;
}

const Matrix4f &Camera::perspectiveLook() const
{
    return mvp;
}

const Matrix4f &Camera::getProjectionMatrix() const {

    return this->projection;
}

unsigned Camera::height() const
{
    return screen_height;
}

unsigned Camera::width() const
{
    return screen_width;
}

Vector3f Camera::unproject(int x, int y, float depth)const
{
    Matrix4f mv=makeRotationMatrix();
    Matrix4f inv=mv*projection;

    inv.inverse();

    //Vertex4f in(x , screen_height - y - 1 , depth , 1.0);//d
    Vector4f in(x , y , depth , 1.0);//d
    in[0]/=screen_width;
    in[1]/=screen_height;
    in*=2;
    in-=1;

    inv.transpose();

    Vector4f ab=inv*in;//d
    ab/=ab[3];

    return Vector3f(ab[0],ab[1],ab[2]);
}

Vector2f Camera::project(const Vector3f &point)
{
    Vector4f tmp(point[0], point[1], point[2], 1.0);
    Vector4f res;
    res=mvp*tmp;
    return Vector2f(res[0], res[1]);
}

void Camera::recalc()
{
    Matrix4f rotation=makeRotationMatrix();

    Matrix4f transform(1,0,0,0,
                       0,1,0,0,
                       0,0,1,0,
                       -m_position[0],-m_position[1],-m_position[2],1);

    mvp =  transform*rotation*projection;

    Vector3f center=m_position-getDirection();
    Vector3f up(0,1,0);
    lookAt(m_position,center,up);
}

Vector3f Camera::getDirection() const
{/*
    Vertex3f dir;
    dir[0]=cos(pitch) * sin(yaw);
    dir[1]=sin(pitch);
    dir[2]=cos(pitch) * cos(yaw);
    dir.print();
*/
    Vector3f test(0,0,1);
    test=m_rotation*test;

    return test;
}

void Camera::onUpdate()
{
    recalc();
}

float Camera::getSightDistance() const
{
    return sight_distance;
}

void Camera::setSightDistance(float value)
{
    sight_distance = value;
}
