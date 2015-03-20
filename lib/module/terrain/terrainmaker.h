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

#ifndef TERRAINMAKER_H
#define TERRAINMAKER_H

#include "terrainsurface.h"

namespace Transvoxel {
    struct TransitionCellData;
    struct RegularCellData;
}

class TerrainMaker
{
public:
    TerrainMaker(int region_size);
    ~TerrainMaker();

    struct SurfaceData
    {
        SurfaceData();
        void copy(TerrainSurface * surface);

        int index_cnt[4],index_offset[4];
        int index_side_cnt[3][4],index_side_offset[3][4];
    };
    /**
     * @brief rebuid Rebuild graphical part of the terrain surface. It will extract triangle surface from voxel grid.
     * @param surface Terrain surface.
     */
    TerrainMaker::SurfaceData rebuid(TerrainSurface *surface);

private:

    int rebuidSurface(TerrainSurface *surface,SurfaceData *temp);

    void createTriangles(const Transvoxel::TransitionCellData *data, const int *edge_list, int &indices_count);
    void createTriangles(const Transvoxel::RegularCellData *data, const int *edge_list, int &indices_count, bool make_normal);
    int marchingCubes(TerrainSurface *surface,SurfaceData *temp, TerrainSurface::LOD level);
    int transvoxel(TerrainSurface *surface,SurfaceData*temp,TerrainSurface::LOD level, TerrainSurface::Side side);

    void resizeIndexBuffer(int size);
    void resizeDataBuffer(int size);

    const TerrainVoxel *bufferVoxel(int x, int y, int z) const;
    Vector3f interpolateVertex(float x1, float y1, float z1, float v1, float x2, float y2, float z2, float v2) const;
    void adjustVertices(Vector3i &v1, Vector3i &v2) const;
    void bufferVoxels(TerrainSurface *surface);

    const int region_size;
    int * edges;

    unsigned char *data_buffer;
    unsigned short *indices_buffer;
    int data_capacity;
    int indices_capacity;

    int buffer_data_count;
    int buffer_indices_count;

    int byte_count;

    TerrainVoxel * buffer;
};

inline const TerrainVoxel *TerrainMaker::bufferVoxel(int x, int y, int z) const
{
    return &buffer[(x+1)*(34)*(34)+(y+1)*(34)+z+1];
}


#endif // TERRAINMAKER_H
