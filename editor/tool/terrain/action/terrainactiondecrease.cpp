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

#include "terrainactiondecrease.h"
#include "../terraincontrol.h"

TerrainActionDecrease::TerrainActionDecrease(const TerrainControl *terrain): TerrainAction(terrain)
{
}

bool TerrainActionDecrease::repeat()
{
    return false;
}

bool TerrainActionDecrease::makeShadowTerrain()
{
    return false;
}

void TerrainActionDecrease::make(int x, int y, int z, float value, float fade, float opacity, unsigned char texture_v, unsigned char texture_h)
{
    if(value>0) return;

    TerrainVoxel voxel=*getBackupVoxel(x,y,z);
    setMaterial(voxel, 15, texture_v, texture_h);

    signed char b=voxelValue(x+1,y  ,z  );
    signed char c=voxelValue(x-1,y  ,z  );

    signed char d=voxelValue(x  ,y+1,z  );
    signed char e=voxelValue(x  ,y-1,z  );

    signed char f=voxelValue(x  ,y  ,z+1);
    signed char g=voxelValue(x  ,y  ,z-1);

    if(voxel.value<0 && b<0 && c<0 && d<0 && e<0 && f<0 && g<0)
    {
        voxel.value=-127;
        setVoxel(x, y, z, &voxel);
        return;
    }
    else if(voxel.value>=0 && b>=0 && c>=0 && d>=0 && e>=0 && f>=0 && g>=0)
    {
        voxel.value=127;
        setVoxel(x, y, z, &voxel);
        return;
    }

    float t=abs(voxel.value);
    int val=voxel.value;
    val+=1.0+t*0.5;
    voxel.value=val>127? 127: val;
    setVoxel(x, y, z, &voxel);

}
