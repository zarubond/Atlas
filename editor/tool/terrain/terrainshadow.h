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
#ifndef TERRAINSHADOW_H
#define TERRAINSHADOW_H

#include "../../../lib/module/terrain/terrainbase.h"
#include <vector>
#include <stack>
/**
 * @brief The TerrainShadow class
 * This is for keeping original surface durring continuous editing.
 */
class TerrainCache
{
public:
    TerrainCache();
    ~TerrainCache();
    /**
     * @brief setTerrain Set terrain to be shadowed while editiong
     * @param base
     */
    void setTerrain(TerrainBase *base);
    /**
     * @brief backupRegionAtPosition Tests if the region was cached, if not than the region containing this point will be cached.
     * @param x
     * @param y
     * @param z
     */
    void backupRegionAtPosition(int x,int y,int z);
    /**
     * @brief swapTerrainData Move region voxel data from terrain regions to cache or the other way
     */
    void swapTerrainData();
    /**
     * @brief clear Clear all cached region voxel data.
     */
    void clear();
private:
    struct RegionBackup{
        RegionBackup():voxel_data(NULL), index_x(0), index_y(0), index_z(0){}
        RegionBackup(int x, int y, int z):voxel_data(NULL), index_x(x), index_y(y), index_z(z){}
        TerrainVoxel * voxel_data;
        int index_x, index_y, index_z;
    };
    TerrainCache::RegionBackup *regionAtIndex(int x, int y, int z) const;
    TerrainCache::RegionBackup *regionAtPosition(unsigned int x, unsigned int y, unsigned int z) const;

    std::stack<TerrainVoxel*> cache;
    std::vector<RegionBackup*> storage;
    RegionBackup * grid;
    TerrainBase * base;
};

#endif // TERRAINSHADOW_H
