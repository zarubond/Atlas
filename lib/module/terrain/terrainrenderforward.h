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

#ifndef TERRAINRENDERFORWARD_H
#define TERRAINRENDERFORWARD_H

#include "../modulerender.h"
#include "terraingraphics.h"

class TerrainRenderForward : public ModuleRender
{
public:
    TerrainRenderForward();
    ~TerrainRenderForward();
    void render(WorldGraphics * world, const Environment *envi, const Matrix4f &mvp, unsigned int);
    bool init();
private:
    Attribute attribute_coord, attribute_texture, attribute_normal;
    Uniform uniform_mvp, uniform_diffuse, uniform_ambient, uniform_sun_dir;
    Uniform u_texture_atlas[4];
    Shader program;
    void renderSurface(TerrainGraphics *base, TerrainSurface *surface);
    void setupRender(TerrainSurface *surface);
};

#endif // TERRAINRENDERFORWARD_H
