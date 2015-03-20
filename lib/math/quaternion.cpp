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

#include "quaternion.h"

Quaternion::Quaternion():x(0),y(0),z(0),w(0)
{}

Quaternion::Quaternion(float pitch, float yaw, float roll)
{
    rotate(pitch,yaw,roll);
}

Quaternion::Quaternion(float x, float y, float z, float w):x(x),y(y),z(z),w(w)
{

}

void Quaternion::rotate(float a, float b, float c, float angle)
{
    float sinAngle;
    angle *= 0.5f;
    Vector3<float> vn(a,b,c);
    vn.normalize();

    sinAngle = sin(angle);

    x = (vn[0] * sinAngle);
    y = (vn[1] * sinAngle);
    z = (vn[2] * sinAngle);
    w = cos(angle);
}

void Quaternion::rotate(float pitch, float yaw, float roll)
{
    float r = pitch/2.0;
    float p = yaw/2.0;
    float y = roll/2.0;

    float sinp = sin(p);
    float siny = sin(y);
    float sinr = sin(r);
    float cosp = cos(p);
    float cosy = cos(y);
    float cosr = cos(r);

    this->x = sinr * cosp * cosy - cosr * sinp * siny;
    this->y = cosr * sinp * cosy + sinr * cosp * siny;
    this->z = cosr * cosp * siny - sinr * sinp * cosy;
    this->w = cosr * cosp * cosy + sinr * sinp * siny;
    normalize();
}

Vector3<float> Quaternion::operator *(const Vector3<float> &vec) const
{
    Vector3<float> vn(vec);
    vn.normalize();

    Quaternion vecQuat, resQuat;
    vecQuat.x = vn[0];
    vecQuat.y = vn[1];
    vecQuat.z = vn[2];
    vecQuat.w = 0.0f;

    resQuat = vecQuat * getConjugate();
    resQuat = *this * resQuat;

    return (Vector3<float>(resQuat.x, resQuat.y, resQuat.z));
}

Quaternion Quaternion::operator* (const Quaternion &rq) const
{
    return Quaternion(w * rq.x + x * rq.w + y * rq.z - z * rq.y,
                      w * rq.y + y * rq.w + z * rq.x - x * rq.z,
                      w * rq.z + z * rq.w + x * rq.y - y * rq.x,
                      w * rq.w - x * rq.x - y * rq.y - z * rq.z);
}

void Quaternion::lookRotation(const Vector3<float>& lookAt, Vector3<float> up)
{
    Vector3<float> forward = lookAt;
    forward.normalize();
    Vector3<float> right = up*forward;
    right.normalize();
    up =forward* right;

    w = sqrtf(1.0f + right[0] + up[1] + forward[2]) * 0.5f;
    float w4_recip = 1.0f / (4.0f * w);
    x = (forward[1] - up[2]) * w4_recip;
    y = (right[2] - forward[0]) * w4_recip;
    z = (up[0] - right[1]) * w4_recip;
}

const float &Quaternion::operator[](int n) const
{
    switch(n)
    {
    case 0: return x;
    case 1: return y;
    case 2: return z;
    default: return w;
    }
}
/*
Quaternion Quaternion::LookRotation(Vector& lookAt, Vector& upDirection) {
    Vector forward = lookAt; Vector up = upDirection;
    Vector::OrthoNormalize(&forward, &up);
    Vector right = Vector::Cross(up, forward);


    Quaternion ret;
    ret.w = sqrtf(1.0f + right.x + up.y + forward.z) * 0.5f;
    float w4_recip = 1.0f / (4.0f * ret.w);
    ret.x = (up.z - forward.y) * w4_recip;
    ret.y = (forward.x - right.z) * w4_recip;
    ret.z = (right.y - up.x) * w4_recip;


    return ret;
}*/


void Quaternion::rotate(const Vector3<float> &angle)
{
    rotate(angle[0],angle[1],angle[2]);
}

Vector3<float> Quaternion::getAngle() const
{
    Vector3f ang;
    ang[2]=atan((2.0*(x*y+z*w))/(1.0-2.0*(y*y+z*z)));
    ang[1]=asin( 2.0*(x*z-w*y));
    ang[0]=atan((2.0*(x*z+y*z))/(1.0-2.0*(z*z+w*w)));
    return ang;
}

void Quaternion::direction(const Vector3<float> &dir)
{
    //float len=dir.length();
    //Vertex3<float> dx
}

void Quaternion::normalize()
{
    float div = sqrt(x*x+y*y+z*z+w*w);
    w /= div;
    x /= div;
    y /= div;
    z /= div;
}

Quaternion Quaternion::getConjugate() const
{
    return Quaternion(-x, -y, -z, w);
}

Matrix4<float> Quaternion::makeMatrix() const
{
    Matrix4<float> mat(
        1.0f - 2.0f*y*y - 2.0f*z*z, 2.0f*x*y - 2.0f*z*w, 2.0f*x*z + 2.0f*y*w, 0.0f,
        2.0f*x*y + 2.0f*z*w, 1.0f - 2.0f*x*x - 2.0f*z*z, 2.0f*y*z - 2.0f*x*w, 0.0f,
        2.0f*x*z - 2.0f*y*w, 2.0f*y*z + 2.0f*x*w, 1.0f - 2.0f*x*x - 2.0f*y*y, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);

    return mat;
}

void Quaternion::setValue(float x, float y, float z, float w)
{
    this->x=x;
    this->y=y;
    this->z=z;
    this->w=w;
}

void Quaternion::setMatrix(const Matrix4<float> &matrix)
{
    float t = 0.5 / sqrt(1.0 + matrix[0] + matrix[5] + matrix[10]);
    w = 0.25/t;
    x = (matrix[9] - matrix[6]) *t ;
    y = (matrix[2] - matrix[8]) *t ;
    z = (matrix[4] - matrix[1]) *t ;
    normalize();
}
