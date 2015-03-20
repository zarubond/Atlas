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

#ifndef TERRAINRENDER_H
#define TERRAINRENDER_H


#include "../../math/math.h"
#include "../../graphics/graphics.h"

#include "terraingraphics.h"
#include "../modulerender.h"


class TerrainRender: public ModuleRender
{
public:
    TerrainRender();
    ~TerrainRender();
    /**
     * @brief init Initialize rendering.
     * @param vertex_file Vertex shader file.
     * @param fragment_file Fragment shader file.
     * @return True on success.
     */
    bool init();
    /**
     * @brief render Render all added surfaces.
     * @param mvp Model view projection matrix.
     * @param cam_pos Camera position.
     */
    void render(WorldGraphics * world, const Environment *envi, const Matrix4f &mvp, unsigned int);
    void renderShadow(WorldGraphics * world, const Environment *envi, const Matrix4f &mvp,unsigned int);

private:
    Attribute attribute_coord3d, attribute_normal, attribute_texture, attribute_shadow_coord3d;
    Uniform uniform_mvp,uniform_shadow_mvp;
    Uniform uniform_texture_atlas,uniform_texture_normal;
    Shader program, shader_shadow;
    void setupRender(TerrainSurface *surface);
    void renderSurface(TerrainGraphics *terrain, TerrainSurface *surface);
    void renderSurfaceShadow(TerrainSurface *surface);
    void setupShadowRender(TerrainSurface *surface);
};

#endif // TERRAINRENDER_H
