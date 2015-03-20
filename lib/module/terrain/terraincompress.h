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

#ifndef TERRAINCOMPRESS_H
#define TERRAINCOMPRESS_H

#include "terrainbase.h"
#include "terrainregion.h"
class TerrainBase;

class TerrainCompress
{
public:
    enum Type
    {
        NONE=0,
        EXT_RLE=1,
        MATERIAL_RLE=2,
        RLE=3,
        BLOCK_RLE=5,
        DEFLATE=6,// the ZIP format
        EXT_RLE_OLD=7,
        SPLIT_RLE=8
    };

    // replace empty space with special number
    // the same for undeground

    TerrainCompress();
    int encode(TerrainCompress::Type type, const TerrainRegion *region, signed char *&voxel_data);
    void decode(TerrainCompress::Type type, TerrainRegion * region, const signed char *voxel_data);
private:

    int compressExtRLEOld(const TerrainRegion * region, unsigned char *&voxel_data);

    int compressNone(const TerrainRegion *region, signed char *&voxel_data);
    void decodeNone(TerrainBase *base, const signed char *voxel_data,unsigned int node_size,
                    unsigned int size);

    int compressExtRLE(const TerrainRegion *region, unsigned char *&voxel_data);
    void decompressExtRLE(TerrainRegion * region, const signed char *voxel_data, int size) const;

    int compressSplitRLE(const TerrainRegion *region, unsigned char *&voxel_data);
    void decompressSplitRLE(TerrainRegion * region, const signed char *voxel_data, int size) const;
};

#endif // TERRAINCOMPRESS_H
