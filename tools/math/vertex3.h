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
#ifndef VERTEX3_H
#define VERTEX3_H

#include <cmath>
#include <iostream>

template <typename T = float>
class Vertex3;


template <typename Type>
Vertex3<Type> operator*(const Vertex3<Type> &vertex,Type value);

template <typename Type>
class Vertex3{

public:
    Vertex3(Type x, Type y, Type z);
    Vertex3();
    Vertex3(Type value);
    template <typename SType>
    Vertex3<Type>(const Vertex3<SType> & vec);

    template <typename SType>
    Vertex3<Type> & operator +=(const Vertex3<SType> & vec);

    template <typename SType>
    Vertex3<Type> & operator =(const Vertex3<SType> & second);
    template <typename SType>
    Vertex3<Type> & operator -=(const Vertex3<SType> & vec);

    template <typename SType>
    Vertex3<Type> & operator *=(const Vertex3<SType> & vec);

    Vertex3<Type> & operator /=(const Type num);

    template <typename SType>
    Vertex3<Type> & operator /=(const Vertex3<SType> & vec);

    Vertex3<Type> & operator *=(const Type num);

    Vertex3<Type> & operator +=(const Type num);
    /**
     * @brief operator == Compare two vertices.
     * @param vec
     * @return
     */
    bool operator ==(const Vertex3<Type> & vec) const;

    bool operator !=(const Vertex3<Type> & vec) const;

    template <typename SType>
    const Vertex3<Type> operator-(const Vertex3<SType> & vec) const;

    template <typename SType>
    const Vertex3<Type> operator+(const Vertex3<SType> & vec) const;

    template <typename SType>
    const Vertex3<Type> operator*(const Vertex3<SType> & vec) const;

    void set(Type x,Type y,Type z);
    /**
     * @brief dot Vector dot product.
     * @param vec
     * @return
     */
    Type dot(const Vertex3<Type> & vec) const;

    void normalize();
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
    /**
     * @brief print Print vector to shell.
     */
    void print() const;

    //friend Vertex3<Type> (::operator* <>) (Type value, const Vertex3<Type> &vertex);
    friend Vertex3<Type> (::operator* <>) (const Vertex3<Type> &vertex, Type value);
private:
    Type v[3];
};

template <typename Type>
Vertex3<Type> operator*(const Vertex3<Type> &vertex,Type value)
{
    return Vertex3<Type>(value*vertex[0], value*vertex[1], value*vertex[2]);
}


#endif // VERTEX3_H
