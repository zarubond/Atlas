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

#include "terrainactionsmooth.h"

#include "../terraincontrol.h"

TerrainActionSmooth::TerrainActionSmooth(const TerrainControl *terrain):
    TerrainAction(terrain)
{
}

bool TerrainActionSmooth::repeat()
{
    return true;
}

void TerrainActionSmooth::make(int x, int y, int z, float value, float fade, float opacity, unsigned char texture_v, unsigned char texture_h)
{
    if(value<0)
    {
        TerrainVoxel voxel=*getBackupVoxel(x,y,z);

        signed char a=voxel.value;
        signed char b=voxelValue(x+1,y  ,z  );
        signed char c=voxelValue(x-1,y  ,z  );

        signed char d=voxelValue(x  ,y+1,z  );
        signed char e=voxelValue(x  ,y-1,z  );

        signed char f=voxelValue(x  ,y  ,z+1);
        signed char g=voxelValue(x  ,y  ,z-1);
        unsigned char mval=ceil((fade*15.0+15.0*(-value/127.0))*opacity);
        if(mval>15) mval=15;

        if(a<0 && b<0 && c<0 && d<0 && e<0 && f<0 && g<0)
        {
            voxel.value=-127;
            setMaterial(voxel,  mval, texture_v, texture_h);
            setVoxel(x, y, z, &voxel);
            return;
        }
        else if(a>=0 && b>=0 && c>=0 && d>=0 && e>=0 && f>=0 && g>=0)
        {
            voxel.value=127;
            setMaterial(voxel,  mval, texture_v, texture_h);
            setVoxel(x, y, z, &voxel);
            return;
        }

        float sum=0;

        for(int i=-2;i<=2;i++)
            for(int j=-2;j<=2;j++)
                for(int k=-2;k<=2;k++)
                    sum+=voxelValue(x+i,y+j,z+k);

        float val=sum/125.0;
        voxel.value=val>127?127:(val<-127?-127:val);
        setMaterial(voxel, mval, texture_v, texture_h);
        setVoxel(x, y, z, &voxel);
    }
}
