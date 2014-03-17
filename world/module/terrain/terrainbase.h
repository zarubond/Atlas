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
#ifndef TERRAINBASE_H
#define TERRAINBASE_H

#include "../../../tools/math/math.h"
#include "terrainregion.h"
#include "../partmanager.h"
#include "../../map.h"

class TerrainBase: public ModuleBase
{
 public:
     TerrainBase(int type);
     virtual ~TerrainBase();

     void setMap(Map * map);
     /**
      * @brief regionsX
      * @return Number of regions on x axes.
      */
     int regionsX() const;
     /**
      * @brief regionsY
      * @return Nnmber of regions on y axes.
      */
     int regionsY() const;
     /**
      * @brief regionsZ
      * @return Number of regions on z axes.
      */
     int regionsZ() const;
     /**
      * @brief regionSize
      * @return Size of one region.
      */
     int regionSize() const;
     /**
      * @brief getIsoLevel
      * @return IsoLevel for marching cubes.
      */
     float getIsoLevel() const;
     /**
      * @brief clear Clear all data about terrain.
      */
     void clear();
     /**
      * @brief getVoxel Get voxel at given position in space.
      * @param x
      * @param y
      * @param z
      * @return If found pointer to the voxel, otherwise pointer to 0 voxel.
      */
     TerrainVoxel * getVoxel(int x, int y, int z);
     /**
      * @brief regionAtIndex Region with given index in region array.
      * @param x
      * @param y
      * @param z
      * @return
      */
     TerrainRegion * regionAtIndex(unsigned short x, unsigned short y, unsigned short z) const;
     /**
      * @brief regionAtPosition Get region on given position i map.
      * @param x
      * @param y
      * @param z
      * @return
      */
     TerrainRegion * regionAtPosition(unsigned short x, unsigned short y, unsigned short z) const;
     /**
      * @brief setVoxelValue Set voxel isovalue at given position.
      * @param x
      * @param y
      * @param z
      * @param value
      */
     void setVoxelValue(int x, int y, int z, signed char value);
     /**
      * @brief setVoxelTexture Set voxel texture at given position.
      * @param x
      * @param y
      * @param z
      * @param texture_id
      */
     void setVoxelTexture(int x, int y, int z, unsigned char texture_id);
     /**
      * @brief setVoxel Set voxel at given position.
      * @param x
      * @param y
      * @param z
      * @param value
      * @param texture_id
      */
     void setVoxel(int x, int y, int z, signed char value, unsigned char texture_id);

     /**
      * @brief build Creates terrain with given size.
      * @param regions_x Number of region on x axes.
      * @param regions_y Number of region on y axes.
      * @param regions_z Number of region on z axes.
      * @param region_size Size of one region.
      * @return Success.
      */
     bool build(int regions_x, int regions_y, int regions_z, int region_size);

protected:
     TerrainRegion ** grid;

     //number of nodes per axe
     unsigned short nodes_x,nodes_y,nodes_z;
     unsigned short node_size;

     float isolevel;

     TerrainVoxel nula;
     Map * map;
};

#endif // TERRAIN_H
