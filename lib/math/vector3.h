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

#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "parsenumber.h"

template <typename T = float>
class Vector3;


template <typename Type>
Vector3<Type> operator*(const Vector3<Type> &vertex,Type value);

template <typename Type>
class Vector3: public ParseNumber
{

public:
    Vector3();
    Vector3(Type value);
    Vector3(Type x, Type y, Type z);

    template <typename SType>
    Vector3<Type>(const Vector3<SType> & vec);

    template <typename SType>
    Vector3<Type> & operator +=(const Vector3<SType> & vec);

    template <typename SType>
    Vector3<Type> & operator =(const Vector3<SType> & second);

    template <typename SType>
    Vector3<Type> & operator -=(const Vector3<SType> & vec);

    template <typename SType>
    Vector3<Type> & operator *=(const Vector3<SType> & vec);

    template <typename SType>
    Vector3<Type> & operator /=(const Vector3<SType> & vec);

    Vector3<Type> & operator /=(const Type num);

    Vector3<Type> & operator *=(const Type num);

    Vector3<Type> & operator +=(const Type num);
    /**
     * @brief operator == Compare two vertices.
     * @param vec
     * @return
     */
    bool operator ==(const Vector3<Type> & vec) const;

    bool operator !=(const Vector3<Type> & vec) const;

    template <typename SType>
    const Vector3<Type> operator-(const Vector3<SType> & vec) const;

    template <typename SType>
    const Vector3<Type> operator+(const Vector3<SType> & vec) const;

    template <typename SType>
    const Vector3<Type> operator*(const Vector3<SType> & vec) const;

    void set(Type x,Type y,Type z);


    /**
     * @brief distance Distance between this and other point
     * @param vec Other point
     * @return
     */
    template <typename SType>
    Type distance(const Vector3<SType> & vec) const;

    /**
     * @brief dot Vector dot product.
     * @param vec
     * @return
     */
    Type dot(const Vector3<Type> & vec) const;
    /**
     * @brief normalize Makes unit vector.
     */
    void normalize();
    /**
     * @brief inverse Points vector to opposite direction.
     */
    void inverse();

    /**
     * @brief operator [] get vertex element.
     * @param n
     * @return
     */
    Type & operator[](int n);

    const Type & operator[](int n) const;
    /**
     * @brief length Vector length.
     * @return
     */
    Type length() const;
    /**
     * @brief abs Make vector absolute.
     */
    void abs();

    Type x();
    Type y();
    Type z();


    const Type *array() const;

    std::string toString();
    void fromString(const char * str);


    //friend Vertex3<Type> (::operator* <>) (Type value, const Vertex3<Type> &vertex);
    friend Vector3<Type> (::operator* <>) (const Vector3<Type> &vertex, Type value);

private:
    //hack for gcc http://gcc.gnu.org/ml/libstdc++/2006-02/msg00087.html
    //const char * strch()

    Type v[3];
};

template <typename Type>
Vector3<Type> operator*(const Vector3<Type> &vertex, Type value)
{
    return Vector3<Type>(value*vertex[0], value*vertex[1], value*vertex[2]);
}

template <typename Type>
inline Type & Vector3<Type>::operator[](int n)
{
    return v[n];
}

template <typename Type>
inline const Type & Vector3<Type>::operator[](int n) const
{
    return v[n];
}


#endif // VECTOR3_H
