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

/*
 *  I would have written this code much cleaner, shorter and generally
 *  nicer, but my team wanted me to do otherwise for the sake of
 *  so-called performance, so here you go.
 */

#include "matrix4.h"

template <typename Type>
Matrix4<Type>::Matrix4 ()
{
    matrix[0] = 0;
    matrix[1] = 0;
    matrix[2] = 0;
    matrix[3] = 0;
    matrix[4] = 0;
    matrix[5] = 0;
    matrix[6] = 0;
    matrix[7] = 0;
    matrix[8] = 0;
    matrix[9] = 0;
    matrix[10] = 0;
    matrix[11] = 0;
    matrix[12] = 0;
    matrix[13] = 0;
    matrix[14] = 0;
    matrix[15] = 0;
}

template <typename Type>
template <typename SType>
Matrix4<Type>::Matrix4 (const SType elements[4][4]) {

    matrix[0] = elements[0][0];
    matrix[4] = elements[0][1];
    matrix[8] = elements[0][2];
    matrix[12] = elements[0][3];
    matrix[1] = elements[1][0];
    matrix[5] = elements[1][1];
    matrix[9] = elements[1][2];
    matrix[13] = elements[1][3];
    matrix[2] = elements[2][0];
    matrix[6] = elements[2][1];
    matrix[10] = elements[2][2];
    matrix[14] = elements[2][3];
    matrix[3] = elements[3][0];
    matrix[7] = elements[3][1];
    matrix[11] = elements[3][2];
    matrix[15] = elements[3][3];
}

template <typename Type>
template <typename SType>
Matrix4<Type>::Matrix4 (const SType elements[])
{
    matrix[0] = elements[0];
    matrix[1] = elements[1];
    matrix[2] = elements[2];
    matrix[3] = elements[3];
    matrix[4] = elements[4];
    matrix[5] = elements[5];
    matrix[6] = elements[6];
    matrix[7] = elements[7];
    matrix[8] = elements[8];
    matrix[9] = elements[9];
    matrix[10] = elements[10];
    matrix[11] = elements[11];
    matrix[12] = elements[12];
    matrix[13] = elements[13];
    matrix[14] = elements[14];
    matrix[15] = elements[15];
}

template <typename Type>
template <typename SType>
Matrix4<Type>::Matrix4 (const Matrix4<SType> & second)
{
    matrix[0]=second.matrix[0];matrix[1]=second.matrix[1];matrix[2]=second.matrix[2];matrix[3]=second.matrix[3];
    matrix[4]=second.matrix[4];matrix[5]=second.matrix[5];matrix[6]=second.matrix[6];matrix[7]=second.matrix[7];
    matrix[8]=second.matrix[8];matrix[9]=second.matrix[9];matrix[10]=second.matrix[10];matrix[11]=second.matrix[11];
    matrix[12]=second.matrix[12];matrix[13]=second.matrix[13];matrix[14]=second.matrix[14];matrix[15]=second.matrix[15];
}

template <typename Type>
Matrix4<Type>::Matrix4 (Type c0,Type c1,Type c2,Type c3,Type c4,Type c5,Type c6,Type c7,
               Type c8,Type c9,Type c10,Type c11,Type c12,Type c13,Type c14,Type c15)
{
    matrix[0] = c0;
    matrix[1] = c1;
    matrix[2] = c2;
    matrix[3] = c3;
    matrix[4] = c4;
    matrix[5] = c5;
    matrix[6] = c6;
    matrix[7] = c7;
    matrix[8] = c8;
    matrix[9] = c9;
    matrix[10] = c10;
    matrix[11] = c11;
    matrix[12] = c12;
    matrix[13] = c13;
    matrix[14] = c14;
    matrix[15] = c15;
}

template <typename Type>
template <typename SType>
void Matrix4<Type>::set(const SType elements[])
{
    matrix[0] = elements[0];
    matrix[1] = elements[1];
    matrix[2] = elements[2];
    matrix[3] = elements[3];
    matrix[4] = elements[4];
    matrix[5] = elements[5];
    matrix[6] = elements[6];
    matrix[7] = elements[7];
    matrix[8] = elements[8];
    matrix[9] = elements[9];
    matrix[10] = elements[10];
    matrix[11] = elements[11];
    matrix[12] = elements[12];
    matrix[13] = elements[13];
    matrix[14] = elements[14];
    matrix[15] = elements[15];
}

template <typename Type>
template <typename SType>
void Matrix4<Type>::set(const SType elements[4][4]) {

    matrix[0] = elements[0][0];
    matrix[4] = elements[0][1];
    matrix[8] = elements[0][2];
    matrix[12] = elements[0][3];
    matrix[1] = elements[1][0];
    matrix[5] = elements[1][1];
    matrix[9] = elements[1][2];
    matrix[13] = elements[1][3];
    matrix[2] = elements[2][0];
    matrix[6] = elements[2][1];
    matrix[10] = elements[2][2];
    matrix[14] = elements[2][3];
    matrix[3] = elements[3][0];
    matrix[7] = elements[3][1];
    matrix[11] = elements[3][2];
    matrix[15] = elements[3][3];
}

template <typename Type>
void Matrix4<Type>::set (Type c0,Type c1,Type c2,Type c3,Type c4,Type c5,Type c6,Type c7,
               Type c8,Type c9,Type c10,Type c11,Type c12,Type c13,Type c14,Type c15)
{
    matrix[0] = c0;
    matrix[1] = c1;
    matrix[2] = c2;
    matrix[3] = c3;
    matrix[4] = c4;
    matrix[5] = c5;
    matrix[6] = c6;
    matrix[7] = c7;
    matrix[8] = c8;
    matrix[9] = c9;
    matrix[10] = c10;
    matrix[11] = c11;
    matrix[12] = c12;
    matrix[13] = c13;
    matrix[14] = c14;
    matrix[15] = c15;
}


template <typename Type>
void Matrix4<Type>::loadIdentity () {

    matrix[1] = matrix[2] = matrix[3] = matrix[4] =
    matrix[6] = matrix[7] = matrix[8] = matrix[9] =
    matrix[11] = matrix[12] = matrix[13] = matrix[14] = 0.0f;

    matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1.0f;
}

template <typename Type>
void Matrix4<Type>::perspective (Type angle, Type near, Type far, Type aspect) {

    float size = near * tanf (angle / 2.0);
    float left = -size, right = size, bottom = -size / aspect, top = size / aspect;

    // unused values in perspective formula.
    matrix[1] = matrix[2] = matrix[3] = matrix[4] = 0;
    matrix[6] = matrix[7] = matrix[12] = matrix[13] = matrix[15] = 0;

    // perspective formula.
    this->matrix[0] = 2 * near / (right - left);
    this->matrix[5] = 2 * near / (top - bottom);
    this->matrix[8] = (right + left) / (right - left);
    this->matrix[9] = (top + bottom) / (top - bottom);
    this->matrix[10] = -(far + near) / (far - near);
    this->matrix[11] = -1;
    this->matrix[14] = -(2 * far * near) / (far - near);
}
template <typename Type>
const Type * Matrix4<Type>::array() const {

    return matrix;
}

template <typename Type>
void Matrix4<Type>::setElements (Type elements[16]) {

    int i;

    for (i = 0; i < 16; i ++) {
        matrix[i] = elements[i];
    }
}

template <typename Type>
void Matrix4<Type>::setElement (int pos, Type element) {

    matrix[pos] = element;
}

template <typename Type>
void Matrix4<Type>::setElement (int row, int col, Type element) {

    matrix[(row * 4) + col] = element;
}

template <typename Type>
Type Matrix4<Type>::operator() (const unsigned row,const unsigned col) const
{
    return matrix[(row * 4) + col];
}

template <typename Type>
template <typename SType>
Matrix4<Type> Matrix4<Type>::operator + (const Matrix4<SType> &second) const {

    Matrix4 result;

    result.matrix[0] = matrix[0] + second.matrix[0];
    result.matrix[1] = matrix[1] + second.matrix[1];
    result.matrix[2] = matrix[2] + second.matrix[2];
    result.matrix[3] = matrix[3] + second.matrix[3];
    result.matrix[4] = matrix[4] + second.matrix[4];
    result.matrix[5] = matrix[5] + second.matrix[5];
    result.matrix[6] = matrix[6] + second.matrix[6];
    result.matrix[7] = matrix[7] + second.matrix[7];
    result.matrix[8] = matrix[8] + second.matrix[8];
    result.matrix[9] = matrix[9] + second.matrix[9];
    result.matrix[10] = matrix[10] + second.matrix[10];
    result.matrix[11] = matrix[11] + second.matrix[11];
    result.matrix[12] = matrix[12] + second.matrix[12];
    result.matrix[13] = matrix[13] + second.matrix[13];
    result.matrix[14] = matrix[14] + second.matrix[14];
    result.matrix[15] = matrix[15] + second.matrix[15];

    return result;
}
template <typename Type>
template <typename SType>
Matrix4<Type> Matrix4<Type>::operator - (const Matrix4<SType> &second) const {

    Matrix4<Type> result;

    result.matrix[0] = matrix[0] - second.matrix[0];
    result.matrix[1] = matrix[1] - second.matrix[1];
    result.matrix[2] = matrix[2] - second.matrix[2];
    result.matrix[3] = matrix[3] - second.matrix[3];
    result.matrix[4] = matrix[4] - second.matrix[4];
    result.matrix[5] = matrix[5] - second.matrix[5];
    result.matrix[6] = matrix[6] - second.matrix[6];
    result.matrix[7] = matrix[7] - second.matrix[7];
    result.matrix[8] = matrix[8] - second.matrix[8];
    result.matrix[9] = matrix[9] - second.matrix[9];
    result.matrix[10] = matrix[10] - second.matrix[10];
    result.matrix[11] = matrix[11] - second.matrix[11];
    result.matrix[12] = matrix[12] - second.matrix[12];
    result.matrix[13] = matrix[13] - second.matrix[13];
    result.matrix[14] = matrix[14] - second.matrix[14];
    result.matrix[15] = matrix[15] - second.matrix[15];

    return result;
}

template <typename Type>
template <typename SType>
Matrix4<Type> Matrix4<Type>::operator * (const Matrix4<SType> &second) const {

    Matrix4<Type> result;
    /*
       11-0  12-1  13-2  14-3

       21-4  22-5  23-6  24-7

       31-8  32-9  33-10 34-11

       41-12 42-13 43-14 44-15
       */

    result.matrix[0] = matrix[0] * second.matrix[0] + matrix[1] * second.matrix[4] +
                       matrix[2] * second.matrix[8] + matrix[3] * second.matrix[12];

    result.matrix[1] = matrix[0] * second.matrix[1] + matrix[1] * second.matrix[5] +
                       matrix[2] * second.matrix[9] + matrix[3] * second.matrix[13];

    result.matrix[2] = matrix[0] * second.matrix[2] + matrix[1] * second.matrix[6] +
                       matrix[2] * second.matrix[10] + matrix[3] * second.matrix[14];

    result.matrix[3] = matrix[0] * second.matrix[3] + matrix[1] * second.matrix[7] +
                       matrix[2] * second.matrix[11] + matrix[3] * second.matrix[15];

    result.matrix[4] = matrix[4] * second.matrix[0] + matrix[5] * second.matrix[4] +
                       matrix[6] * second.matrix[8] + matrix[7] * second.matrix[12];

    result.matrix[5] = matrix[4] * second.matrix[1] + matrix[5] * second.matrix[5] +
                       matrix[6] * second.matrix[9] + matrix[7] * second.matrix[13];

    result.matrix[6] = matrix[4] * second.matrix[2] +  matrix[5] * second.matrix[6] +
                       matrix[6] * second.matrix[10] + matrix[7] * second.matrix[14];

    result.matrix[7] = matrix[4] * second.matrix[3] + matrix[5] * second.matrix[7] +
                       matrix[6] * second.matrix[11] + matrix[7] * second.matrix[15];

    result.matrix[8] = matrix[8] * second.matrix[0] + matrix[9] * second.matrix[4]+
                       matrix[10] * second.matrix[8] + matrix[11] * second.matrix[12];

    result.matrix[9] = matrix[8] * second.matrix[1] + matrix[9] * second.matrix[5] +
                       matrix[10] * second.matrix[9] + matrix[11] * second.matrix[13];

    result.matrix[10] = matrix[8] * second.matrix[2] + matrix[9] * second.matrix[6] +
                        matrix[10] * second.matrix[10] + matrix[11] * second.matrix[14];

    result.matrix[11] = matrix[8] * second.matrix[3] + matrix[9] * second.matrix[7] +
                        matrix[10] * second.matrix[11] + matrix[11] * second.matrix[15];

    result.matrix[12] = matrix[12] * second.matrix[0] + matrix[13] * second.matrix[4] +
                        matrix[14] * second.matrix[8] + matrix[15] * second.matrix[12];

    result.matrix[13] = matrix[12] * second.matrix[1] + matrix[13] * second.matrix[5]+
                        matrix[14] * second.matrix[9]+ matrix[15] * second.matrix[13];

    result.matrix[14] = matrix[12] * second.matrix[2] + matrix[13] * second.matrix[6]+
                    matrix[14] * second.matrix[10] + matrix[15] * second.matrix[14];

    result.matrix[15] = matrix[12] * second.matrix[3] + matrix[13] * second.matrix[7] +
                        matrix[14] * second.matrix[11]+ matrix[15] * second.matrix[15];

    return result;
}
/*
template <typename Type>
template <typename SType>
Vector4<Type> Matrix4<Type>::operator *(const Vector4<SType> &vertex) const
{
    return Vector4<Type>(matrix[0]*vertex[0] + matrix[1]*vertex[1] + matrix[2]*vertex[2]  + matrix[3]*vertex[3],
                         matrix[4]*vertex[0] + matrix[5]*vertex[1] + matrix[6]*vertex[2]  + matrix[7]*vertex[3],
                         matrix[8]*vertex[0] + matrix[9]*vertex[1] + matrix[10]*vertex[2] + matrix[11]*vertex[3],
                         matrix[12]*vertex[0] + matrix[13]*vertex[1] + matrix[14]*vertex[2] + matrix[15]*vertex[3]
                    );
}*/

template <typename Type>
Matrix4<Type> & Matrix4<Type>::operator * (Type value) {

    matrix[0] *= value;
    matrix[1] *= value;
    matrix[2] *= value;
    matrix[3] *= value;
    matrix[4] *= value;
    matrix[5] *= value;
    matrix[6] *= value;
    matrix[7] *= value;
    matrix[8] *= value;
    matrix[9] *= value;
    matrix[10] *= value;
    matrix[11] *= value;
    matrix[12] *= value;
    matrix[13] *= value;
    matrix[14] *= value;
    matrix[15] *= value;
    return * this;
}

template <typename Type>
Matrix4<Type> operator * (Type value, Matrix4<Type> & mat) {

    mat *= value;
    return mat;
}

template <typename Type>
Matrix4<Type> & Matrix4<Type>::operator += (const Matrix4<Type> & second) {

    return (*this = *this + second);
}
template <typename Type>
Matrix4<Type> & Matrix4<Type>::operator -= (const Matrix4<Type> & second) {

    return (*this = *this - second);
}
template <typename Type>
template <typename SType>
Matrix4<Type> & Matrix4<Type>::operator *= (const Matrix4<SType> &second) {

    return (*this = *this * second);
}
template <typename Type>
Matrix4<Type> & Matrix4<Type>::operator *= (Type value)
{

    return (*this = *this * value);
}

template <typename Type>
bool Matrix4<Type>::operator == (const Matrix4<Type> & second) const
{
    if(matrix[0]==second.matrix[0] && matrix[1]==second.matrix[1] && matrix[2]==second.matrix[2] &&
            matrix[3]==second.matrix[3] && matrix[4]==second.matrix[4] && matrix[5]==second.matrix[5] &&
            matrix[6]==second.matrix[6] && matrix[7]==second.matrix[7] && matrix[8]==second.matrix[8] &&
            matrix[9]==second.matrix[9] && matrix[10]==second.matrix[10] && matrix[11]==second.matrix[11] &&
            matrix[12]==second.matrix[12] && matrix[13]==second.matrix[13] && matrix[14]==second.matrix[14] &&
            matrix[15]==second.matrix[15])
        return true;

    return false;
}

template <typename Type>
template <typename SType>
Matrix4<Type> & Matrix4<Type>::operator =(const Matrix4<SType> & second)
{
    matrix[0]=second.matrix[0];matrix[1]=second.matrix[1];matrix[2]=second.matrix[2];matrix[3]=second.matrix[3];
    matrix[4]=second.matrix[4];matrix[5]=second.matrix[5];matrix[6]=second.matrix[6];matrix[7]=second.matrix[7];
    matrix[8]=second.matrix[8];matrix[9]=second.matrix[9];matrix[10]=second.matrix[10];matrix[11]=second.matrix[11];
    matrix[12]=second.matrix[12];matrix[13]=second.matrix[13];matrix[14]=second.matrix[14];matrix[15]=second.matrix[15];

    return *this;
}

template <typename Type>
Matrix4<Type> & Matrix4<Type>::rotateX (Type angle)
{
    Matrix4<Type> rotation (1,0,0,0,
                            0,cos(angle),-sin(angle),0,
                            0,sin(angle),cos(angle),0,
                            0,0,0,1);

    return (*this = *this * rotation);
}

template <typename Type>
Matrix4<Type> & Matrix4<Type>::rotateY (Type angle)
{
    Matrix4<Type> rotation (cos(angle), 0, sin(angle), 0,
                            0, 1, 0, 0,
                           -sin(angle), 0, cos(angle), 0,
                            0, 0, 0, 1);

    return (*this = *this * rotation);
}

template <typename Type>
Matrix4<Type> & Matrix4<Type>::rotateZ (Type angle)
{
    Type elements[16] = {cos (angle), -sin (angle), 0.0, 0.0,
                          sin (angle), cos (angle), 0.0, 0.0,
                          0.0, 0.0, 1.0, 0.0,
                          0.0, 0.0, 0.0, 1.0};

    Matrix4<Type> rotation (elements);

    return (*this = *this * rotation);
}

template <typename Type>
void Matrix4<Type>::rotate(Type angle, Type x, Type y, Type z)
{
    matrix[0] = 1+(1-cos(angle))*(x*x-1);
    matrix[1] = -z*sin(angle)+(1-cos(angle))*x*y;
    matrix[2] = y*sin(angle)+(1-cos(angle))*x*z;
    matrix[3] = 0;

    matrix[4] = z*sin(angle)+(1-cos(angle))*x*y;
    matrix[5] = 1+(1-cos(angle))*(y*y-1);
    matrix[6] = -x*sin(angle)+(1-cos(angle))*y*z;
    matrix[7] = 0;

    matrix[8] = -y*sin(angle)+(1-cos(angle))*x*z;
    matrix[9] = x*sin(angle)+(1-cos(angle))*y*z;
    matrix[10] = 1+(1-cos(angle))*(z*z-1);
    matrix[11] = 0;

    matrix[12] = 0;
    matrix[13] = 0;
    matrix[14] = 0;
    matrix[15] = 1;
}

template <typename Type>
Matrix4<Type> & Matrix4<Type>::translate(Type x, Type y, Type z)
{
    Matrix4<Type> transform(1,0,0,0,
                            0,1,0,0,
                            0,0,1,0,
                            x,y,z,1);
    return ((*this) =  (*this)*transform);
}

template <typename Type>
Matrix4<Type> &Matrix4<Type>::translate(const Vector3<Type> &translate)
{
    Matrix4<Type> transform(1,0,0,0,
                            0,1,0,0,
                            0,0,1,0,
                            translate[0],translate[1],translate[2],1);
    return ((*this) =  (*this)*transform);
}

template <typename Type>
void Matrix4<Type>::ortho(Type left, Type right, Type bottom, Type top, Type zNear, Type zFar)
{
    loadIdentity();

    matrix[0] = 2 / (right - left);
    matrix[5] = 2 / (top - bottom);
    matrix[10] = - 2 / (zFar - zNear);

    matrix[3] = - (right + left) / (right - left);
    matrix[7] = - (top + bottom) / (top - bottom);
    matrix[11] = - (zFar + zNear) / (zFar - zNear);
}

template <typename Type>
void Matrix4<Type>::lookAt(const Vector3<Type> & eye,const Vector3<Type> & center,const  Vector3<Type> & up)
{
    loadIdentity();

    Vector3<Type> f=center;
    f-=eye;
    f.normalize();
    Vector3<Type> u=up;
    u.normalize();
    Vector3<Type> s=f*u;
    s.normalize();
    u=s*f;

    matrix[0]=s[0];
    matrix[1]=s[1];
    matrix[2]=s[2];

    matrix[4]=u[0];
    matrix[5]=u[1];
    matrix[6]=u[2];

    matrix[8]=-f[0];
    matrix[9]=-f[1];
    matrix[10]=-f[2];

    matrix[3] = s.dot(eye);
    matrix[7] = u.dot(eye);
    matrix[11] = f.dot(eye);
}

template <typename Type>
void Matrix4<Type>::inverse()
{/*
   11-0  12-1  13-2  14-3

   21-4  22-5  23-6  24-7

   31-8  32-9  33-10 34-11

   41-12 42-13 43-14 44-15
   */
    Type det=1/determinant();
    Type mat[16];

    mat[0]=matrix[5]*matrix[10]*matrix[15] + matrix[6]*matrix[11]*matrix[13] + matrix[7]*matrix[9]*matrix[14]
          -matrix[5]*matrix[11]*matrix[14] - matrix[6]*matrix[9]*matrix[15]  - matrix[7]*matrix[10]*matrix[13];

    mat[1]=matrix[1]*matrix[11]*matrix[14] + matrix[2]*matrix[9]*matrix[15] + matrix[3]*matrix[10]*matrix[13]
          -matrix[1]*matrix[10]*matrix[15] - matrix[2]*matrix[11]*matrix[13]- matrix[3]*matrix[9]*matrix[14];

    mat[2]=matrix[1]*matrix[6]*matrix[15] + matrix[2]*matrix[7]*matrix[13] + matrix[3]*matrix[5]*matrix[14]
          -matrix[1]*matrix[7]*matrix[14] - matrix[2]*matrix[5]*matrix[15]- matrix[3]*matrix[6]*matrix[13];

    mat[3]=matrix[1]*matrix[7]*matrix[10] + matrix[2]*matrix[5]*matrix[11] + matrix[3]*matrix[6]*matrix[9]
          -matrix[1]*matrix[6]*matrix[11] - matrix[2]*matrix[7]*matrix[9]- matrix[3]*matrix[5]*matrix[10];

    mat[4]=matrix[4]*matrix[11]*matrix[14] + matrix[6]*matrix[8]*matrix[15] + matrix[7]*matrix[10]*matrix[12]
          -matrix[4]*matrix[10]*matrix[15] - matrix[6]*matrix[11]*matrix[12]- matrix[7]*matrix[8]*matrix[14];

    mat[5]=matrix[0]*matrix[10]*matrix[15] + matrix[2]*matrix[11]*matrix[12] + matrix[3]*matrix[8]*matrix[14]
          -matrix[0]*matrix[11]*matrix[14] - matrix[2]*matrix[8]*matrix[15]- matrix[3]*matrix[10]*matrix[12];

    mat[6]=matrix[0]*matrix[7]*matrix[14] + matrix[2]*matrix[4]*matrix[15] + matrix[3]*matrix[6]*matrix[12]
          -matrix[0]*matrix[6]*matrix[15] - matrix[2]*matrix[7]*matrix[12]- matrix[3]*matrix[4]*matrix[14];

    mat[7]=matrix[0]*matrix[6]*matrix[11] + matrix[2]*matrix[7]*matrix[8] + matrix[3]*matrix[4]*matrix[10]
          -matrix[0]*matrix[7]*matrix[10] - matrix[2]*matrix[4]*matrix[11]- matrix[3]*matrix[6]*matrix[8];

    mat[8]=matrix[4]*matrix[9]*matrix[15] + matrix[5]*matrix[11]*matrix[12] + matrix[7]*matrix[8]*matrix[13]
          -matrix[4]*matrix[11]*matrix[13] - matrix[5]*matrix[8]*matrix[15]- matrix[7]*matrix[9]*matrix[12];

    mat[9]=matrix[0]*matrix[11]*matrix[13] + matrix[1]*matrix[8]*matrix[15] + matrix[3]*matrix[9]*matrix[12]
          -matrix[0]*matrix[9]*matrix[15] - matrix[1]*matrix[11]*matrix[12]- matrix[3]*matrix[8]*matrix[13];

    mat[10]=matrix[0]*matrix[5]*matrix[15] + matrix[1]*matrix[7]*matrix[12] + matrix[3]*matrix[4]*matrix[13]
          -matrix[0]*matrix[7]*matrix[13] - matrix[1]*matrix[4]*matrix[15]- matrix[3]*matrix[5]*matrix[12];

    mat[11]=matrix[0]*matrix[7]*matrix[9] + matrix[1]*matrix[4]*matrix[11] + matrix[3]*matrix[5]*matrix[8]
          -matrix[0]*matrix[5]*matrix[11] - matrix[1]*matrix[7]*matrix[8]- matrix[3]*matrix[4]*matrix[9];

    mat[12]=matrix[4]*matrix[10]*matrix[13] + matrix[5]*matrix[8]*matrix[14] + matrix[6]*matrix[9]*matrix[12]
          -matrix[4]*matrix[9]*matrix[14] - matrix[5]*matrix[10]*matrix[12]- matrix[6]*matrix[8]*matrix[13];

    mat[13]=matrix[0]*matrix[9]*matrix[14] + matrix[1]*matrix[10]*matrix[12] + matrix[2]*matrix[8]*matrix[13]
          -matrix[0]*matrix[10]*matrix[13] - matrix[1]*matrix[8]*matrix[14]- matrix[2]*matrix[9]*matrix[12];

    mat[14]=matrix[0]*matrix[6]*matrix[13] + matrix[1]*matrix[4]*matrix[14] + matrix[2]*matrix[5]*matrix[12]
          -matrix[0]*matrix[5]*matrix[14] - matrix[1]*matrix[6]*matrix[12]- matrix[2]*matrix[4]*matrix[13];

    mat[15]=matrix[0]*matrix[5]*matrix[10] + matrix[1]*matrix[6]*matrix[8] + matrix[2]*matrix[4]*matrix[9]
          -matrix[0]*matrix[6]*matrix[9] - matrix[1]*matrix[4]*matrix[10]- matrix[2]*matrix[5]*matrix[8];

    matrix[0] =mat[0]*det;
    matrix[1] =mat[1]*det;
    matrix[2] =mat[2]*det;
    matrix[3] =mat[3]*det;
    matrix[4] =mat[4]*det;
    matrix[5] =mat[5]*det;
    matrix[6] =mat[6]*det;
    matrix[7] =mat[7]*det;
    matrix[8] =mat[8]*det;
    matrix[9] =mat[9]*det;
    matrix[10]=mat[10]*det;
    matrix[11]=mat[11]*det;
    matrix[12]=mat[12]*det;
    matrix[13]=mat[13]*det;
    matrix[14]=mat[14]*det;
    matrix[15]=mat[15]*det;
}

template <typename Type>
Type Matrix4<Type>::determinant()
{
    /*
       11-0  12-1  13-2  14-3

       21-4  22-5  23-6  24-7

       31-8  32-9  33-10 34-11

       41-12 42-13 43-14 44-15
       */

    Type det=matrix[0]*matrix[5]*matrix[10]*matrix[15]
              +matrix[0]*matrix[6]*matrix[11]*matrix[13]
              +matrix[0]*matrix[7]*matrix[9]*matrix[14]

              +matrix[1]*matrix[4]*matrix[11]*matrix[14]
              +matrix[1]*matrix[6]*matrix[8]*matrix[15]
              +matrix[1]*matrix[7]*matrix[10]*matrix[12]

              +matrix[2]*matrix[4]*matrix[9]*matrix[15]
              +matrix[2]*matrix[5]*matrix[11]*matrix[12]
              +matrix[2]*matrix[7]*matrix[8]*matrix[13]

              +matrix[3]*matrix[4]*matrix[10]*matrix[13]
              +matrix[3]*matrix[5]*matrix[8]*matrix[14]
              +matrix[3]*matrix[6]*matrix[9]*matrix[12]

              -matrix[0]*matrix[5]*matrix[11]*matrix[14]
              -matrix[0]*matrix[6]*matrix[9]*matrix[15]
              -matrix[0]*matrix[7]*matrix[10]*matrix[13]

              -matrix[1]*matrix[4]*matrix[10]*matrix[15]
              -matrix[1]*matrix[6]*matrix[11]*matrix[12]
              -matrix[1]*matrix[7]*matrix[8]*matrix[14]

              -matrix[2]*matrix[4]*matrix[11]*matrix[13]
              -matrix[2]*matrix[5]*matrix[8]*matrix[15]
              -matrix[2]*matrix[7]*matrix[9]*matrix[12]

              -matrix[3]*matrix[4]*matrix[9]*matrix[14]
              -matrix[3]*matrix[5]*matrix[10]*matrix[12]
              -matrix[3]*matrix[6]*matrix[8]*matrix[13];
    return det;
}

template <typename Type>
Matrix4<Type> &Matrix4<Type>::scale(Type s)
{
    Matrix4 rotation (s, 0, 0, 0,
                      0, s, 0, 0,
                      0, 0, s, 0,
                      0, 0, 0, 1);

    return (*this = *this * rotation);
}

template <typename Type>
Matrix4<Type> &Matrix4<Type>::scale(Type sx, Type sy, Type sz)
{
    Matrix4 rotation (sx, 0, 0, 0,
                      0, sy, 0, 0,
                      0, 0, sz, 0,
                      0, 0, 0, 1);

    return (*this = *this * rotation);
}

/*
*			| 0  1  2  3  |             | 0  4  8  12 |
*			| 4  5  6  7  |             | 1  5  9  13 |
*			| 8  9  10 11 |             | 2  6  10 14 |
*			| 12 13 14 15 |             | 3  7  11 15 |
  */
template <typename Type>
void Matrix4<Type>::transpose()
{
    float tmp;
    tmp=matrix[1];
    matrix[1]=matrix[4];
    matrix[4]=tmp;

    tmp=matrix[2];
    matrix[2]=matrix[8];
    matrix[8]=tmp;

    tmp=matrix[3];
    matrix[3]=matrix[12];
    matrix[12]=tmp;

    tmp=matrix[6];
    matrix[6]=matrix[9];
    matrix[9]=tmp;

    tmp=matrix[7];
    matrix[7]=matrix[13];
    matrix[13]=tmp;

    tmp=matrix[11];
    matrix[11]=matrix[14];
    matrix[14]=tmp;


}
