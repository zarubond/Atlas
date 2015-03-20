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

#ifndef SCENEFORWARDRENDERER_H
#define SCENEFORWARDRENDERER_H


#include <mutex>
#include <math.h>
#include <locale.h>
#include <ctime>
#include <sys/timeb.h>

#include "../graphics/graphics.h"
#include "../math/math.h"
#include "../tools/elapsedcpu.h"
#include "scenerender.h"

#include "worldrenderforward.h"
#include "vr/vrrender.h"
#include "vr/vrframebuffer.h"
//#include "distortionbuffer.h"
//#include "distortionrender.h"

class SceneRendererForward : public SceneRender
{
public:
    SceneRendererForward();
    virtual ~SceneRendererForward();
    /**
     * @brief init Initialize all required things.
     * @return
     */
    bool init();
    /**
     * @brief render
     */
    void renderScene(WorldGraphics * world, const Camera &camera);
    void renderSceneVR(WorldGraphics * world, const Camera &camera);
    void renderBuffers(WorldGraphics *, const Camera &){}
    void renderBuffersVR(WorldGraphics * world, const Camera & camera);

    void freeze();

    void resize(int w, int h);
private:
    int elapsedMilli();
    void renderWorld(WorldGraphics *world, const Camera &camera);
    void clearRenderLists(WorldGraphics *world);

    WorldRenderForward drawer;

    std::mutex mutex;

    //fps counter
    int elapsed;
    bool recheck;

    bool init_gl;
    int screen_width,screen_height;

    VrFrameBuffer dbuffer;
    VrRender drender;
};

#endif // SCENEFORWARDRENDERER_H
