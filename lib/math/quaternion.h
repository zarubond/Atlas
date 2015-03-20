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

#ifndef QUATERNION_H
#define QUATERNION_H

#include "math.h"
class Output;

class Quaternion
{
public:
    Quaternion();
    Quaternion(float pitch, float yaw, float roll);
    Quaternion(float x,float y,float z,float w);

    void rotate(float a,float b, float c, float angle);
    void rotate(float pitch, float yaw, float roll);
    void rotate(const Vector3<float> & angle);
    Vector3<float> getAngle() const;
    void direction(const Vector3<float> & dir);
    Matrix4<float> makeMatrix() const;
    void setValue(float x,float y,float z, float w);
    void setMatrix(const Matrix4<float> & matrix);

    void normalize();
    /**
     * @brief getConjugate
     * @return Inverse quaternion
     */
    Quaternion getConjugate() const;
    Vector3<float> operator *(const Vector3<float> &vec) const;
    Quaternion operator *(const Quaternion &rq) const;
    void lookRotation(const Vector3<float> &lookAt, Vector3<float> up=Vector3<float>(0,1,0));
    const float & operator[](int n) const;
private:
    float x,y,z,w;
};

#endif // QUATERNION_H
