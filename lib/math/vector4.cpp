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

#include "vector4.h"

template <typename Type>
Vector4<Type>::Vector4(Type x, Type y, Type z,Type w)
{
    v[0]=x;
    v[1]=y;
    v[2]=z;
    v[3]=w;
}

template <typename Type>
Vector4<Type>::Vector4()
{
    v[0]=0;
    v[1]=0;
    v[2]=0;
    v[3]=0;
}

template <typename Type>
Vector4<Type> & Vector4<Type>::operator /= (Type value)
{
    v[0]/=value;
    v[1]/=value;
    v[2]/=value;
    v[3]/=value;
    return *this;
}

template <typename Type>
Vector4<Type> & Vector4<Type>::operator *= (Type value)
{
    v[0]*=value;
    v[1]*=value;
    v[2]*=value;
    v[3]*=value;
    return *this;
}


template<typename Type>
template<typename SType>
Vector4<Type> & Vector4<Type>::operator *=(const Vector4<SType> & vec)
{
    Type xy = v[0]*vec.y    - vec.x*v[1];
    Type xz = v[0]*vec.z    - vec.x*v[2];
    Type xw = v[0]*vec.v[3] - vec.x*v[3];
    Type yz = v[1]*vec.z    - vec.y*v[2];
    Type yw = v[1]*vec.v[3] - vec.y*v[3];
    Type zw = v[2]*vec.v[3] - vec.z*v[3];

    v[0]=v[1]*zw - v[2]*yw + v[3]*yz;
    v[1]=v[2]*xw - v[0]*zw - v[3]*xz;
    v[2]=v[0]*yw - v[1]*xw + v[3]*xy;
    v[3]=v[1]*xz - v[0]*yz - v[2]*xy;

    return *this;
}

template <typename Type>
Vector4<Type> & Vector4<Type>::operator -= (Type value)
{
    v[0]-=value;
    v[1]-=value;
    v[2]-=value;
    v[3]-=value;
    return *this;
}


template <typename Type>
template <typename SType>
Vector4<Type> & Vector4<Type>::operator =(const Vector4<SType> & second)
{
    v[0]=second.x;
    v[1]=second.y;
    v[2]=second.z;
    v[3]=second.v[3];
    return *this;
}

template <typename Type>
Type Vector4<Type>::length() const
{
    return sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2] + v[3]*v[3]);
}

template <typename Type>
Vector4<Type> & Vector4<Type>::normalize()
{
    Type l=length();
    if(l==0) return *this;
    v[0]/=l;
    v[1]/=l;
    v[2]/=l;
    v[3]/=l;
    return *this;
}

template <typename Type>
void Vector4<Type>::set(Type x, Type y, Type z, Type w)
{
    v[0]=x;
    v[1]=y;
    v[2]=z;
    v[3]=w;
}

template <typename Type>
const Type *Vector4<Type>::array() const
{
    return v;
}

template <typename Type>
template <typename SType>
void Vector4<Type>::set(const Vector4<SType> & vec)
{
    v[0]=vec[0];
    v[1]=vec[1];
    v[2]=vec[2];
    v[3]=vec[3];
}

template <typename Type>
std::string Vector4<Type>::toString() const
{
    setlocale(LC_NUMERIC, "C");
    std::ostringstream oss;
    oss << v[0];
    oss << "/";
    oss << v[1];
    oss << "/";
    oss << v[2];
    oss << "/";
    oss << v[3];
    setlocale(LC_NUMERIC, "");
    return oss.str();
}

template <typename Type>
void Vector4<Type>::fromString(const char *str)
{
    const char * pos;
    pos=parse(v[0],str);
    if(*pos=='/')
    {
        pos=parse(v[1],pos+1);
        if(*pos=='/')
        {
            pos=parse(v[2],pos+1);
            if(*pos=='/')
                parse(v[3],pos+1);
        }
    }
}
