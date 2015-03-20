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
#ifndef TERRAINACTION_H
#define TERRAINACTION_H

#include "../terrainundo.h"
#include "../../../shape.h"
#include "../terrainshadow.h"

class TerrainControl;

class TerrainAction
{
public:
    TerrainAction(const TerrainControl *editor);
    virtual ~TerrainAction();
    void stroke(TerrainUndo *undo, Shape *shape, TerrainCache *shadow, unsigned char texture_v, unsigned char texture_h);
    virtual bool makeShadowTerrain();
    virtual bool repeat();
protected:
    virtual void make(int x, int y, int z, float value, float fade, float opacity, unsigned char texture_v, unsigned char texture_h);

    float isovalue(float x, float y, float z) const;
    signed char voxelValue(unsigned x,unsigned y,unsigned z);
    Vector3f getVertex(float x1, float y1, float z1, float v1, float x2, float y2, float z2, float v2);
    const TerrainVoxel *getBackupVoxel(unsigned x,unsigned y,unsigned z);

    void setVoxel(int x, int y, int z, const TerrainVoxel *voxel);
    void setVoxelValue(int x, int y, int z, signed char value);

    void setMaterial(TerrainVoxel &voxel, unsigned char value, unsigned char texture_v, unsigned char texture_h);
private:
    const TerrainControl * editor;
    Shape * shape;
    TerrainUndo * undo;
    TerrainCache * shadow;
};

#endif // TERRAINACTION_H
