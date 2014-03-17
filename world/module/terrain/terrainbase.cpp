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
#include "terrainbase.h"
#include "terrainregion.h"

TerrainBase::TerrainBase(int type):ModuleBase(type)
{
    nodes_x=0;
    nodes_y=0;
    nodes_z=0;
    node_size=0;

    isolevel=0;
    nula.texture_id=0;
    nula.value=1.0;
    grid=NULL;
}

TerrainBase::~TerrainBase()
{/*
    if(grid!=NULL)
        for(int i=0;i<nodes_x*nodes_y*nodes_z;i++)
        {
            if(grid[i]!=NULL)
            {
                if(map->removeObject(grid[i]))
                    delete grid[i];
            }
        }*/
    delete [] grid;
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

void TerrainBase::setVoxel(int x, int y, int z,signed char value, unsigned char texture_id)
{
    int i=x/node_size;
    int j=y/node_size;
    int k=z/node_size;

    if(i<nodes_x && j<nodes_y && k<nodes_z)
    {
        if(grid[i*nodes_z*nodes_y +j*nodes_z+k]==NULL)
        {
            grid[i*nodes_z*nodes_y +j*nodes_z+k]
                    =new TerrainRegion(this,i*node_size,j*node_size,k*node_size,node_size);
            map->insertObject(grid[i*nodes_z*nodes_y +j*nodes_z+k]);
        }

        grid[i*nodes_z*nodes_y +j*nodes_z+k]->setVoxel(x,y,z,value,texture_id);
    }
}

void TerrainBase::setVoxelTexture(int x, int y, int z, unsigned char texture_id)
{
    int a=x/node_size;
    int b=y/node_size;
    int c=z/node_size;

    if(a<nodes_x && b<nodes_y && c<nodes_z)
    {
        if(grid[a*nodes_z*nodes_y +b*nodes_z+c]==NULL)
        {
            grid[a*nodes_z*nodes_y +b*nodes_z+c]
                        =new TerrainRegion(this,a*node_size,b*node_size,c*node_size,node_size);
            map->insertObject(grid[a*nodes_z*nodes_y +b*nodes_z+c]);
        }

        grid[a*nodes_z*nodes_y +b*nodes_z+c]->setVoxelTexture(x,y,z,texture_id);
    }
}

void TerrainBase::setVoxelValue(int x, int y, int z, signed char value)
{
    int a=x/node_size;
    int b=y/node_size;
    int c=z/node_size;

    if(a<nodes_x && b<nodes_y && c<nodes_z)
    {
        if(grid[a*nodes_z*nodes_y +b*nodes_z+c]==NULL)
        {
            grid[a*nodes_z*nodes_y +b*nodes_z+c]
                    =new TerrainRegion(this,a*node_size,b*node_size,c*node_size,node_size);
            map->insertObject(grid[a*nodes_z*nodes_y +b*nodes_z+c]);
        }

        grid[a*nodes_z*nodes_y +b*nodes_z+c]->setVoxelValue(x,y,z,value);
    }
}

TerrainVoxel *TerrainBase::getVoxel(int x, int y, int z)
{
    if(x<0) x=0;
    if(y<0) y=0;
    if(z<0) z=0;

    int a=x/node_size;
    int b=y/node_size;
    int c=z/node_size;

    if(a>=nodes_x)
    {
        a=nodes_x-1;
        x=nodes_x*node_size-1;
    }
    if(b>=nodes_y)
    {
        b=nodes_y-1;
        y=nodes_y*node_size-1;
    }
    if(c>=nodes_z)
    {
        c=nodes_z-1;
        z=nodes_z*node_size-1;
    }

    if(grid[a*nodes_z*nodes_y+b*nodes_z+c]!=NULL)
    {
        TerrainVoxel *voxel=grid[a*nodes_z*nodes_y+b*nodes_z+c]->voxelField();
        int i=(x-a*node_size)*(node_size)*(node_size)+(y-b*node_size)*(node_size)+z-c*node_size;
        return &voxel[i];
    }

    return &nula;
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

float TerrainBase::getIsoLevel() const
{
    return isolevel;
}

void TerrainBase::clear()
{
    TerrainRegion * tmp;
    for(int x=0;x<nodes_x;x++)
        for(int y=0;y<nodes_y;y++)
            for(int z=0;z<nodes_z;z++)
            {
                tmp=grid[x*nodes_z*nodes_y +y*nodes_z+z];
                if(tmp!=NULL)
                {
                    if(map->removeObject(tmp))
                    {
                        delete tmp;
                        grid[x*nodes_z*nodes_y +y*nodes_z+z]=NULL;
                    }
                    else
                    {
                        std::cerr<<"Unable to remove Terrain region"<<std::endl;
                    }
                }
            }
}

TerrainRegion *TerrainBase::regionAtIndex(unsigned short x, unsigned short y, unsigned short z) const
{
    if(x<nodes_x && y<nodes_y && z<nodes_z)
    {
        return grid[x*nodes_z*nodes_y +y*nodes_z+z];
    }

    return NULL;
}


TerrainRegion *TerrainBase::regionAtPosition(unsigned short x, unsigned short y, unsigned short z) const
{
    x/=node_size;
    y/=node_size;
    z/=node_size;
    return regionAtIndex(x,y,z);
}
