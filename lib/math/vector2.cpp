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

#include "vector2.h"

template <typename Type>
Vector2<Type>::Vector2()
{
    v[0]=0;
    v[1]=0;
}

template <typename Type>
Vector2<Type>::Vector2(Type x, Type y)
{
    v[0]=x;
    v[1]=y;
}

template <typename Type>
Type & Vector2<Type>::operator[](int n)
{
    return v[n];
}

template <typename Type>
const Type & Vector2<Type>::operator[](int n) const
{
    return v[n];
}

template <typename Type>
const Type *Vector2<Type>::array() const
{
    return v;
}

template <typename Type>
Vector2<Type> & Vector2<Type>::operator /=(const Type num)
{
    v[0]/=num;
    v[1]/=num;
    return *this;
}

template <typename Type>
Vector2<Type> & Vector2<Type>::operator *=(const Type num)
{
    v[0]*=num;
    v[1]*=num;
    return *this;
}

template <typename Type>
Vector2<Type> & Vector2<Type>::operator +=(const Type num)
{
    v[0]+=num;
    v[1]+=num;
    return *this;
}


template <typename Type>
template <typename SType>
Type Vector2<Type>::distance(const Vector2<SType> & vec) const
{
    return sqrt((vec[0]-v[0])*(vec[0]-v[0]) + (vec[1]-v[1])*(vec[1]-v[1]));
}
