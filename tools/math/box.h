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
#ifndef BOX_H
#define BOX_H

#include "math.h"

class Box
{
public:
    Box();

    Vertex3<float> getVertexP(const Vertex3<float> &normal) const;
    void set(const Vertex3<float> & corner, float w, float h, float d);
    void setExtreme(float min_x,float min_y,float min_z,float max_x,float max_y,float max_z);
private:
    Vertex3<float> corner;
    float w,h,d;
};

#endif // BOX_H
