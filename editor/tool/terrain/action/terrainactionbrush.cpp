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

#include "terrainactionbrush.h"

#include "../terraincontrol.h"

TerrainActionBrush::TerrainActionBrush(const TerrainControl *terrain)
    : TerrainAction(terrain)
{
}

bool TerrainActionBrush::makeShadowTerrain()
{
    return true;
}

void TerrainActionBrush::make(int x, int y, int z, float value, float fade, float opacity, unsigned char texture_v, unsigned char texture_h)
{
    if(value<0)
    {
        TerrainVoxel voxel=*getBackupVoxel(x,y,z);
        unsigned char val=ceil((fade*15.0+15.0*(-value/127.0))*opacity);
        if(val>15) val=15;
        setMaterial(voxel, val, texture_v, texture_h);
        setVoxel(x,y,z,&voxel);
    }
}
