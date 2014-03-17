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
#include "matrix3.h"

#include "./matrix4.h"

template <typename Type>
Matrix3<Type>::Matrix3()
{
}

template <typename Type>
Matrix3<Type>::Matrix3(const Type m4[])
{
    matrix[0]=m4[0];
    matrix[1]=m4[1];
    matrix[2]=m4[2];

    matrix[3]=m4[3];
    matrix[4]=m4[4];
    matrix[5]=m4[5];

    matrix[6]=m4[6];
    matrix[7]=m4[7];
    matrix[8]=m4[8];
}

template <typename Type>
void Matrix3<Type>::invese()
{

    Matrix3<Type> a=*this;
    Type det=1/determinant();

    matrix[0]=(a.matrix[4]*a.matrix[8] - a.matrix[5]*a.matrix[7])*det;
    matrix[1]=(a.matrix[2]*a.matrix[7] - a.matrix[1]*a.matrix[8])*det;
    matrix[2]=(a.matrix[1]*a.matrix[5] - a.matrix[2]*a.matrix[4])*det;

    matrix[3]=(a.matrix[5]*a.matrix[6] - a.matrix[3]*a.matrix[8])*det;
    matrix[4]=(a.matrix[0]*a.matrix[8] - a.matrix[2]*a.matrix[6])*det;
    matrix[5]=(a.matrix[2]*a.matrix[3] - a.matrix[0]*a.matrix[5])*det;

    matrix[6]=(a.matrix[3]*a.matrix[7] - a.matrix[4]*a.matrix[6])*det;
    matrix[7]=(a.matrix[1]*a.matrix[6] - a.matrix[0]*a.matrix[7])*det;
    matrix[8]=(a.matrix[0]*a.matrix[4] - a.matrix[1]*a.matrix[3])*det;
}

template <typename Type>
void Matrix3<Type>::transpose()
{
    Type tmp;
    tmp=matrix[1];
    matrix[1]=matrix[3];
    matrix[3]=tmp;

    tmp=matrix[2];
    matrix[2]=matrix[6];
    matrix[6]=tmp;

    tmp=matrix[5];
    matrix[5]=matrix[7];
    matrix[7]=tmp;
}

template <typename Type>
Type Matrix3<Type>::determinant()
{
    //a11a22a33+a21a32a13+a31a12a23-a11a32a23-a31a22a13-a21a12a33
    return matrix[0]*matrix[4]*matrix[8] + matrix[3]*matrix[7]*matrix[2] + matrix[6]*matrix[1]*matrix[5]
            -matrix[0]*matrix[7]*matrix[5] - matrix[6]*matrix[4]*matrix[2] - matrix[3]*matrix[1]*matrix[8];
}

template <typename Type>
const Type *Matrix3<Type>::array() const
{
    return matrix;
}

template <typename Type>
void Matrix3<Type>::print()
{
    std::cout<<matrix[0]<<" "<<matrix[1]<<" "<<matrix[2]<<std::endl;
    std::cout<<matrix[3]<<" "<<matrix[4]<<" "<<matrix[5]<<std::endl;
    std::cout<<matrix[6]<<" "<<matrix[7]<<" "<<matrix[8]<<std::endl;
}

template <typename Type>
template <typename SType>
Matrix3<Type> & Matrix3<Type>::operator =(const Matrix4<SType> & second)
{
    matrix[0]=second.matrix[0];
    matrix[1]=second.matrix[1];
    matrix[2]=second.matrix[2];

    matrix[3]=second.matrix[4];
    matrix[4]=second.matrix[5];
    matrix[5]=second.matrix[6];

    matrix[6]=second.matrix[8];
    matrix[7]=second.matrix[9];
    matrix[8]=second.matrix[10];

    return *this;
}


template <typename Type>
void Matrix3<Type>::loadIdentity ()
{
    matrix[0] = matrix[4] = matrix[8] = 1.0;
    matrix[1] = matrix[2] = matrix[3] = 0.0;
    matrix[5] = matrix[6] = matrix[7] = 0.0;
}
