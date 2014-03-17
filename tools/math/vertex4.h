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
#ifndef VERTEX4_H
#define VERTEX4_H

#include <cmath>
#include <iostream>

template <typename T = float>
class Vertex4;

template <typename Type>
class Vertex4{
public:
    Vertex4(Type x, Type y, Type z,Type w);
    Vertex4();
    /**
     * @brief print
     */
    void print();
    /**
     * @brief operator /=
     * @param value
     * @return
     */
    Vertex4<Type> & operator /= (Type value);
    /**
     * @brief operator *=
     * @param value
     * @return
     */
    Vertex4<Type> & operator *= (Type value);
    /**
     *
     */
    template<typename SType>
    Vertex4<Type> & operator *=(const Vertex4<SType> & vec);
    /**
     * @brief operator -=
     * @param value
     * @return
     */
    Vertex4<Type> & operator -= (Type value);
    /**
     *
     */
    template <typename SType>
    Vertex4<Type> & operator =(const Vertex4<SType> & second);
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
    Vertex4<Type> & normalize();

    template <typename SType>
    void set(const Vertex4<SType> &vec);

    void set(Type x ,Type y ,Type z ,Type w);

private:
    Type v[4];

};

template <typename Type>
Vertex4<Type> operator * (Type value, Vertex4<Type> & vec)
{
    vec *= value;
    return vec;
}


#endif // VERTEX4_H
