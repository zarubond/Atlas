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
#ifndef MAPOBJECT_H
#define MAPOBJECT_H

#include "math/math.h"
#include "tools/camera.h"

#include <stdlib.h>

class MapObject;
/**
 * @brief The Collision struct if object with information about collision.
 */
struct Collision
{
    /**
     * @brief object Pointer to colided object if found, or NULL.
     */
    MapObject * object;
    /**
     * @brief collision Point of collision or end of ray.
     */
    Vector3f collision;
    /**
     * @brief normal Normal at collision point
     */
    Vector3f normal;
};

class MapObject
{
public:
    MapObject();

    virtual ~MapObject();

    /**
     * @brief inBox resolves if the object is in cube.
     * @param x Lower left x cube coordinate.
     * @param y Lower left x cube coordinate.
     * @param z Lower left x cube coordinate.
     * @param w Length of cube edge.
     * @return Is object in cube.
     */
    virtual bool inBox(int x,int y,int z,int w) const;

    virtual bool inBox(const Cube & bbox) const;
    /**
     * @brief Object collision with ray.
     * @param a Ray start.
     * @param b Ray end.
     * @return Intersection point with object of end of ray (b).
     */
    virtual void collision(const Vector3f & a,Collision & b);
    /**
     * @brief Select objects to be drawn.
     * @param camera Current view camera.
     */
    virtual void selectDraw(const Camera & camera)=0;
    virtual void selectDraw()=0;
    /**
     * @brief type Type of the object
     */
    virtual int type()=0;
    /**
     * @brief For object list in octree.
     */
    //MapObject * next;
protected:
};



#endif // MAPOBJECT_H
