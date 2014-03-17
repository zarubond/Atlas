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
#include "camera.h"


#define degToRad(x) (x * 0.017453)

//for wydle
#undef near
#undef far

Camera::Camera()
{
    projection.loadIdentity();
    setPosition(Vertex3f(0.0,0.0,0.0));
    yaw=0;
    pitch=0;
    screen_height=600;
    screen_width=800;
    sight_distance=1;
    //recal();
}

void Camera::setPitch(float angle)
{
    pitch=angle;
    rotation.rotate(pitch,yaw,0.0);
    recal();
}

void Camera::setYaw(float angle)
{
    yaw=angle;
    rotation.rotate(pitch,yaw,0.0);
    recal();
}

float Camera::getPitch() const
{
    return pitch;
}

float Camera::getYaw() const
{
    return yaw;
}

void Camera::perspective(GLfloat view_angle, GLfloat width,GLfloat height, GLfloat near, GLfloat far)
{
    screen_height=height;
    screen_width=width;

    setup(view_angle,width,height,near,far);

    float size = near * tanf(degToRad(view_angle) / 2.0);
    float left = -size, right = size, bottom = -size / (width/height), top = size / (width/height);

    // unused values in perspective formula.
    projection[1] = 0;
    projection[2] = 0;
    projection[3] = 0;
    projection[4] = 0;
    projection[6] = 0;
    projection[7] = 0;
    projection[12]= 0;
    projection[13]= 0;
    projection[15]= 0;
    // perspective formula.
    projection[0] = 2 * near / (right - left);
    projection[5] = 2 * near / (top - bottom);
    projection[8] = (right + left) / (right - left);
    projection[9] = (top + bottom) / (top - bottom);
    projection[10]= -(far + near) / (far - near);
    projection[11]= -1;
    projection[14]= -(2 * far * near) / (far - near);
}


const Matrix4f &Camera::getLook() const
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

Vertex3f Camera::unproject(int x, int y, GLfloat depth)const
{
    Matrix4f mv=makeRotationMatrix();
    Matrix4f inv=mv*projection;

    inv.inverse();

    Vertex4f in(x , screen_height - y - 1 , depth , 1.0);//d
    in[0]/=screen_width;
    in[1]/=screen_height;
    in*=2;
    in-=1;

    inv.transpose();

    Vertex4f ab=inv*in;//d
    ab/=ab[3];

    return Vertex3f(ab[0],ab[1],ab[2]);
}

void Camera::recal()
{
    Matrix4f rotation=makeRotationMatrix();

    Matrix4f transform(1,0,0,0,
                       0,1,0,0,
                       0,0,1,0,
                       -position[0],-position[1],-position[2],1);

    mvp =  transform*rotation*projection;

    Vertex3f center=position-getDirection();
    Vertex3f up(0,1,0);
    lookAt(position,center,up);
}

Vertex3f Camera::getDirection() const
{/*
    Vertex3f dir;
    dir[0]=cos(pitch) * sin(yaw);
    dir[1]=sin(pitch);
    dir[2]=cos(pitch) * cos(yaw);
    dir.print();
*/
    Vertex3f test(0,0,1);
    test=rotation*test;

    return test;
}

void Camera::onUpdate()
{
    recal();
}

float Camera::getX() const
{
    return position[0];
}

float Camera::getY() const
{
    return position[1];
}

float Camera::getZ() const
{
    return position[2];
}

float Camera::getSightDistance() const
{
    return sight_distance;
}

void Camera::setSightDistance(float value)
{
    sight_distance = value;
}
