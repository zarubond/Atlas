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

#include "grassbase.h"

GrassBase::GrassBase(int type):ModuleBase(type)
{
    region_size=0;
    nodes_x=0;
    nodes_y=0;
    nodes_z=0;
    grid=NULL;
}

GrassBase::~GrassBase()
{
    delete [] grid;
}

void GrassBase::setMap(Map *map)
{
    this->map=map;
}

void GrassBase::build(unsigned short nodes_x, unsigned short nodes_y, unsigned short nodes_z, unsigned short node_size)
{
    this->nodes_x=nodes_x;
    this->nodes_y=nodes_y;
    this->nodes_z=nodes_z;
    this->region_size=node_size;

    grid=new GrassRegion *[nodes_x*nodes_y*nodes_z];

    for(int i=0;i<nodes_x*nodes_y*nodes_z;i++)
        grid[i]=NULL;
}

void GrassBase::createRegion(int x, int y, int z)
{

}

void GrassBase::addTuft(const Vector3f &position)
{
    int x=position[0],y=position[1],z=position[2];
    int i=x/region_size;
    int j=y/region_size;
    int k=z/region_size;

    if(i<nodes_x && j<nodes_y && k<nodes_z)
    {
        if(grid[i*nodes_z*nodes_y +j*nodes_z+k]==NULL)
            createRegion(i,j,k);

        grid[i*nodes_z*nodes_y +j*nodes_z+k]->addTuft(position);
    }
}

bool GrassBase::removeTuft(const Vector3f &position)
{
    int x=position[0],y=position[1],z=position[2];
    int i=x/region_size;
    int j=y/region_size;
    int k=z/region_size;

    if(i<nodes_x && j<nodes_y && k<nodes_z)
    {
        if(grid[i*nodes_z*nodes_y +j*nodes_z+k]!=NULL)
            return grid[i*nodes_z*nodes_y +j*nodes_z+k]->removeTuft(position);
    }
    return false;
}

GrassRegion *GrassBase::getRegion(int x, int y, int z)
{
    return grid[x*nodes_z*nodes_y +y*nodes_z+z];
}

unsigned short GrassBase::nodesX()
{
    return nodes_x;
}

unsigned short GrassBase::nodesY()
{
    return nodes_y;
}

unsigned short GrassBase::nodesZ()
{
    return nodes_z;
}
