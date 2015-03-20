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

#include "terrainaction.h"

#include "../terraincontrol.h"

TerrainAction::TerrainAction(const TerrainControl *editor):
    editor(editor), shape(NULL)
{}

TerrainAction::~TerrainAction()
{}

void TerrainAction::stroke(TerrainUndo * undo, Shape *shape, TerrainCache * shadow,unsigned char texture_v,unsigned char texture_h)
{
    this->shape=shape;
    this->undo=undo;
    this->shadow=shadow;
    int size=ceil(shape->getRadius())+1;
    float fade=editor->materialFade();
    float opacity=editor->materialOpacity();

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
                    make(x, y, z, value, fade, opacity, texture_v, texture_h);
                }
    }
}

bool TerrainAction::makeShadowTerrain()
{
    return false;
}

bool TerrainAction::repeat()
{
    return false;
}

void TerrainAction::make(int UNUSED(x), int UNUSED(y), int UNUSED(z), float UNUSED(value), float UNUSED(fade), float UNUSED(opacity)
                         , unsigned char UNUSED(texture_v), unsigned char UNUSED(texture_h))
{
}

float TerrainAction::isovalue(float x, float y, float z) const
{
    return shape->isovalue(x,y,z);
}

signed char TerrainAction::voxelValue(unsigned x, unsigned y, unsigned z)
{
    return undo->value(x,y,z);
}

Vector3f TerrainAction::getVertex(float x1, float y1, float z1,float v1,
                                          float x2, float y2, float z2,float v2)
{
    Vector3f tmp;
    float mu =0;

    if(v1 - v2!=0)  mu= v1 / (v1 - v2);

    tmp[0] = x1 + mu * (x2 - x1);
    tmp[1] = y1 + mu * (y2 - y1);
    tmp[2] = z1 + mu * (z2 - z1);
  //  tmp=a+mu*(b-a);

    return tmp;
}

const TerrainVoxel *TerrainAction::getBackupVoxel(unsigned x, unsigned y, unsigned z)
{
    return undo->voxel(x,y,z);
}

void TerrainAction::setVoxel(int x, int y, int z, const TerrainVoxel * voxel)
{
    if(makeShadowTerrain())
        shadow->backupRegionAtPosition(x,y,z);
    editor->getTerrain()->setVoxel(x,y,z,voxel);
}

void TerrainAction::setVoxelValue(int x, int y, int z, signed char value)
{
    if(makeShadowTerrain())
        shadow->backupRegionAtPosition(x,y,z);
    editor->getTerrain()->setVoxelValue(x,y,z,value);
}

void TerrainAction::setMaterial(TerrainVoxel &  voxel, unsigned char value, unsigned char texture_v, unsigned char texture_h)
{
    //int val=ceil(15.0*(-value/127.0));
    if(editor->blankTexture())
    {
        voxel.clearMaterial();
        return;
    }
//vertical
    if(texture_v==0)
    {
        for(int i=0;i<4;i++)
            voxel.setTextureRawV(i,0);
    }
    else if(voxel.textureV(texture_v-1)<=value)
    {
        voxel.setTextureV(texture_v-1,value);
    }

//horizontal
    if(texture_h==0)
    {
        for(int i=0;i<4;i++)
            voxel.setTextureRawH(i,0);
    }
    else if(voxel.textureH(texture_h-1)<=value)
    {
        voxel.setTextureH(texture_h-1,value);
    }
}

