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

#include "terrainsurface.h"

#include "terraingraphics.h"

TerrainSurface::TerrainSurface(TerrainGraphics* terrain, int x, int y, int z, int size)
    :TerrainRegion(terrain,x,y,z,size),level(LEVEL_0),terrain(terrain),drawable(false)
{
    for(int i=0;i<4;i++)
    {
        index_cnt[i]=0;
        index_offset[i]=0;

        for(int side=0;side<3;side++)
        {
            index_side_cnt[side][i]=0;
            index_side_offset[side][i]=0;
        }
    }
}

TerrainSurface::~TerrainSurface()
{
    vbo.destroy();
    ibo.destroy();
}

TerrainSurface::LOD TerrainSurface::lodLevel() const
{
    return level;
}

void TerrainSurface::selectDraw(const Camera & camera)
{
    if(drawable)
    {
        Vector3f dist(pos[0]+16.0,pos[1]+16.0,pos[2]+16.0);
        dist-=camera.position();
        float len=dist.length();
        if(len<128.0)
            level=LEVEL_0;
        else if(len<256.0)
            level=LEVEL_1;
        else if(len<512.0)
            level=LEVEL_2;
        else
            level=LEVEL_3;

        terrain->pushRender(this);
    }
}

void TerrainSurface::selectDraw()
{
    if(drawable)
    {/*
        Vector3f dist(pos[0]+16.0,pos[1]+16.0,pos[2]+16.0);
        dist-=camera.position();
        float len=dist.length();
        if(len<128.0)
            level=LEVEL_0;
        else if(len<256.0)
            level=LEVEL_1;
        else if(len<512.0)
            level=LEVEL_2;
        else
            level=LEVEL_3;*/
        level=LEVEL_3;
        terrain->pushRender(this);
    }
}

void TerrainSurface::clearGraphics()
{
    drawable=false;
    vbo.destroy();
    ibo.destroy();
    vao.destroy();
    shadow_vao.destroy();

    for(int i=0;i<4;i++)
    {
        index_cnt[i]=0;
        index_offset[i]=0;

        for(int side=0;side<3;side++)
        {
            index_side_cnt[side][i]=0;
            index_side_offset[side][i]=0;
        }
    }
}

int TerrainSurface::type()
{
    return 1;
}

void TerrainSurface::setVertices(int size, GLvoid * data, int index_cnt[4],int index_offset[4])
{
    vbo.create(size,data);
}

bool TerrainSurface::isDrawable()
{
    return drawable;
}
