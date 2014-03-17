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
#ifndef TERRAINGRAPHICS_H
#define TERRAINGRAPHICS_H

#include "terrainbase.h"
#include "terrainsurface.h"
#include "../../../tools/graphics.h"


class TerrainGraphics : public TerrainBase
{
public:
    TerrainGraphics(int type);
    ~TerrainGraphics();

    /**
     * @brief build Build all terrain parts.
     * @param nodes_x
     * @param nodes_y
     * @param nodes_z
     * @param node_size
     */
    void build(int nodes_x, int nodes_y, int nodes_z, int node_size);
    /**
     * @brief rebuid Rebuild surface of terrain.
     * @param force
     */
    void rebuid(bool force=false);

    void pushRender(TerrainSurface * surface);
    void pushRebuild(TerrainSurface * surface);
    TerrainSurface * popRender();
    void clearRender();
    int renderCount();

    bool setTexture(unsigned short id, Texture * texture);
    /**
     * @brief maxTextures Maximum allowed terrain textures.
     * @return
     */
    int maxTextures() const;

    Texture * getTexture(unsigned short id);

    TextureArray atlas;
    TextureArray normalmap;

    void setVoxel(int x, int y, int z, signed char value, unsigned char texture_id);
    void setVoxelTexture(int x, int y, int z, unsigned char texture_id);
    unsigned char materialId(unsigned char texture_vertival, unsigned char texture_horizontal);
    void setVoxelValue(int x, int y, int z, signed char value);
private:
    int rebuildLevel(TerrainSurface *surface, TerrainSurface::LOD level);
    int marchingCubes(TerrainSurface *surface, TerrainRegion::LOD level);
    int transvoxel(TerrainSurface *surface, TerrainSurface::LOD level, TerrainSurface::Side side);

    TerrainSurface ** render_list;
    TerrainSurface ** rebuild_list;
    int render_count,rebuild_count;
    Texture * textures[8];
    GLfloat *data_buffer;
    GLushort *indices_buffer;
    int capacity;
    int indices_capacity;
    int * edges;
};

#endif // TERRAINGRAPHICS_H
