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
#include "terrainactionincrease.h"

#include "../terraintool.h"

TerrainActionIncrease::TerrainActionIncrease(const TerrainTool *terrain):TerrainAction(terrain)
{
}

void TerrainActionIncrease::make(TerrainUndo * undo,int x, int y, int z, float value, int)
{
    if(value<editor->getTerrain()->getIsoLevel())
    {
        float val=undo->value(x,y,z)-1;
        if(val>=-127)
            editor->getTerrain()->setVoxelValue(x,y,z,val);
    }
}
