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

//#include "vertex3.h"

template <typename Type>
Vector3<Type>::Vector3()
{
    v[0]=0;
    v[1]=0;
    v[2]=0;
}

template <typename Type>
Vector3<Type>::Vector3(Type value)
{
    v[0]=value;
    v[1]=value;
    v[2]=value;
}

template <typename Type>
Vector3<Type>::Vector3(Type x, Type y, Type z)
{
    v[0]=x;
    v[1]=y;
    v[2]=z;
}

template <typename Type>
template <typename SType>
Vector3<Type>::Vector3(const Vector3<SType> & vec)
{
    v[0]=vec[0];
    v[1]=vec[1];
    v[2]=vec[2];
}

template <typename Type>
template <typename SType>
Vector3<Type> & Vector3<Type>::operator +=(const Vector3<SType> & vec)
{
    v[0]+=vec[0];
    v[1]+=vec[1];
    v[2]+=vec[2];
    return *this;
}

template <typename Type>
template <typename SType>
Vector3<Type> & Vector3<Type>::operator =(const Vector3<SType> & second)
{
    v[0]=second[0];
    v[1]=second[1];
    v[2]=second[2];
    return *this;
}

template <typename Type>
template <typename SType>
Vector3<Type> & Vector3<Type>::operator -=(const Vector3<SType> & vec)
{
    v[0]-=vec[0];
    v[1]-=vec[1];
    v[2]-=vec[2];
    return *this;
}

template <typename Type>
template <typename SType>
Vector3<Type> & Vector3<Type>::operator *=(const Vector3<SType> & vec)
{
    float a=v[0],b=v[1],c=v[2];
    v[0]=b*vec.v[2] - c*vec.v[1];
    v[1]=c*vec.v[0] - a*vec.v[2];
    v[2]=a*vec.v[1] - b*vec.v[0];
    return *this;
}

template <typename Type>
Vector3<Type> & Vector3<Type>::operator /=(const Type num)
{
    v[0]/=num;
    v[1]/=num;
    v[2]/=num;
    return *this;
}

template <typename Type>
template <typename SType>
Vector3<Type> & Vector3<Type>::operator /=(const Vector3<SType> & vec)
{
    v[0]/=vec[0];
    v[1]/=vec[1];
    v[2]/=vec[2];
    return *this;
}

template <typename Type>
Vector3<Type> & Vector3<Type>::operator *=(const Type num)
{
    v[0]*=num;
    v[1]*=num;
    v[2]*=num;
    return *this;
}

template <typename Type>
Vector3<Type> & Vector3<Type>::operator +=(const Type num)
{
    v[0]+=num;
    v[1]+=num;
    v[2]+=num;
    return *this;
}

template <typename Type>
bool Vector3<Type>::operator ==(const Vector3<Type> & vec) const
{
    if(v[0]!=vec.v[0] || v[1]!=vec.v[1] || v[2]!=vec.v[2])
        return false;
    return true;
}

template <typename Type>
bool Vector3<Type>::operator !=(const Vector3<Type> & vec) const
{
    if(v[0]!=vec.v[0] || v[1]!=vec.v[1] || v[2]!=vec.v[2])
        return true;
    return false;
}

template <typename Type>
template <typename SType>
const Vector3<Type> Vector3<Type>::operator-(const Vector3<SType> & vec) const
{
    return Vector3<Type>(v[0]-vec[0], v[1]-vec[1], v[2]-vec[2]);
}

template <typename Type>
template <typename SType>
const Vector3<Type> Vector3<Type>::operator+(const Vector3<SType> & vec) const
{
    return Vector3<Type>(v[0]+vec[0], v[1]+vec[1], v[2]+vec[2]);
}

template <typename Type>
template <typename SType>
const Vector3<Type> Vector3<Type>::operator*(const Vector3<SType> & vec) const
{
    return Vector3<Type>(v[1]*vec.v[2] - v[2]*vec.v[1],v[2]*vec.v[0] - v[0]*vec.v[2],v[0]*vec.v[1] - v[1]*vec.v[0]);
}

template <typename Type>
void Vector3<Type>::set(Type x,Type y,Type z)
{
    v[0]=x;
    v[1]=y;
    v[2]=z;
}

template <typename Type>
Type Vector3<Type>::dot(const Vector3<Type> & vec) const
{
    return v[0]*vec.v[0]+v[1]*vec.v[1]+v[2]*vec.v[2];
}

template <typename Type>
void Vector3<Type>::normalize()
{
    const Type l=length();
    if(l==0) return;
    v[0]/=l;
    v[1]/=l;
    v[2]/=l;
}

template <typename Type>
void Vector3<Type>::inverse()
{
    v[0]*=-1;
    v[1]*=-1;
    v[2]*=-1;
}

template <typename Type>
Type Vector3<Type>::length() const
{
    Type val=v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
    return sqrt(val);
}

template <typename Type>
void Vector3<Type>::abs()
{
    v[0]=fabs(v[0]);
    v[1]=fabs(v[1]);
    v[2]=fabs(v[2]);
}

template <typename Type>
const Type *Vector3<Type>::array()const
{
    return v;
}

template <typename Type>
std::string Vector3<Type>::toString()
{
    setlocale(LC_NUMERIC, "C");
    std::ostringstream oss;
    oss << v[0];
    oss << "/";
    oss << v[1];
    oss << "/";
    oss << v[2];
    setlocale(LC_NUMERIC, "");
    return oss.str();
}

template <typename Type>
void Vector3<Type>::fromString(const char *str)
{
    const char * pos;
    pos=parse(v[0],str);
    if(*pos=='/')
    {
        pos=parse(v[1],pos+1);
        if(*pos=='/')
            parse(v[2],pos+1);
    }
}

template <typename Type>
Type Vector3<Type>::x()
{
    return v[0];
}

template <typename Type>
Type Vector3<Type>::y()
{
    return v[1];
}

template <typename Type>
Type Vector3<Type>::z()
{
    return v[2];
}

template <typename Type>
template <typename SType>
Type Vector3<Type>::distance(const Vector3<SType> & vec) const
{
    return sqrt((vec[0]-v[0])*(vec[0]-v[0]) + (vec[1]-v[1])*(vec[1]-v[1]) + (vec[2]-v[2])*(vec[2]-v[2]));
}
