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

#ifndef TERRAINSURFACE_H
#define TERRAINSURFACE_H

#include "terrainregion.h"
#include "../../graphics/graphics.h"

class TerrainGraphics;

class TerrainSurface : public TerrainRegion
{
public:

    TerrainSurface(TerrainGraphics *terrain, int x, int y, int z, int size);
    ~TerrainSurface();

    /**
     * @brief The Side enum Side path select.
     */
    enum Side
    {
        X_SIDE=0,
        Y_SIDE,
        Z_SIDE,
        SURFACE
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

    void selectDraw(const Camera &camera);
    void selectDraw();

    void clearGraphics();

    int type();

    void setVertices(int size, GLvoid * data, int index_cnt[], int index_offset[]);

    bool isDrawable();

    LOD lodLevel() const;

    friend class TerrainMaker;
    friend class TerrainRenderForward;
    friend class TerrainRender;
private:
    /**
     * @brief level Present terrain's level of detail
     */
    LOD level;

    TerrainGraphics* terrain;

    VertexBuffer vbo;
    IndexBuffer ibo;
    VertexArray vao, shadow_vao;
    bool drawable;
    int index_cnt[4],index_offset[4];
    int index_side_offset[3][4];
    int index_side_cnt[3][4];
};

#endif // TERRAINSURFACE_H
