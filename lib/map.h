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
#ifndef MAP_H
#define MAP_H

#include <fstream>
#include <string>

#include "math/math.h"
#include "octreenode.h"
#include "mapobject.h"


#include "environment/lightmanager.h"

class Map
{
 public:
     Map();
     ~Map();
     bool setup(int width, int height,int depth);
     /**
      * @brief Select vissible object on map. Must be called before render().
      * @param camera
      */
     void buildRenderList(const Camera & camera);
     void buildRenderListAll();
     /**
      * @brief Insert object to map.
      * @param object
      */
     bool insertObject(MapObject * object);
     /**
      * @brief insertOutOfTree Out of tree object are not inserted to tree. They are all tested every frame.
      * Use this for objects which are moved very often.
      * @param object
      */
     void insertOutOfTree(MapObject * object);
     void removeOutOfTree(MapObject * object);
     /**
      * @brief removeObject Remove object from map.
      * @param object
      * @return
      */
     bool removeObject(MapObject * object);
     /**
      * @brief collision Collision with objects in map.
      * @param a
      * @param b
      * @return
      */
     Collision collision(const Vector3f a,const Vector3f b);

     /**
      * @brief collision Collision with objects in map.
      * @param a
      * @param b
      * @return
      */
     Collision collisionVisible(const Vector3f &a, const Vector3f &b, const Camera & camera);
     /**
      * @brief maxWidth Maximum map width.
      * @return
      */
     int maxWidth() const;
     /**
      * @brief maxHeight Maximum map height.
      * @return
      */
     int maxHeight() const;
     /**
      * @brief maxDepth Maximum map depth.
      * @return
      */
     int maxDepth() const;
     /**
      * @brief width Map width.
      * @return
      */
     int width() const;
     /**
      * @brief height Map height.
      * @return
      */
     int height() const;
     /**
      * @brief depth Map depth.
      * @return
      */
     int depth() const;
     /**
      * @brief nodeSize Size of leaf in map octree.
      * @return
      */
     int nodeSize() const;

     bool changed() const;
     void setChanged(bool changed);

private:
     void checkBondarys();
     void renderLights(const Matrix4f & mvp);
    int space_width,space_height,space_depth;
    MapObject * out_tree[100];
    int out_tree_count;
    bool m_changed;

     /**
      * @brief root Octreen root node.
      */
    OctreeNode root;
 };
#endif // MAP_H
