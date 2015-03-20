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

#ifndef WORLDRENDER_H
#define WORLDRENDER_H

#include "../module/modulerender.h"
//#include "../module/grass/grassrender.h"
#include "../module/model/modelrender.h"
#include "../module/terrain/terrainrender.h"
#include "../module/ocean/oceanrender.h"
#include "../module/skybox/skyboxrender.h"
#include "../module/editor/editorrender.h"
#include "./lightdirectionalrender.h"
#include "../worldgraphics.h"

class WorldRender
{
public:
    WorldRender();
    bool init();
    void renderPrev(WorldGraphics * world, const Camera &camera, unsigned int elapsed);
    void render(WorldGraphics *world, const Matrix4f &mvp, unsigned int elapsed);
    void renderShadow(WorldGraphics *world, const Matrix4f &mvp,unsigned int elapsed);

private:
    // renderers
    ModelRender model;
    TerrainRender terrain;
   // GrassRender grass;
    OceanRender ocean;
    SkyBoxRender skybox;
    //editor
    EditorRender editor;

    ModuleRender * renderers[10];
    int render_count;
};

#endif // WORLDRENDER_H
