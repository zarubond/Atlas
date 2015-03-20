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

#include "scenerendererdeferred.h"

//#define SPEED_TEST
/*
 *
 *GLuint result = 0;
if (!firsttime) glGetQueryObjectuiv(fQuery, GL_QUERY_RESULT, &result);
glBeginQuery(GL_TIME_ELAPSED, fQuery);
glDraw...
glEndQuery(GL_TIME_ELAPSED);
glGenQueries(1, &fQuery);
 *
 */

SceneRendererDeferred::SceneRendererDeferred()
{
    init_gl=false;
    recheck=true;
    render_shadow=true;
}

SceneRendererDeferred::~SceneRendererDeferred()
{
}

bool SceneRendererDeferred::init()
{
    if(!init_gl)
    {
        directional.init();
        ambient.init();
        fog.init();
        shadow.init();
        gbuffer.init();
        drawer.init();
        dbuffer.init();
        drender.init();
        glFinish();
        init_gl=true;
    }

    return true;
}

void SceneRendererDeferred::resize(int w, int h)
{
    screen_width=w;
    screen_height=h;

    if(init_gl)
    {
        recheck=true;
    }
}

void SceneRendererDeferred::renderScene(WorldGraphics *world, const Camera &camera)
{
    if(world==NULL)  return;

    mutex.lock();

#ifdef SPEED_TEST
    ElapsedCPU timer;

    glFinish();
    timer.start();
#endif
    glViewport(0, 0, camera.width(), camera.height());
    if(world->environment.light)
    {
        glDisable(GL_DEPTH_TEST);

        ambient.render(&world->environment.ambient,&gbuffer);

        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);

        if(world->environment.shadow)
            directional.render(&world->environment.sun, camera, &gbuffer, &shadow, shadow_matrix);
        else
            directional.render(&world->environment.sun, camera, &gbuffer);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        fog.render(world,&world->environment.fog, camera, &gbuffer);
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderWorld(world, camera.perspectiveLook(), camera);
    }
#ifdef SPEED_TEST
            glFinish();
            cout<<"Render scene:"<<timer.elapsed()<<"ms"<<endl;
#endif
    mutex.unlock();
}

void SceneRendererDeferred::renderBuffers(WorldGraphics * world, const Camera &camera)
{
    if(world==NULL)
        return;

    elapsed=elapsedMilli();

    if(world->environment.light)
    {
        mutex.lock();

        resizeBuffers();

        if(world->environment.shadow && (world->map.changed() || render_shadow))
        {
            render_shadow=false;
            world->map.setChanged(false);
            renderShadow(world, elapsed, camera);
        }

#ifdef SPEED_TEST
        ElapsedCPU timer;
        glFinish();
        timer.start();
#endif

        //future http://www.gamerendering.com/category/lighting/
        gbuffer.bind();
        glEnable(GL_DEPTH_TEST);
        glViewport(0,0,gbuffer.width(),gbuffer.height());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderWorld(world, camera.perspectiveLook(), camera);
        gbuffer.unbind();

#ifdef SPEED_TEST
        glFinish();
        cout<<"Render buffers:"<<timer.elapsed()<<"ms"<<endl;
#endif
        mutex.unlock();
    }
}

void SceneRendererDeferred::renderSceneVR(WorldGraphics *world, const Camera &camera)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glViewport(0, 0, camera.width(), camera.height());

    mutex.lock();
    drender.render(&dbuffer);
    mutex.unlock();
    glEnable(GL_DEPTH_TEST);
}

void SceneRendererDeferred::renderBuffersVR(WorldGraphics * world, const Camera & camera)
{
    if(world==NULL)
        return;

    elapsed=elapsedMilli();

    if(world->environment.light)
    {
        mutex.lock();

        resizeBuffers();

        if(world->environment.shadow && (world->map.changed() || render_shadow))
        {
            render_shadow=false;
            world->map.setChanged(false);
            renderShadow(world, elapsed, camera);
        }

#ifdef SPEED_TEST
        ElapsedCPU timer;
        glFinish();
        timer.start();
#endif

        //future http://www.gamerendering.com/category/lighting/
        Camera left=drender.leftEye(camera);
        Camera right=drender.rightEye(camera);

        gbuffer.bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0,0, gbuffer.width()/2.0, gbuffer.height());
        renderWorld(world, left.perspectiveLook(), left);

        glViewport(gbuffer.width()/2.0,0, gbuffer.width()/2.0, gbuffer.height());
        renderWorld(world, right.perspectiveLook(), right);

        gbuffer.unbind();
        mutex.unlock();

        dbuffer.bind();
        renderScene(world, camera);
        dbuffer.release();

#ifdef SPEED_TEST
        glFinish();
        cout<<"Render buffers:"<<timer.elapsed()<<"ms"<<endl;
#endif

    }
}

void SceneRendererDeferred::renderWorld(WorldGraphics * world, const Matrix4f &mvp, const Camera & cam)
{
    clearRenderLists(world);
    world->map.buildRenderList(cam);

    drawer.renderPrev(world, cam, elapsed);
    if(world->environment.wireframe && glVersion()==OpenGL::OPENGL_3_0)
    {
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        drawer.render(world, mvp, elapsed);
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }
    else
    {
        drawer.render(world, mvp, elapsed);
    }
}

void SceneRendererDeferred::renderShadow(WorldGraphics * world, int elapsed, const Camera & cam)
{
    world->map.buildRenderListAll();
#ifdef SPEED_TEST
    ElapsedCPU timer;

    glFinish();
    timer.start();
#endif
    shadow.bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    if(glVersion()==OPENGL_3_0)
    {
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }

    glViewport(0, 0, shadow.width(), shadow.height());
    shadow_matrix=world->environment.sun.shadowMVP(world->map.width(),world->map.height(),world->map.depth());
    drawer.renderShadow(world, shadow_matrix, elapsed);
    clearRenderLists(world);
    shadow.unbind();
#ifdef SPEED_TEST
    glFinish();
    cout<<"Render shadow:"<<timer.elapsed()<<"ms"<<endl;
#endif
}

void SceneRendererDeferred::clearRenderLists(WorldGraphics * world)
{
    world->terrain.clearRenderList();
    world->model.clearRenderList();
}

void SceneRendererDeferred::resizeBuffers()
{
    if(recheck)
    {
        recheck=false;
        render_shadow=true;
        shadow.resize(screen_width*2, screen_height*2);
        gbuffer.resize(screen_width, screen_height);
        dbuffer.resize(screen_width,screen_height);
    }
}

int SceneRendererDeferred::elapsedMilli()
{
    timeb tb;
    ftime(&tb);
    int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
    return nCount;
}

#undef SPEED_TEST
