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

#include "worldrender.h"

WorldRender::WorldRender()
{
}

bool WorldRender::init()
{
    render_count=0;

    renderers[render_count++]=&terrain;
    renderers[render_count++]=&model;
    //renderers[render_count++]=&grass;
    renderers[render_count++]=&ocean;
    renderers[render_count++]=&skybox;
    renderers[render_count++]=&editor;//must be fist for trasparency


    if(!terrain.init()) cerr<<"Unable to load terrain renderer!"<<endl;
    if(!model.init())   cerr<<"Unable to load model renderer!"<<endl;
   // if(!grass.init()) cerr<<"Unable to load grass renderer!"<<endl;
    if(!ocean.init())   cerr<<"Unable to load ocean renderer!"<<endl;
    if(!skybox.init())  cerr<<"Unable to load skybox renderer!"<<endl;
    if(!editor.init())  cerr<<"Unable to load editor renderer!"<<endl;

    return true;
}

void WorldRender::renderPrev(WorldGraphics * world, const Camera &camera, unsigned int elapsed)
{
    for(int i=0;i<render_count;i++)
        renderers[i]->renderPrev(world,&world->environment, camera, elapsed);
}

void WorldRender::render(WorldGraphics * world, const Matrix4f &mvp, unsigned int elapsed)
{
    for(int i=0;i<render_count;i++)
        renderers[i]->render(world, &world->environment, mvp, elapsed);
}

void WorldRender::renderShadow(WorldGraphics * world, const Matrix4f &mvp, unsigned int elapsed)
{
    for(int i=0;i<render_count;i++)
        renderers[i]->renderShadow(world, &world->environment, mvp, elapsed);
}
