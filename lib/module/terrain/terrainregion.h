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

#ifndef TERRAINREGION_H
#define TERRAINREGION_H

#include "../../mapobject.h"
#include "../../math/math.h"

#include "terrainvoxel.h"

#include <stdint.h>
#include <fstream>
#include <string>

class TerrainBase;
/**
 * @brief The TerrainRegion class is representing part of the terrain.
 */
class TerrainRegion :public MapObject
{
public:
    enum RegionType{
        UNDERGROUND=0,
        GROUND=1,
        ABOVEGROUND=2
    };

    /**
     * @brief init Initionlize terrain region.
     * @param terrain Parent terrain.
     * @param x X position.
     * @param y Y position.
     * @param z Z position.
     * @param size Region size in power of two.
     */
    TerrainRegion(TerrainBase* terrain,int x, int y, int z, int size);
    virtual ~TerrainRegion();

    /**
     * @brief selectDraw Always selected because the size of terrain is identical to octree leaf.
     */
    void selectDraw(const Camera &);

    void selectDraw();
    /**
     * @brief collision Collision of terrain with ray.
     * @param a Ray start point.
     * @param b Ray end poin in collision. Normal and pointer is written on collision.
     */
    void collision(const Vector3f & a,Collision & b);
    /**
     * @brief inBox Test if the terrain fits in octree node.
     * @param x Node position x.
     * @param y Node position y.
     * @param z Node position z.
     * @param w Node width.
     * @return
     */
    bool inBox(int x, int y, int z, int w) const;
    /**
     * @brief field Get pointer to the voxel on given position
     * @param x
     * @param y
     * @param z
     * @return
     */
    const TerrainVoxel *voxel(int x,int y,int z) const;
    /**
     * @brief fieldFast is identical to field() but withou boundary checks and compress check.
     * @param x
     * @param y
     * @param z
     * @return
     */
    const TerrainVoxel *voxelRaw(int x,int y,int z) const;

    /**
     * @brief voxelField Get pointer to field of voxels
     * @return
     */
    TerrainVoxel *voxelData();
    void setVoxelData(TerrainVoxel * data);

    /**
     * @brief setVoxelValue Set voxel value.
     * @param x
     * @param y
     * @param z
     * @param value
     */
    void setVoxelValue(int x, int y, int z, signed char value);
    /**
     * @brief setVoxelMaterial
     * @param x
     * @param y
     * @param z
     * @param material
     */
    void setVoxelMaterial(int x, int y, int z, const unsigned char * material);
    /**
     * @brief setVoxel Set voxel value.
     * @param x
     * @param y
     * @param z
     * @param value
     * @param texture_id
     */
    void setVoxel(int x, int y, int z, const TerrainVoxel * voxel);

    Vector3f getNormal(const Vector3i & vert0, const Vector3i & vert1, int step) const;
    Vector3f getVertex(int x1, int y1, int z1, int x2, int y2, int z2) const;

    void voxelCollision(const Vector3i &voxel, const Vector3f & a, Collision & b);

    int type();
    /**
     * @brief isDirty
     * @return If the region changed since last rebuild.
     */
    bool isDirty() const;

    /**
     * @brief decompress Check if the region is compressed, and if yes, it will be decompressed.
     */
    void decompress();
    void touchDecompress();
    bool isCompressed() const;
    const Vector3i &position() const;

    RegionType regionType() const;
    void setRegionType(RegionType type);

    //if something changed since last surface rebuild
    bool dirty;
    //
    bool changed;

    friend class TerrainCompress;
    friend class TerrainBase;
    friend class TerrainIO;
protected:
    bool isFinite(float var) const;

    const Vector3i pos;// lower left position
    TerrainVoxel * voxel_data;
    const int size;

    signed char * voxel_compressed;
    unsigned int comressed_size;

    RegionType region_type;
    TerrainBase * terrain;
};


inline const TerrainVoxel *TerrainRegion::voxelRaw(int x, int y, int z) const
{
    return &voxel_data[x*(size)*(size)+y*(size)+z];
}

#endif // TERRAINREGION_H
