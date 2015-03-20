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

#include "terrainactionsculptor.h"

#include "../terraincontrol.h"

TerrainActionSculptor::TerrainActionSculptor(const TerrainControl *editor):TerrainAction(editor)
{
}

bool TerrainActionSculptor::makeShadowTerrain()
{
    return true;
}

void TerrainActionSculptor::make(int x, int y, int z, float value, float fade, float opacity, unsigned char texture_v, unsigned char texture_h)
{
    TerrainVoxel voxel=*getBackupVoxel(x,y,z);
    if(value>=0)
    {
        float b=isovalue(x+1,y  ,z  );
        float c=isovalue(x-1,y  ,z  );

        float d=isovalue(x  ,y+1,z  );
        float e=isovalue(x  ,y-1,z  );

        float f=isovalue(x  ,y  ,z+1);
        float g=isovalue(x  ,y  ,z-1);

        if(voxel.value>=0 && value<voxel.value && (b<0 || c<0 || d<0 || e<0 || f<0 || g<0))
        {
            voxel.value=value;
            setMaterial(voxel, 15, texture_v, texture_h);
            setVoxel(x,y,z,&voxel);
        }
    }
    else
    {
        if(value<voxel.value || voxel.value>=0)
        {
            voxel.value=value;
            setMaterial(voxel, 15, texture_v, texture_h);
            setVoxel(x,y,z,&voxel);
        }
    }
}
