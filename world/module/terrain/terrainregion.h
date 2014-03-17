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
#ifndef TERRAINREGION_H
#define TERRAINREGION_H

#include "../../mapobject.h"
#include "../../../tools/math/math.h"

#include <stdint.h>
#include <iostream>
#include <fstream>
#include <string>

/**
 * @brief The TerrainVoxel struct is basic type fo voxel.
 */
struct TerrainVoxel{
    TerrainVoxel(signed char value,unsigned char texture_id):value(value),texture_id(texture_id){}
    //TerrainVoxel(Voxel & voxel):value(voxel.value),texture_id(voxel.texture_id){}
    TerrainVoxel(){}
    /**
     * @brief value Isovalue of voxel.
     */
    signed char value;
    /**
     * @brief texture_id Voxel texture identification.
     */
    unsigned char texture_id;
};


class TerrainBase;
/**
 * @brief The TerrainRegion class is representing part of the terrain.
 */
class TerrainRegion :public MapObject
{
public:
    /**
     * @brief The Side enum Side path select.
     */
    enum Side
    {
        X_SIDE=0,
        Y_SIDE,
        Z_SIDE

    };
    /**
     * @brief The LOD enum Level of terrains detail.
     */
    enum LOD
    {
        LEVEL_0=0,
        LEVEL_1,
        LEVEL_2,
        LEVEL_3,
        LEVEL_NONE
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
     * @brief getVoxel Get voxel at given position.
     * @param point Position of voxel in space
     * @return voxel
     */
    TerrainVoxel getVoxel(Vertex3i point);
    /**
     * @brief getVoxel Get voxel at given position.
     * @param x
     * @param y
     * @param z
     * @return voxel
     */
    TerrainVoxel getVoxel(int x, int y, int z);
    /**
     * @brief getIsolevel Terrain isovalue.
     * @return
     */
    float getIsolevel();

    /**
     * @brief marchingCubes Transforms voxel grid to triangle mesh.
     * @param buffer Buffer for vertices. With structure vertex, normal, texture: x,y,z,n,n,n,t
     * @param indices Buffer for indices. Indices for triangle rendering.
     * @param data_count Number of vertice in buffer.
     * @param indices_count Number of indices in buffer.
     * @param step Resolution selector. Alwais power of 2. The maximum si width of terrain.
     */
    void marchingCubes(GLfloat *&buffer, GLushort *&indices, GLfloat *&buffer_switch, GLushort *&indices_switch, GLint &data_count, GLint &indices_count, GLint &switch_count, int step=1);
    /**
     * @brief transvoxel Generates pathes between different resolutions.
     * @param buffer  Buffer for vertices. With structure vertex, normal, texture: x,y,z,n,n,n,t
     * @param indices Buffer for indices. Indices for triangle rendering.
     * @param data_count Number of vertice in buffer.
     * @param indices_count Number of indices in buffer.
     * @param side Side of terrain patch to be generated.
     * @param step Resolution of the patch.
     */
    void transvoxel(GLfloat *&buffer, GLushort *&indices, GLint &data_count, GLint &indices_count,
                     Side side, int step);
    /**
     * @brief selectDraw Always selected because the size of terrain is identical to octree leaf.
     */
    void selectDraw(const Camera &);
    /**
     * @brief collision Collision of terrain with ray.
     * @param a Ray start point.
     * @param b Ray end poin in collision. Normal and pointer is written on collision.
     */
    void collision(const Vertex3f & a,Collision & b);
    /**
     * @brief inBox Test if the terrain fits in octree node.
     * @param x Node position x.
     * @param y Node position y.
     * @param z Node position z.
     * @param w Node width.
     * @return
     */
    bool inBox(int x, int y, int z, int w);
    /**
     * @brief field Get pointer to the voxel on given position
     * @param x
     * @param y
     * @param z
     * @return
     */
    TerrainVoxel * field(int x,int y,int z);

    /**
     * @brief voxelField Get pointer to field of voxels
     * @return
     */
    TerrainVoxel *voxelField();
    /**
     * @brief setVoxelTexture Set voxel value.
     * @param x
     * @param y
     * @param z
     * @param texture_id
     */
    void setVoxelTexture(int x, int y, int z, int texture_id);
    /**
     * @brief setVoxelValue Set voxel value.
     * @param x
     * @param vy
     * @param z
     * @param value
     */
    void setVoxelValue(int x, int vy, int z, signed char value);
    /**
     * @brief setVoxel Set voxel value.
     * @param x
     * @param y
     * @param z
     * @param value
     * @param texture_id
     */
    void setVoxel(int x, int y, int z, signed char value, unsigned char texture_id);
    /**
     * @brief pos_x Terrain position.
     * @brief pos_x Terrain position.
     * @brief pos_x Terrain position.
     */
    int pos_x,pos_y,pos_z;
    /**
     * @brief level Present terrain's level of detail
     */
    LOD level;

    Vertex4f gradientNormal(int i, int j, int k, int step);
    Vertex4f gradientNormal(const Vertex3i &vertex);
    Vertex3f getNormal(const Vertex3i & vert0, const Vertex3i & vert1);
    Vertex3f getVertex(float x1, float y1, float z1, float x2, float y2, float z2);

    Vertex3f interpolate(Vertex4f a, Vertex4f b);
    void adjustVertices(Vertex3i &v1, Vertex3i &v2);
    bool isFinite(float var);

    void voxelCollision(const Vertex3i &voxel, const Vertex3f & a, Collision & b);

    int type();

protected:

    struct Edge{
        int id;
        char value;
        char texture;
    };

    //static int * edges;
    const int edges_size;

    const int size;
    TerrainBase * terrain;
    TerrainVoxel * data_grid;

    //if something changed
    bool dirty;

    float isolevel;
};

#endif // TERRAINREGION_H
