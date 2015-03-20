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

#include "terrainbase.h"
#include "terrainregion.h"

TerrainBase::TerrainBase(int type)
    : ModuleBase(type),nula(127)
{
    nodes_x=0;
    nodes_y=0;
    nodes_z=0;
    node_size=0;

    grid=NULL;
    node_size=32;

    cache_data=new signed char[node_size*node_size*node_size*sizeof(TerrainVoxel)*2];
    visible=true;
}

TerrainBase::~TerrainBase()
{
    delete [] grid;
    delete [] cache_data;
}

void TerrainBase::setMap(Map *map)
{
    this->map=map;
}

bool TerrainBase::build(int regions_x, int regions_y, int regions_z, int region_size)
{
    this->nodes_x=regions_x;
    this->nodes_y=regions_y;
    this->nodes_z=regions_z;
    this->node_size=region_size;

    grid=new TerrainRegion *[regions_x*regions_y*regions_z];
    for(int i=0;i<nodes_x*nodes_y*nodes_z;i++)
    {
        grid[i]=NULL;
    }
    return true;
}

void TerrainBase::decompress(TerrainRegion *region)
{
    TerrainCompress compress;
    if(region_cache.size()==128)
    {
        TerrainRegion * tmp=region_cache.front();
        region_cache.pop_front();
        //if(tmp->changed)
        {
            int size=compress.encode(TerrainCompress::SPLIT_RLE,tmp,cache_data);
            delete [] tmp->voxel_compressed;
            signed char *voxel_data=new signed char[size];
            memcpy(voxel_data, cache_data,size);
            tmp->comressed_size=size;
            tmp->voxel_compressed=voxel_data;
        }

        //recycle memory
        region->voxel_data=tmp->voxel_data;
        tmp->voxel_data=NULL;
    }
    else
        region->voxel_data=new TerrainVoxel[node_size*node_size*node_size];

   // region->changed=false;
    compress.decode(TerrainCompress::SPLIT_RLE,region,region->voxel_compressed);
    region_cache.push_back(region);
}

void TerrainBase::touchDecompress(TerrainRegion *region)
{
    for (std::list<TerrainRegion*>::iterator it = region_cache.begin(); it != region_cache.end(); it++)
    {
        if((*it)==region)
        {
            TerrainRegion * tmp=(*it);
            region_cache.erase(it);
            region_cache.push_back(tmp);
            return;
        }
    }
}

TerrainRegion *TerrainBase::createRegion(int x,int y,int z)
{
    if(grid[x*nodes_z*nodes_y +y*nodes_z+z]==NULL)
    {
        grid[x*nodes_z*nodes_y +y*nodes_z+z]=new TerrainRegion(this,x*node_size,y*node_size,z*node_size,node_size);
        map->insertObject(grid[x*nodes_z*nodes_y +y*nodes_z+z]);
    }
    return grid[x*nodes_z*nodes_y +y*nodes_z+z];
}

void TerrainBase::removeRegion(TerrainRegion *region)
{
    if(map->removeObject(region))
    {
        for (std::list<TerrainRegion*>::iterator it=region_cache.begin(); it!=region_cache.end(); ++it)
        {
            if((*it)==region)
            {
                region_cache.erase(it);
                break;
            }
        }
        grid[((region->position()[0])/node_size)*nodes_z*nodes_y +((region->position()[1])/node_size)*nodes_z+((region->position()[2])/node_size)]=NULL;
        delete region;
    }
}

void TerrainBase::setVoxel(int x, int y, int z, const TerrainVoxel *voxel)
{
    int i=x/node_size;
    int j=y/node_size;
    int k=z/node_size;

    if(i<nodes_x && j<nodes_y && k<nodes_z)
    {
        if(grid[i*nodes_z*nodes_y +j*nodes_z+k]==NULL)
            createRegion(i,j,k);

        if(x%node_size==0 && i>0 && grid[(i-1)*nodes_z*nodes_y +j*nodes_z+k]!=NULL)
        {
            grid[(i-1)*nodes_z*nodes_y +j*nodes_z+k]->dirty=true;
        }
        if(x%node_size==node_size-1 && i+1<nodes_x && grid[(i+1)*nodes_z*nodes_y +j*nodes_z+k]!=NULL)
        {
            grid[(i+1)*nodes_z*nodes_y +j*nodes_z+k]->dirty=true;
        }

        if(y%node_size==0 && j>0 && grid[i*nodes_z*nodes_y +(j-1)*nodes_z+k]!=NULL)
        {
            grid[i*nodes_z*nodes_y +(j-1)*nodes_z+k]->dirty=true;
        }
        if(y%node_size==node_size-1 && j+1<nodes_x && grid[i*nodes_z*nodes_y +(j+1)*nodes_z+k]!=NULL)
        {
            grid[i*nodes_z*nodes_y +(j+1)*nodes_z+k]->dirty=true;
        }

        if(z%node_size==0 && z>0 && grid[i*nodes_z*nodes_y +j*nodes_z+k-1]!=NULL)
        {
            grid[i*nodes_z*nodes_y +j*nodes_z+k-1]->dirty=true;
        }
        if(z%node_size==node_size-1 && k+1<nodes_x && grid[i*nodes_z*nodes_y +j*nodes_z+k+1]!=NULL)
        {
            grid[i*nodes_z*nodes_y +j*nodes_z+k+1]->dirty=true;
        }

        grid[i*nodes_z*nodes_y +j*nodes_z+k]->setVoxel(x,y,z,voxel);
    }
}

void TerrainBase::setVoxelMaterial(int x, int y, int z, const unsigned char *material)
{
    int i=x/node_size;
    int j=y/node_size;
    int k=z/node_size;

    if(i<nodes_x && j<nodes_y && k<nodes_z)
    {
        if(grid[i*nodes_z*nodes_y +j*nodes_z+k]==NULL)
            createRegion(i,j,k);

        if(x%node_size==0 && i>0 && grid[(i-1)*nodes_z*nodes_y +j*nodes_z+k]!=NULL)
        {
            grid[(i-1)*nodes_z*nodes_y +j*nodes_z+k]->dirty=true;
        }
        if(x%node_size==node_size-1 && i+1<nodes_x && grid[(i+1)*nodes_z*nodes_y +j*nodes_z+k]!=NULL)
        {
            grid[(i+1)*nodes_z*nodes_y +j*nodes_z+k]->dirty=true;
        }

        if(y%node_size==0 && j>0 && grid[i*nodes_z*nodes_y +(j-1)*nodes_z+k]!=NULL)
        {
            grid[i*nodes_z*nodes_y +(j-1)*nodes_z+k]->dirty=true;
        }
        if(y%node_size==node_size-1 && j+1<nodes_x && grid[i*nodes_z*nodes_y +(j+1)*nodes_z+k]!=NULL)
        {
            grid[i*nodes_z*nodes_y +(j+1)*nodes_z+k]->dirty=true;
        }

        if(z%node_size==0 && z>0 && grid[i*nodes_z*nodes_y +j*nodes_z+k-1]!=NULL)
        {
            grid[i*nodes_z*nodes_y +j*nodes_z+k-1]->dirty=true;
        }
        if(z%node_size==node_size-1 && k+1<nodes_x && grid[i*nodes_z*nodes_y +j*nodes_z+k+1]!=NULL)
        {
            grid[i*nodes_z*nodes_y +j*nodes_z+k+1]->dirty=true;
        }

        grid[i*nodes_z*nodes_y +j*nodes_z+k]->setVoxelMaterial(x,y,z,material);
    }
}

void TerrainBase::setVoxelValue(int x, int y, int z, int value)
{
    int i=x/node_size;
    int j=y/node_size;
    int k=z/node_size;

    if(i<nodes_x && j<nodes_y && k<nodes_z)
    {
        if(grid[i*nodes_z*nodes_y +j*nodes_z+k]==NULL)
            createRegion(i,j,k);

        //FIX for edge changing
        if(x%node_size==0 && i>0 && grid[(i-1)*nodes_z*nodes_y +j*nodes_z+k]!=NULL)
        {
            grid[(i-1)*nodes_z*nodes_y +j*nodes_z+k]->dirty=true;
        }
        if(x%node_size==node_size-1 && i<nodes_x && grid[(i+1)*nodes_z*nodes_y +j*nodes_z+k]!=NULL)
        {
            grid[(i+1)*nodes_z*nodes_y +j*nodes_z+k]->dirty=true;
        }

        if(y%node_size==0 && j>0 && grid[i*nodes_z*nodes_y +(j-1)*nodes_z+k]!=NULL)
        {
            grid[i*nodes_z*nodes_y +(j-1)*nodes_z+k]->dirty=true;
        }
        if(y%node_size==node_size-1 && j<nodes_x && grid[i*nodes_z*nodes_y +(j+1)*nodes_z+k]!=NULL)
        {
            grid[i*nodes_z*nodes_y +(j+1)*nodes_z+k]->dirty=true;
        }

        if(z%node_size==0 && z>0 && grid[i*nodes_z*nodes_y +j*nodes_z+k-1]!=NULL)
        {
            grid[i*nodes_z*nodes_y +j*nodes_z+k-1]->dirty=true;
        }
        if(z%node_size==node_size-1 && k<nodes_x && grid[i*nodes_z*nodes_y +j*nodes_z+k+1]!=NULL)
        {
            grid[i*nodes_z*nodes_y +j*nodes_z+k+1]->dirty=true;
        }

        grid[i*nodes_z*nodes_y +j*nodes_z+k]->setVoxelValue(x,y,z,value);
    }
}


int TerrainBase::regionsX() const
{
    return nodes_x;
}

int TerrainBase::regionsY() const
{
    return nodes_y;
}

int TerrainBase::regionsZ() const
{
    return nodes_z;
}

int TerrainBase::regionSize() const
{
    return node_size;
}

void TerrainBase::clear()
{
    TerrainRegion * tmp;
    for(int x=0;x<nodes_x;x++)
        for(int y=0;y<nodes_y;y++)
            for(int z=0;z<nodes_z;z++)
            {
                tmp=grid[x*nodes_z*nodes_y + y*nodes_z + z];
                if(tmp!=NULL)
                {
                    if(map->removeObject(tmp))
                    {
                        delete tmp;
                        grid[x*nodes_z*nodes_y +y*nodes_z+z]=NULL;
                    }
                    else
                    {
                        cerr<<"Unable to remove Terrain region"<<endl;
                    }
                }
            }
    region_cache.clear();
}

TerrainRegion *TerrainBase::regionAtIndex(int x, int y, int z) const
{
    if(x<nodes_x && y<nodes_y && z<nodes_z)
    {
        return grid[x*nodes_z*nodes_y +y*nodes_z+z];
    }

    return NULL;
}


TerrainRegion *TerrainBase::regionAtPosition(int x, int y, int z) const
{
    x/=node_size;
    y/=node_size;
    z/=node_size;
    return regionAtIndex(x,y,z);
}
