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

#include "terrainshadow.h"

TerrainCache::TerrainCache()
{
    grid=NULL;
    base=NULL;
}

TerrainCache::~TerrainCache()
{
    clear();
    while(!cache.empty())
    {
        delete [] (cache.top());
        cache.pop();
    }

    delete [] grid;
}

void TerrainCache::setTerrain(TerrainBase *base)
{
    delete [] grid;
    grid=NULL;

    if(base==NULL)
    {
        this->base=NULL;
    }
    else
    {
        this->base=base;
        int n=base->regionsX()*base->regionsY()*base->regionsZ();
        grid=new RegionBackup[n];

        for(int x=0;x<base->regionsX();x++)
            for(int y=0;y<base->regionsY();y++)
                for(int z=0;z<base->regionsZ();z++)
                {
                    grid[x*base->regionsZ()*base->regionsY() +y*base->regionsZ()+z]=RegionBackup(x,y,z);
                }
    }
}

void TerrainCache::backupRegionAtPosition(int x, int y, int z)
{
    RegionBackup * backup=regionAtPosition(x,y,z);

    if(backup==NULL || backup->voxel_data!=NULL)
        return;

    TerrainRegion * region=base->regionAtIndex(x/base->regionSize(),y/base->regionSize(),z/base->regionSize());
    if(region==NULL)
        region=base->createRegion(x/base->regionSize(),y/base->regionSize(),z/base->regionSize());

    int n=base->regionSize();
    n=n*n*n;

    region->decompress();

    if(cache.empty())
        backup->voxel_data=new TerrainVoxel[n];
    else
    {
        backup->voxel_data=cache.top();
        cache.pop();
    }

    memcpy(backup->voxel_data,region->voxelData(),n*sizeof(TerrainVoxel));
    storage.push_back(backup);
}

void TerrainCache::swapTerrainData()
{
    RegionBackup * backup;
    TerrainRegion * region;
    for(std::vector<RegionBackup*>::iterator it = storage.begin(); it != storage.end(); ++it)
    {
        backup=(*it);
        region=base->regionAtIndex(backup->index_x,backup->index_y,backup->index_z);
        if(region==NULL)
            region=base->createRegion(backup->index_x,backup->index_y,backup->index_z);
        region->decompress();
        TerrainVoxel * tmp=region->voxelData();
        region->setVoxelData(backup->voxel_data);
        backup->voxel_data=tmp;
    }
}

void TerrainCache::clear()
{
    RegionBackup* region;
    for(std::vector<RegionBackup*>::iterator it = storage.begin(); it != storage.end(); ++it)
    {
        region=(*it);
        cache.push(region->voxel_data);
        region->voxel_data=NULL;
    }
    storage.clear();
}

TerrainCache::RegionBackup *TerrainCache::regionAtIndex(int x, int y, int z) const
{
    if(x<base->regionsX() && y<base->regionsY() && z<base->regionsZ())
    {
        return &grid[x*base->regionsZ()*base->regionsY() +y*base->regionsZ()+z];
    }

    return NULL;
}

TerrainCache::RegionBackup *TerrainCache::regionAtPosition(unsigned int x, unsigned int y, unsigned int z) const
{
    x/=base->regionSize();
    y/=base->regionSize();
    z/=base->regionSize();
    return regionAtIndex(x,y,z);
}
