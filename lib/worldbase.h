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
#ifndef WORLDBASE_H
#define WORLDBASE_H

#include "module/grass/grassbase.h"
#include "module/model/modelbase.h"
#include "module/terrain/terrainbase.h"
//#include "module/skybox/skyboxbase.h"
#include "module/ocean/oceanbase.h"

class WorldBase
{
public:
#define ATLAS_MODULE(a, b,c) a,
enum MODULES { ATLAS_MODULES };
#undef ATLAS_MODULE

    WorldBase();

    GrassBase grass;
    TerrainBase terrain;
    ModelBase model;
    OceanBase ocean;
    Map map;

    std::string name;
};

#endif // WORLDBASE_H
