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
#include "plane.h"

Plane::Plane()
{
    normal.set(0.0,0.0,0.0);
    point.set(0.0,0.0,0.0);
}

void Plane::set3Points( Vertex3<float> &v1,  Vertex3<float> &v2,  Vertex3<float> &v3)
{
    Vertex3<float> aux1, aux2;

    aux1 = v1 - v2;
    aux2 = v3 - v2;

    normal = aux2 * aux1;

    normal.normalize();
    point=v2;
    d = -(normal.dot(point));
}

float Plane::distance(const Vertex3<float> &point) const
{
    return (d + normal.dot(point));
}

const Vertex3<float> &Plane::getNormal() const
{
    return normal;
}

void Plane::setNormal(const Vertex3<float> &value)
{
    normal = value;
}
