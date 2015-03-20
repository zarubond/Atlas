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

#ifndef MATH_H
#define MATH_H

#include "mathdefine.h"

//tempelate problems
#include "vector2.h"
#include "vector2.cpp"
#include "vector3.h"
#include "vector3.cpp"
#include "vector4.h"
#include "vector4.cpp"
#include "matrix4.h"
#include "matrix4.cpp"
#include "matrix3.h"
#include "matrix3.cpp"
#include "quaternion.h"

//objects
#include "sphere.h"
#include "triangle.h"
#include "cube.h"
#include "plane.h"
#include "box.h"
#include "cone.h"
#include "cylinder.h"


typedef Matrix4<float> Matrix4f;
typedef Matrix4<double> Matrix4d;

typedef Matrix3<float> Matrix3f;
typedef Matrix3<double> Matrix3d;

typedef Vector2<float> Vector2f;
typedef Vector2<int> Vector2i;

typedef Vector3<signed char> Vector3c;
typedef Vector3<int> Vector3i;
typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;
typedef Vector3<float> ColorRGB;

typedef Vector4<int> Vector4i;
typedef Vector4<float> Vector4f;
typedef Vector4<double> Vector4d;
//typedef Vector4<float> ColorRGBA;

class ColorRGBA: public Vector4<float>{
public:
    void set(float r, float g, float b, float a)
    {
        v[0]=r;
        v[1]=g;
        v[2]=b;
        v[3]=a;
    }
    void setRed(float r)
    {
        v[0]=r;
    }

    void setGreen(float g)
    {
        v[1]=g;
    }

    void setBlue(float b)
    {
        v[2]=b;
    }

    void setAlpha(float a)
    {
        v[3]=a;
    }
};

typedef unsigned short hfloat;

/*
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
void printGLM(const glm::mat4 & m);

glm::mat4 matrixGLM(const Matrix4f & mat);


void printGLM4(const glm::vec4 & v);
void printGLM3(const glm::vec3 & v);
*/

//http://www.flipcode.com/documents/matrfaq.html

template <typename T>
int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

float fabs(float val);

class Math
{
public:
    Math();
    static hfloat makeHFloat(float f);
    static float makeFloat(hfloat hf);
};

#endif // MATH_H
