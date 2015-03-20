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

/**
 *  @file Matrix4.h
 *  @author Filip Vondrasek, Ondrej Zaruba
 *  Provides basic matrices functions
 *
 * 	Exclamation: Matrix is stored in column-row order, which means, that
 *  matrix written as an element enumeration {0, 1, 2, 3, 4, 5, 6, 7, 8,
 *  9, 10, 11, 12, 13, 14, 15} is (for the purpose of the class being
 *  compatible with OpenGL) stored and worked with as:
 *
 *   |0 4 8  12|
 *   |1 5 9  13|
 *   |2 6 10 14|
 *   |3 7 11 15|
 *
 *  Keep that in mind whoever is going to work with this class!
 */

#ifndef __MATRIX4_H__
#define __MATRIX4_H__

#include <cmath>

#undef near
#undef far
#undef NEAR
#undef FAR

/**
 *  @brief The main matrix class
 */
#include "vector4.h"
#include "vector3.h"

//http://www.cprogramming.com/tutorial/3d/rotationMatrices.html

template <typename Type>
class Matrix4 {

public:
    /**
     *  @brief A constructor which initializes matrix to identity matrix.
     */
    Matrix4<Type> ();
    /**
     *  @brief A constructor which initializes all matrix elements
     *  @param elements[16] A 1D array containing matrix values
     */

    template <typename SType>
    Matrix4<Type> (const SType elements[16]);
    /**
     *  @brief A constructor which initializes all matrix elements
     *  @param elements[4][4] A 2D array containing matrix values
     */
    template <typename SType>
    Matrix4<Type> (const SType elements[4][4]);

    template <typename SType>
    Matrix4<Type> (const Matrix4<SType> & second);

    Matrix4<Type> (Type c0,Type c1,Type c2,Type c3,Type c4,Type c5,Type c6,Type c7,
                   Type c8,Type c9,Type c10,Type c11,Type c12,Type c13,Type c14,Type c15);

    /**
     * @brief Initializes matrix.
     * @param elements[16] A 1D array containing matrix values in OpenGL format
     */
     template <typename SType>
     void set(const SType elements[16]);

     template <typename SType>
     void set (const SType elements[4][4]);

     void set (Type c0,Type c1,Type c2,Type c3,Type c4,Type c5,Type c6,Type c7,
                    Type c8,Type c9,Type c10,Type c11,Type c12,Type c13,Type c14,Type c15);

    Type operator() (const unsigned row,const unsigned col) const;
/*
    Type operator [][] (const int i,const int j)const
    {
        //return matrix[i];
    }*/

    /**
     *  @brief Multiplies a matrix by a matrix
     *  @param second Matrix to multiply with.
     *  @return The result of the multiplication.
     */
    //http://drrobsjournal.blogspot.cz/2012/10/fast-simd-4x4-matrix-multiplication.html
    template <typename SType>
    Matrix4<Type> operator * (const Matrix4<SType> & second) const;

    //template <typename SType>
    //Vector4<Type> operator * (const Vector4<SType> & vex) const;
    /**
     *  @brief Multiplies a matrix by a value
     *  @param second Number to multiply with.
     *  @return The result of the multiplication.
     */
    Matrix4<Type> & operator * (Type value);
    /**
     *  @brief Multiplies a matrix by a value
     *  @param Number to be multiplied.
     *  @param second Matrix to multiply with.
     *  @return The result of the multiplication.
     */

    //friend Matrix4<Type> operator * (const Type value,const Matrix4<Type> & second);

    /**
     *  @brief Substracts a matrix from a matrix
     *  @param second Matrix to subtract.
     *  @return The result of the subtraction.
     */
    template <typename SType>
    Matrix4<Type> operator - (const Matrix4<SType> & second) const;
    /**
     *  @brief Adds a matrix to a matrix
     *  @param second Matrix to add.
     *  @return The result of the addition.
     */
    template <typename SType>
    Matrix4<Type> operator + (const Matrix4<SType> & second) const;

    /**
     *  @brief Multiplies a matrix by a matrix and stores the result
     *  @param second Matrix to multiply with.
     *  @return The result of the multiplication.
     */
    template <typename SType>
    Matrix4<Type> & operator *= (const Matrix4<SType> & second);

    /**
     *  @brief Multiplies a matrix by a value
     *  @param second Number to multiply with.
     *  @return The result of the multiplication.
     */
    Matrix4<Type> & operator *= (Type value);
    /**
     *  @brief Adds a matrix to a matrix.
     *  @param second Matrix to add.
     *  @return The result of the addition.
     */
    Matrix4<Type> & operator += (const Matrix4<Type> & second);
    /**
     *  @brief Substracts a matrix from a matrix.
     *  @param second Matrix to subtract.
     *  @return The result of the subtraction.
     */
    Matrix4<Type> & operator -= (const Matrix4<Type> & second);

    /**
     *   @brief Compares two matrices.
     *   @param second Matrix to compare with.
     *   @return The matrices are either the same (true) or different in any element (false).
     */
    bool operator == (const Matrix4<Type> & second) const;

    template <typename SType>
    Matrix4<Type> & operator =(const Matrix4<SType> & second);

    /**
     *  @brief Returns a pointer to the float array (matrix)
     *  @return Pointer to a matrix float array.
     */
    const Type * array() const;

    /**
     *  @brief Makes an identity matrix.
     */
    void loadIdentity ();
    /**
      * @brief Sets a matrix with elements.
      * @param elements[] Elements to be set.
      */
    void setElements (Type elements[]);
    /**
      * @brief Sets an element in a matrix.
      * @param pos Position of the element.
      * @param element value of the element.
      */
    void setElement (int pos, Type element);
    /**
      * @brief Sets an element in a matrix.
      * @param row Row of the element.
      * @param col Column of the element.
      * @param element value of the element.
      */
    void setElement (int row, int col, Type element);
    /**
      * Loads a perspective matrix.
      * @param angle Angle of the matrix.
      * @param near Near plane.
      * @param far Far plane.
      * @param aspect Aspect of the matrix.
      */
    void perspective (Type angle, Type near, Type far, Type aspect);
    /**
      * @brief Rotates a matrix.
      * @param angle Angle by which to rotate in radian.
      * @param x X coordinate.
      * @param y Y coordinate.
      * @param z Z coordinate.
      */
    void rotate(Type angle, Type x,Type y, Type z);

    /**
      * @brief Rotates a matrix around x axis.
      * @param angle Angle by which to rotate in radian.
      * @return Rotated matrix.
      */
    Matrix4<Type> &rotateX(Type angle);
    /**
      * @brief Rotates a matrix around y axis.
      * @param angle Angle by which to rotate in radian.
      * @return Rotated matrix.
      */
    Matrix4<Type> & rotateY (Type angle);
    /**
      * @brief Rotates a matrix around z axis.
      * @param angle Angle by which to rotate in radian.
      * @return Rotated matrix.
      */
    Matrix4<Type> & rotateZ (Type angle);
    /**
      * @brief Translates a matrix.
      * @param x X coordinate.
      * @param y Y coordinate.
      * @param z Z coordinate.
      * @return Translated matrix.
      */
    Matrix4<Type> & translate (Type x, Type y, Type z);

    Matrix4<Type> & translate(const Vector3<Type> & translate);
    /**
     * @brief ortho Calculate ortographics matrix
     * @param left
     * @param right
     * @param bottom
     * @param top
     * @param zNear
     * @param zFar
     */
    void ortho(Type left ,Type right ,Type bottom ,Type top ,Type zNear ,Type zFar);
    /**
     * @brief lookAt Calculate view matrix.
     * @param eye
     * @param center
     * @param up
     */
    void lookAt(const Vector3<Type> &eye, const Vector3<Type> &center, const Vector3<Type> &up);
    /**
      *@brief Inverse of matrix.
      */
    void inverse();
    /**
      * @brief Calculates determinant of matrix.
      */
    Type determinant();

    //Type & operator[](int n);

    //const Type & operator[](int n) const;

    Matrix4<Type> & scale (Type s);
    Matrix4<Type> & scale (Type sx, Type sy, Type sz);
    /**
     * @brief operator [] Get element of matrix
     * @param n
     * @return
     */
    Type & operator[](int n)
    {
        return matrix[n];
    }

    const Type &operator [](int n) const
    {
        return matrix[n];
    }
    /**
     * @brief transpose Transpose matrix
     */
    void transpose();

    template <typename OType>
    friend Vector4<OType> operator *(const Matrix4<OType> & matrix,const Vector4<OType> & vertex);
protected:
    Type matrix[16];

};

template <typename OType>
Vector4<OType> operator *(const Matrix4<OType> & matrix,const Vector4<OType> & vertex)
{
    return Vector4<OType>(matrix[0]*vertex[0] + matrix[1]*vertex[1] + matrix[2]*vertex[2]  + matrix[3]*vertex[3],
                         matrix[4]*vertex[0] + matrix[5]*vertex[1] + matrix[6]*vertex[2]  + matrix[7]*vertex[3],
                         matrix[8]*vertex[0] + matrix[9]*vertex[1] + matrix[10]*vertex[2] + matrix[11]*vertex[3],
                         matrix[12]*vertex[0] + matrix[13]*vertex[1] + matrix[14]*vertex[2] + matrix[15]*vertex[3]
                    );
}

#endif /* __MATRIX4_H__ */
