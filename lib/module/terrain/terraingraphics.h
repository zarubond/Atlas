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

#ifndef TERRAINGRAPHICS_H
#define TERRAINGRAPHICS_H

#include "terrainbase.h"
#include "terrainsurface.h"
#include "terrainmaker.h"
#include "terrainmaterial.h"
#include "../../graphics/graphics.h"
#include <mutex>

class TerrainGraphics : public TerrainBase
{
public:
    TerrainGraphics(int type);
    ~TerrainGraphics();
    /**
     * @brief build Build all terrain parts.
     * @param nodes_x Number of nodes (TerrainRegion) on x axes.
     * @param nodes_y Number of nodes (TerrainRegion) on y axes.
     * @param nodes_z Number of nodes (TerrainRegion) on z axes.
     * @param node_size Size of one node, should by power of two and identical to size of map node.
     */
    void build(int nodes_x, int nodes_y, int nodes_z, int node_size);
    /**
     * @brief rebuid Rebuild surface of terrain.
     * @param force If true than all regions will be rebuild (very time consuming). If false than only those regions which was changed will be rebuil.
     */
    void rebuid(bool force=false);
    /**
     * @brief pushRender Add surface on the stack to be rendered. Stack is cleard after each render.
     * @param surface Surface to be rendered.
     */
    void pushRender(TerrainSurface * surface);
    void pushRebuild(TerrainSurface * surface);
    TerrainSurface * popRender();
    TerrainSurface **renderList(int & count);
    int renderCount();
    void clearRenderList();
    TerrainRegion * createRegion(int x, int y, int z);

    /**
     * @brief maxTextures Maximum allowed terrain textures.
     * @return maximum Number of supported textures applyable on terrain.
     */
    int maxMaterials() const;
    /**
     * @brief getMaterial
     * @param id Material id (must be zero or greated and smaller than maxTextures);
     * @return Pointer to material used by terrain.
     */
    TerrainMaterial *getMaterial(int id);
    /**
     * @brief addMaterial it will add new material to terrain
     * @param material
     * @return
     */
    bool addMaterial(TerrainMaterial * material);
    int materialCount()const;


    TextureArray *textures();
    TextureArray *normalMaps();

    std::mutex mutex;
private:

    TerrainSurface ** render_list;
    TerrainSurface ** rebuild_list;
    TerrainMaker * maker;
    int render_count,rebuild_count;

    TextureArray texture;
    TextureArray normalmap;

    int material_count;
    TerrainMaterial *materials[8];
};

#endif // TERRAINGRAPHICS_H
