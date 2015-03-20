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

#ifndef VECTOR2_H
#define VECTOR2_H

template <typename Type>
class Vector2
{
public:
    Vector2();
    Vector2(Type x, Type y);

    /**
     * @brief operator [] get vertex element.
     * @param n
     * @return
     */
    Type & operator[](int n);

    const Type & operator[](int n) const;

    const Type *array() const;

    Vector2<Type> & operator /=(const Type num);

    Vector2<Type> & operator *=(const Type num);

    Vector2<Type> & operator +=(const Type num);

    /**
     * @brief distance Distance between this and other point
     * @param vec Other point
     * @return
     */
    template <typename SType>
    Type distance(const Vector2<SType> & vec) const;

private:
    Type v[2];
};

#endif // VECTOR2_H
