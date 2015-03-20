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

#ifndef SPHERE_H
#define SPHERE_H

#include "vector3.h"
#include "math.h"

class Sphere
{
public:
    Sphere();
    Sphere(Vector3<float> center,float radius);
    /**
     * @brief set Set matrix paramethers.
     * @param center
     * @param radius
     */
    void set(Vector3<float> center,float radius);
    void set(float x,float y,float z,float radius);
    Vector3<float> intersection(const Vector3<float> &start, const Vector3<float> &end);
    /**
     * @brief createSphere Get sphere points
     * @param vertices
     * @param count
     */
    void createSphere(float *&vertices, int &count);
    /**
     * @brief inside Detect if point is inside sphere.
     * @param point
     * @return
     */
    bool inside(Vector3<float> point);
    /**
     * @brief getRadius Radius of sphere.
     * @return
     */
    float getRadius() const;
    /**
     * @brief getPosition Position of the sphere's center.
     * @return
     */
    const Vector3<float> &getPosition() const;
private:
    Vector3<float> center;
    float radius;
};

#endif // SPHERE_H
