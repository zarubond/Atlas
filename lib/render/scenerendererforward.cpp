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

#include "scenerendererforward.h"

//#define SPEED_TEST

SceneRendererForward::SceneRendererForward()
{
    init_gl=false;
    recheck=true;
}

SceneRendererForward::~SceneRendererForward()
{
}

bool SceneRendererForward::init()
{
    if(!init_gl)
    {
        init_gl=drawer.init();
        glFinish();
        dbuffer.init();
        drender.init();
    }

    return init_gl;
}

void SceneRendererForward::resize(int w, int h)
{
    screen_width=w;
    screen_height=h;

    if(init_gl)
    {
        recheck=true;
        drender.resize();
    }
}

void SceneRendererForward::renderScene(WorldGraphics *world, const Camera &camera)
{
    if(world==NULL)  return;
    elapsed=elapsedMilli();
    mutex.lock();

#ifdef SPEED_TEST
    ElapsedCPU timer;

    glFinish();
    timer.start();
#endif
    glViewport(0, 0, camera.width(), camera.height());
    renderWorld(world, camera);

#ifdef SPEED_TEST
    glFinish();
    cout<<"Render time:"<<timer.elapsed()<<"ms"<<endl;
    timer.restart();
#endif
    mutex.unlock();
}

void SceneRendererForward::renderSceneVR(WorldGraphics *world, const Camera &camera)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glViewport(0, 0, camera.width(), camera.height());

    mutex.lock();
    drender.render(&dbuffer);
    mutex.unlock();
    glEnable(GL_DEPTH_TEST);
}

void SceneRendererForward::renderBuffersVR(WorldGraphics *world, const Camera &camera)
{
    if(recheck)
        dbuffer.resize(screen_width,screen_height);

    elapsed=elapsedMilli();

    Camera left=drender.leftEye(camera);
    Camera right=drender.rightEye(camera);

    mutex.lock();

    clearRenderLists(world);
    world->map.buildRenderList(camera);

    dbuffer.bind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0,0.0,0.0,1.0);

    //left
    glViewport(0,0,screen_width/2,screen_height);
    drawer.renderPrev(world, left, elapsed);

    glClear(GL_DEPTH_BUFFER_BIT);
    drawer.render(world, left.perspectiveLook(), elapsed);

    //right
    glViewport(screen_width/2, 0, screen_width/2, screen_height);
    drawer.renderPrev(world, right, elapsed);

    glClear(GL_DEPTH_BUFFER_BIT);
    drawer.render(world, right.perspectiveLook(), elapsed);


    dbuffer.release();
    mutex.unlock();
}

void SceneRendererForward::renderWorld(WorldGraphics * world,const Camera & camera)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawer.renderPrev(world, camera, elapsed);

    clearRenderLists(world);
    world->map.buildRenderList(camera);

    drawer.render(world, camera.perspectiveLook(), elapsed);
}


int SceneRendererForward::elapsedMilli()
{
    timeb tb;
    ftime(&tb);
    int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
    return nCount;
}

void SceneRendererForward::clearRenderLists(WorldGraphics * world)
{
    world->terrain.clearRenderList();
    world->model.clearRenderList();
}

#undef SPEED_TEST
