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
#include "terrainactionsmooth.h"

#include "../terraintool.h"

TerrainActionSmooth::TerrainActionSmooth(const TerrainTool *terrain):TerrainAction(terrain)
{
}

void TerrainActionSmooth::make(TerrainUndo * undo,int x, int y, int z, float value,int)
{
    if(value<0)
    {
        float a=undo->value(x,y,z);
        float b=undo->value(x+1,y,z);
        float c=undo->value(x-1,y,z);

        float d=undo->value(x,y+1,z);
        float e=undo->value(x,y-1,z);

        float f=undo->value(x,y,z+1);
        float g=undo->value(x,y,z-1);

        float val=(a*20+b+c+d+e+f+g)/26;
        if(val>127)
            val=127;
        if(val<-127)
            val=-127;
        editor->getTerrain()->setVoxelValue(x,y,z,val);
    }
}
