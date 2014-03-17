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
#ifndef MATH_H
#define MATH_H

#include <cmath>

#undef near
#undef far
#undef NEAR
#undef FAR

//tempelate problems
#include "vertex3.h"
#include "vertex3.cpp"
#include "vertex4.h"
#include "vertex4.cpp"
#include "matrix4.h"
#include "matrix4.cpp"
#include "matrix3.h"
#include "matrix3.cpp"


#include "sphere.h"
#include "triangle.h"
#include "cube.h"
#include "plane.h"
#include "quaternion.h"
#include "box.h"


typedef Matrix4<float> Matrix4f;
typedef Matrix4<double> Matrix4d;

typedef Matrix3<float> Matrix3f;
typedef Matrix3<double> Matrix3d;

typedef Vertex3<int> Vertex3i;
typedef Vertex3<float> Vertex3f;
typedef Vertex3<double> Vertex3d;
typedef Vertex3<float> ColorRGB;

typedef Vertex4<int> Vertex4i;
typedef Vertex4<float> Vertex4f;
typedef Vertex4<double> Vertex4d;
typedef Vertex4<float> ColorRGBA;

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
};

#endif // MATH_H
