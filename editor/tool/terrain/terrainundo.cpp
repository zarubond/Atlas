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

#include "terrainundo.h"
#include "terraintool.h"

TerrainUndo::TerrainUndo(TerrainBase *terrain):terrain(terrain)
{
    backup_buffer=NULL;
    backup_size=0;
    backup_x=0;
    backup_y=0;
    backup_z=0;
    first=false;
}

TerrainUndo::~TerrainUndo()
{
    delete [] backup_buffer;
}

bool TerrainUndo::backup(int x, int y, int z, int size)
{/*
    if(size*size*size>=300000)
    {
       // cerr<<"size error terrain undo"<<endl;
        backup_size=0;
        return true;
    }
*/
    backup_buffer=new TerrainVoxel[size*size*size];

    backup_size=size;
    backup_x=x;
    backup_y=y;
    backup_z=z;

    for(int i=0; i<size; i++)
        for(int j=0; j<size; j++)
            for(int k=0; k<size; k++)
            {
                backup_buffer[i*size*size+j*size+k]=*(terrain->getVoxel(i+x,j+y,k+z));
            }
    return true;
}

signed char TerrainUndo::value(unsigned x, unsigned y, unsigned z) const
{
    x-=backup_x;
    y-=backup_y;
    z-=backup_z;

    if(x < backup_size && y < backup_size && z < backup_size)
        return backup_buffer[x*backup_size*backup_size+y*backup_size+z].value;

    return 0;
}

TerrainVoxel *TerrainUndo::voxel(unsigned x, unsigned y, unsigned z) const
{
    x-=backup_x;
    y-=backup_y;
    z-=backup_z;

    if(x < backup_size && y < backup_size && z < backup_size)
        return &backup_buffer[x*backup_size*backup_size+y*backup_size+z];

    return NULL;
}

void TerrainUndo::undo()
{
    for(unsigned x=0; x < backup_size; x++)
        for(unsigned y=0; y < backup_size; y++)
            for(unsigned z=0; z < backup_size; z++)
            {
                //terrain->setVoxel(backup_x+x,backup_y+y,backup_z+z, terrain->getVoxel(x+backup_x,y+backup_y,z+backup_z));
                terrain->setVoxel(backup_x+x,backup_y+y,backup_z+z,
                                  &backup_buffer[x*backup_size*backup_size+y*backup_size+z]);
            }
}

void TerrainUndo::redo()
{
    if(first)
    {
        undo();
    }
    first=true;
}
