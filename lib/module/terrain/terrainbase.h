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

#ifndef TERRAINBASE_H
#define TERRAINBASE_H

#include <list>

#include "../../math/math.h"
#include "terrainregion.h"
#include "terraincompress.h"
#include "../modulebase.h"
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
     const TerrainVoxel *getVoxel(int x, int y, int z) const;
     /**
      * @brief regionAtIndex Region with given index in region array.
      * @param x
      * @param y
      * @param z
      * @return
      */
     TerrainRegion * regionAtIndex(int x, int y, int z) const;
     /**
      * @brief regionAtPosition Get region on given position i map.
      * @param x
      * @param y
      * @param z
      * @return
      */
     TerrainRegion * regionAtPosition(int x, int y, int z) const;
     /**
      * @brief setVoxelValue Set voxel isovalue at given position.
      * @param x
      * @param y
      * @param z
      * @param value
      */
     void setVoxelValue(int x, int y, int z, int value);
     /**
      * @brief setVoxel Set voxel at given position.
      * @param x
      * @param y
      * @param z
      * @param value
      * @param texture_id
      */
     void setVoxel(int x, int y, int z, const TerrainVoxel * voxel);

     void setVoxelMaterial(int x, int y, int z, const unsigned char * material);

     /**
      * @brief build Creates terrain with given size.
      * @param regions_x Number of region on x axes.
      * @param regions_y Number of region on y axes.
      * @param regions_z Number of region on z axes.
      * @param region_size Size of one region.
      * @return Success.
      */
     bool build(int regions_x, int regions_y, int regions_z, int region_size);

     void decompress(TerrainRegion * region);
     void touchDecompress(TerrainRegion * region);

     virtual TerrainRegion * createRegion(int x, int y, int z);
protected:
     void removeRegion(TerrainRegion * region);

     TerrainRegion ** grid;

     //number of nodes per axe
     int nodes_x,nodes_y,nodes_z;
     int node_size;

     std::list<TerrainRegion*> region_cache;

     signed char *cache_data;

     const TerrainVoxel nula;
     Map * map;
};

inline const TerrainVoxel *TerrainBase::getVoxel(int x, int y, int z) const
{
    //modulo is much slower
/*    if(x<0) x+=nodes_x*node_size;
    else if(x>=nodes_x*node_size) x-=(nodes_x*node_size);
    if(y<0) y+=nodes_y*node_size;
    else if(y>=nodes_y*node_size) y-=(nodes_y*node_size);
    if(z<0) z+=nodes_z*node_size;
    else if(z>=nodes_z*node_size) z-=(nodes_z*node_size);

    x+=nodes_x*node_size*((x<0)-(x>=nodes_x*node_size));
    y+=nodes_y*node_size*((y<0)-(y>=nodes_y*node_size));
    z+=nodes_z*node_size*((z<0)-(z>=nodes_z*node_size));
    x=std::max(0, std::min(x, nodes_x*node_size-1));
    y=std::max(0, std::min(y, nodes_y*node_size-1));
    z=std::max(0, std::min(z, nodes_z*node_size-1));*/

    //clamp to zero
/*
    x+=(x>>31)*x;
    y+=(z>>31)*y;
    z+=(z>>31)*z;
    if(x>=nodes_x*node_size) x=nodes_x*node_size-1;
    if(y>=nodes_y*node_size) y=nodes_y*node_size-1;
    if(z>=nodes_z*node_size) z=nodes_z*node_size-1;
    //x-=(x-nodes_x*node_size+1)*(x>=nodes_x*node_size);
    //y-=(y-nodes_y*node_size+1)*(y>=nodes_y*node_size);
    //z-=(z-nodes_z*node_size+1)*(z>=nodes_z*node_size);
*/
    if(x<0)x=0; else if(x>=nodes_x*node_size) x=nodes_x*node_size-1;
    if(y<0)y=0; else if(y>=nodes_y*node_size) y=nodes_y*node_size-1;
    if(z<0)z=0; else if(z>=nodes_z*node_size) z=nodes_z*node_size-1;

    const int a=x/node_size;
    const int b=y/node_size;
    const int c=z/node_size;
    TerrainRegion * tmp=grid[a*nodes_z*nodes_y+b*nodes_z+c];
    if(tmp!=NULL)
    {
        tmp->decompress();
        return tmp->voxelRaw(x-node_size*a,y-node_size*b,z-node_size*c);
    }
    return &nula;
}

#endif // TERRAIN_H
