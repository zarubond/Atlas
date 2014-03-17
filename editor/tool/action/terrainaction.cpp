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
#include "terrainaction.h"

#include "../terraintool.h"

TerrainAction::TerrainAction(const TerrainTool *editor):
    editor(editor)
{
}

TerrainAction::~TerrainAction()
{
}

void TerrainAction::stroke(TerrainUndo * undo,const Shape *shape)
{
    int size=ceil(shape->getRadius())+1;
    if(undo->backup(shape->minX(),shape->minY(),shape->minZ(),2*size+2))
    {
        for(int x=shape->minX(); x < shape->maxX(); x++)
            for(int y=shape->minY(); y<shape->maxY(); y++)
                for(int z=shape->minZ(); z<shape->maxZ(); z++)
                {
                    float value=shape->isovalue(x,y,z);
                    if(value>127)
                        value=127;
                    else if(value<-127)
                        value=-127;
                    make(undo,x,y,z,value,editor->currentMaterial());
                }
    }
}

void TerrainAction::make(TerrainUndo * undo,int x, int y, int z, float value, int texture_id)
{
}

