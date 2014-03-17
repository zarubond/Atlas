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
#ifndef TERRAINSURFACE_H
#define TERRAINSURFACE_H

#include "terrainregion.h"
#include "../../../tools/graphics.h"

class TerrainGraphics;

class TerrainSurface : public TerrainRegion
{
public:

    TerrainSurface(TerrainGraphics *terrain, int x, int y, int z, int size);
    ~TerrainSurface();
    /**
     * @brief render Render the terrain part.
     */
    void render(GLint attribute_coord3d,GLint attribute_normal,GLint attribute_texcoord);
    /**
     * @brief renderShadow Render terrain's shadow.
     */
    void renderShadow(GLint attribute_shadow_coord3d);

    /**
     * @brief rebuild Rebuild terrain part.
     * @param force
     */
    //void rebuild(bool force=false);

    void selectDraw(const Camera &camera);

    void clearLevel(LOD level);

    int type();

    friend class TerrainGraphics;
private:
    void clear();

    TerrainGraphics* terrain;

   // void rebuildLevel(LOD level);

    VertexBuffer vbo_buffer[4],vbo_switch_buffer[4];
    IndexBuffer ibo_buffer[4],ibo_switch_buffer[4];
    int vertex_count[4],indices_count[4],vertex_switch_count[4],indices_switch_count[4];

    VertexBuffer trans_vbo_buffer[4][3];
    IndexBuffer trans_ibo_buffer[4][3];
    int trans_vertex_count[4][3], trans_indices_count[4][3],trans_indices_offset[4][3];
};

#endif // TERRAINSURFACE_H
