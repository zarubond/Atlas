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

#ifndef TERRAINMATERIAL_H
#define TERRAINMATERIAL_H

#include "../../graphics/graphics.h"

class TerrainMaterial
{
public:
    TerrainMaterial(int id,TextureArray * normalmap_atlas,TextureArray * texture_atlas);

    bool loadTexture(const char * path);

    bool loadNormalMap(const char * path);

    void disable();

    bool enabled();

    int id();

    std::string name;
    std::string normal_map_path;
    std::string texture_path;

private:
    TextureArray * normalmap_atlas;
    TextureArray * texture_atlas;
    const int _id;
    bool _enabled;
};

#endif // TERRAINMATERIAL_H
