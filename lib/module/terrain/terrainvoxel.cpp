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

#include "terrainvoxel.h"

TerrainVoxel::TerrainVoxel()
{
    value=0;
    material[0]=0;
    material[1]=0;
    material[2]=0;
    material[3]=0;
}

TerrainVoxel::TerrainVoxel(signed char value)
{
    this->value=value;
    material[0]=0;
    material[1]=0;
    material[2]=0;
    material[3]=0;
}

void TerrainVoxel::setTextureRawV(int id, unsigned char value)
{
    material[id]=(material[id]&0xf) | (value<<4);
}

void TerrainVoxel::setTextureRawH(int id, unsigned char value)
{
    material[id]=(material[id]&0xf0) | value;
}

void TerrainVoxel::setMaterial(const unsigned char *material)
{
    for(int i=0;i<4;i++)
        this->material[i]=material[i];
}

void TerrainVoxel::clearMaterial()
{
    material[0]=0;
    material[1]=0;
    material[2]=0;
    material[3]=0;
}

unsigned char TerrainVoxel::textureV(int id)
{
    return material[id] >> 4;
}

unsigned char TerrainVoxel::textureH(int id)
{
    return material[id]&0xf;
}

void TerrainVoxel::setTextureH(unsigned char id, unsigned char value)
{
    float m;
    unsigned char t;

    setTextureRawH(id,0);
    int h=0;
    for(int i=0;i<4;i++)
        h+=textureH(i);
    if(h==0)
        m=0;
    else
        m=float(15-value)/float(h);
    h=15;
    for(int i=0;i<4;i++)
    {
        t=textureH(i)*m;
        setTextureRawH(i,t);
        h-=t;
    }

    setTextureRawH(id,h);
}

void TerrainVoxel::setTextureV(unsigned char id, unsigned char value)
{
    float m;
    unsigned char t;
    setTextureRawV(id,0);
    int h=0;
    for(int i=0;i<4;i++)
        h+=textureV(i);
    if(h==0)
        m=0;
    else
        m=float(15-value)/float(h);
    h=15;
    for(int i=0;i<4;i++)
    {
        t=textureV(i)*m;
        setTextureRawV(i,t);
        h-=t;
    }
    setTextureRawV(id,h);
}
