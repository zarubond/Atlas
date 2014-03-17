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
#include "mapdrawer.h"

MapDrawer::MapDrawer()
{
}

bool MapDrawer::load()
{
    if(!terrain.init())
        std::cerr<<"Unable to load terrain renderer!"<<std::endl;
    if(!model.init())
        std::cerr<<"Unable to load model renderer!"<<std::endl;
    if(!grass.init())
        std::cerr<<"Unable to load grass renderer!"<<std::endl;

    return true;
}

void MapDrawer::clearRenderList()
{
    model.clearRenderList();
    terrain.clearRenderList();
    grass.clearRenderList();
}

void MapDrawer::render(const Matrix4f &mvp, int elapsed)
{
    terrain.render(mvp,elapsed);
    model.render(mvp,elapsed);
    grass.render(mvp,elapsed);
}

void MapDrawer::renderShadow(const Matrix4f &mvp, unsigned int elapsed)
{
    terrain.renderShadow(mvp,elapsed);
    model.renderShadow(mvp,elapsed);
    grass.renderShadow(mvp,elapsed);
}
