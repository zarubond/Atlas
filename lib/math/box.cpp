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

#include "box.h"

Box::Box()
{
}

Vector3<float> Box::getVertexP(const Vector3<float> &normal) const
{
    Vector3<float> res = corner;

    if (normal[0] > 0)
        res[0] += w;

    if (normal[1] > 0)
        res[1] += h;

    if (normal[2] > 0)
        res[2] += d;

    return res;
}

void Box::set(const Vector3<float> &corner, float w, float h, float d)
{
    this->corner=corner;

    if (w < 0.0)
    {
        w = -w;
        this->corner[0] -= w;
    }
    if (h < 0.0)
    {
        h = -h;
        this->corner[1] -= h;
    }
    if (d < 0.0)
    {
        d = -d;
        this->corner[2] -= d;
    }
    this->w = w;
    this->h = h;
    this->d = d;
}

void Box::setExtreme(float min_x, float min_y, float min_z, float max_x, float max_y, float max_z)
{
    set(Vector3<float>(min_x,min_y,min_z),max_x-min_x,max_y-min_y,max_z-min_z);
}
