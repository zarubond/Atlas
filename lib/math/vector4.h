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

#ifndef VERTEX4_H
#define VERTEX4_H

#include <cmath>
#include "parsenumber.h"

template <typename T = float>
class Vector4;

template <typename Type>
class Vector4: public ParseNumber
{
public:
    Vector4(Type x, Type y, Type z,Type w);
    Vector4();
    /**
     * @brief operator /=
     * @param value
     * @return
     */
    Vector4<Type> & operator /= (Type value);
    /**
     * @brief operator *=
     * @param value
     * @return
     */
    Vector4<Type> & operator *= (Type value);
    /**
     *
     */
    template<typename SType>
    Vector4<Type> & operator *=(const Vector4<SType> & vec);
    /**
     * @brief operator -=
     * @param value
     * @return
     */
    Vector4<Type> & operator -= (Type value);
    /**
     *
     */
    template <typename SType>
    Vector4<Type> & operator =(const Vector4<SType> & second);
    /**
     * @brief operator []
     * @param n
     * @return
     */
    Type & operator[](int n);
    /**
     * @brief operator []
     * @param n
     * @return
     */
    const Type & operator[](int n) const;
    /**
     * @brief length
     * @return Lenght of vector.
     */
    Type length() const;
    /**
     * @brief normalize
     * @return
     */
    Vector4<Type> & normalize();

    template <typename SType>
    void set(const Vector4<SType> &vec);

    void set(Type x ,Type y ,Type z ,Type w);

    const Type *array() const;

    std::string toString() const;
    /**
     * @brief fromString Converts string in format "x/y/z/w" (ie. "0.02/11.2/22.0/99.3")
     * @param str
     */
    void fromString(const char *str);
protected:
    Type v[4];

};

template <typename Type>
Vector4<Type> operator * (Type value, Vector4<Type> & vec)
{
    vec *= value;
    return vec;
}

template <typename Type>
inline Type & Vector4<Type>::operator[](int n)
{
    return v[n];
}

template <typename Type>
inline const Type & Vector4<Type>::operator[](int n) const
{
    return v[n];
}

#endif // VERTEX4_H
