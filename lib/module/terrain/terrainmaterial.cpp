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

#include "terrainmaterial.h"

TerrainMaterial::TerrainMaterial(int id, TextureArray *normalmap_atlas, TextureArray *texture_atlas)
    :normalmap_atlas(normalmap_atlas),texture_atlas(texture_atlas),_id(id),_enabled(false)
{}

bool TerrainMaterial::loadTexture(const char *path)
{
    if(texture_atlas->loadSub(path,_id))
    {
        texture_path=path;
        _enabled=true;
        return true;
    }
    return false;
}

bool TerrainMaterial::loadNormalMap(const char *path)
{
    if(normalmap_atlas->loadSub(path,_id))
    {
        normal_map_path=path;
        _enabled=true;
        return true;
    }
    return false;
}

void TerrainMaterial::disable()
{
    _enabled=false;
}

bool TerrainMaterial::enabled()
{
    return _enabled;
}

int TerrainMaterial::id()
{
    return _id;
}
