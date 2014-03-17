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
#ifndef MATRIX3_H
#define MATRIX3_H

template <typename Type>
class Matrix4;

//#include "matrix4.h"


/**
 * @brief The Matrix3f class
 */
template <typename Type>
class Matrix3
{
public:
    Matrix3<Type>();
    Matrix3<Type>(const Type m4[9]);
    /**
     * @brief invese Inverse this matrix.
     */
    void invese();
    /**
     * @brief transpose Transpose this matrix.
     */
    void transpose();

    template <typename SType>
    Matrix3<Type> & operator =(const Matrix4<SType> & second);
    /**
     * @brief determinant Matrix determinant.
     * @return Value of deterninant.
     */
    Type determinant();
    /**
     * @brief array Array of matrix values.
     * @return  Matrix array.
     */
    const Type * array()const;
    /**
     * @brief operator []
     * @param n
     * @return
     */
    Type & operator[](int n)
    {
        return matrix[n];
    }
    /**
     * @brief operator []
     * @param n
     * @return
     */
    const Type &operator [](int n) const
    {
        return matrix[n];
    }

    /**
     * @brief print Print matrix to shell.
     */
    void print();
    /**
     * @brief loadIdentity
     */
    void loadIdentity();
private:
    Type matrix[9];
};

#endif // MATRIX3_H
