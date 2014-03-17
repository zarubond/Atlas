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
#ifndef WORLD_H
#define WORLD_H

#include "module/grass/grassgraphics.h"
#include "module/model/modelgraphics.h"
#include "module/terrain/terraingraphics.h"
#include "module/skybox/skyboxgraphics.h"
#include "module/water/watergraphics.h"

#include "map.h"

#define ATLAS_MODULES \
    ATLAS_MODULE(GRASS,Grass,grass) \
    ATLAS_MODULE(TERRAIN,Terrain,terrain) \
    ATLAS_MODULE(MODEL,Model,model) \
    ATLAS_MODULE(SKYBOX,SkyBox,skybox) \
    ATLAS_MODULE(WATER,Water,water) \

class World
{
public:

#define ATLAS_MODULE(a, b,c) a,
enum MODULES { ATLAS_MODULES };
#undef ATLAS_MODULE

    Map map;

    GrassGraphics grass;
    TerrainGraphics terrain;
    ModelGraphics model;
    SkyBoxGraphics skybox;
private:

};

#endif // WORLD_H
