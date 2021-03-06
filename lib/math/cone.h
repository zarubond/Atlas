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

#ifndef CONE_H
#define CONE_H

#include "mathdefine.h"

#include "vector3.h"

class Cone
{
public:
    Cone();
    /**
     * @brief create
     * @param height
     * @param radius
     * @param data {pos_x, pos_y,pos_x, normal_x, normal_y, normal_z},{pos_x, pos_y,pos_x, normal_x, normal_y, normal_z}...
     */
    static void triangleMesh(float *&data, int &count, int step, float height, float radius, const Vector3<float> & position=Vector3<float>(0.0,0.0,0.0));
};

#endif // CONE_H
