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
#ifndef PLANE_H
#define PLANE_H

#include "math.h"

class Plane
{
public:
    Plane();
    /**
     * @brief set3Points Set three points defining plane
     * @param v1
     * @param v2
     * @param v3
     */
    void set3Points(Vertex3<float> &v1, Vertex3<float> &v2, Vertex3<float> &v3);
    /**
     * @brief distance Get distance from plane to point.
     * @param point
     * @return
     */
    float distance(const Vertex3<float> &point) const;

    const Vertex3<float> &getNormal() const;
    void setNormal(const Vertex3<float> &value);

private:
    Vertex3<float> normal,point;
    float d;
};

#endif // PLANE_H
