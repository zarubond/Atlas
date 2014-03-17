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
#ifndef OCTREENODE_H
#define OCTREENODE_H

#include <iostream>
#include <inttypes.h>
#include <iostream>
#include <fstream>
#include <string>

#include "../tools/math/math.h"
#include "../tools/graphics.h"

#include "complement/light.h"
#include "mapobject.h"


class OctreeNode
{
public:
    OctreeNode();
    OctreeNode(unsigned short x, unsigned short y,unsigned short z, unsigned short node_size,const unsigned short leaf_size);
    ~OctreeNode();
    /**
     * @brief Build octree.
     * @param x Position x.
     * @param y Position y.
     * @param z Position z.
     * @param edge_size Root cube size.
     */
    void build(unsigned short x, unsigned short y, unsigned short z, unsigned short node_size, const unsigned short leaf_size);
    /**
     * @brief Add object to map
     * @param obj
     * @return Success
     */
    bool add(MapObject *obj);
    /**
     * @brief remove Remove object from tree based on position.
     * @param obj
     * @return Seccess
     */
    bool remove(MapObject * obj);
    /**
     * @brief removeForce Remove object from where ever it is
     * @param obj
     * @return
     */
    bool removeForce(MapObject * obj);
    /**
     * @brief Add light to map.
     * @param light
     * @return Success
     */
    bool add(Light * light);

    /**
     * @brief selectDraw Selects objects in tree to be drawn.
     * @param camera
     */
    void selectVisible(const Camera & camera);
    /**
     * @brief collision Tree object collision with ray.
     * @param a Ray start.
     * @param b Ray end.
     * @return Ray-object collision or ray end.
     */
    void collision(const Vertex3f & a, Collision &b);
    /**
     * @brief collisionVisible Collide ray with visible only objects.
     * @param a Ray start.
     * @param b Ray end.
     * @param camera
     */
    void collisionVisible(const Vertex3f & a, Collision &b, const Camera &camera);

private:
    MapObject * object_list;
    Cube bbox;

    OctreeNode * octant[8];
    unsigned short node_size;
    unsigned short leaf_size;
    unsigned short pos_x,pos_y,pos_z;
    Sphere sphere;
    unsigned short cnt;
};

#endif // OCTREENODE_H
