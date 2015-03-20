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

#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <mutex>
#include <math.h>
#include <locale.h>
#include <ctime>
#include <sys/timeb.h>

#include "../graphics/graphics.h"
#include "../math/math.h"
#include "../tools/elapsedcpu.h"
#include "scenerender.h"

#include "worldrender.h"

#include "../render/lightambientrender.h"
#include "../render/lightdirectionalrender.h"
#include "../render/fogrender.h"

#include "vr/vrframebuffer.h"
#include "vr/vrrender.h"

//http://horde3d.org/wiki/index.php5?title=Tutorial_-_Setup_Horde_with_Qt5_%26_QtQuick_2.1
/**
 * @brief The Canvas class is place for drawing.
 */

class SceneRendererDeferred: public SceneRender
{
public:
    explicit SceneRendererDeferred();
    virtual ~SceneRendererDeferred();
    /**
     * @brief init Initialize all required things.
     * @return
     */
    bool init();
    /**
     * @brief render
     */
    void renderScene(WorldGraphics * world, const Camera & camera);
    void renderBuffers(WorldGraphics * world, const Camera &camera);
    void renderSceneVR(WorldGraphics * world, const Camera &camera);
    void renderBuffersVR(WorldGraphics *world, const Camera &camera);

    void resize(int w, int h);
private:
    void resizeBuffers();
    int elapsedMilli();

    void renderWorld(WorldGraphics *world, const Matrix4f &mvp, const Camera &cam);
    //to be http://organicvectory.com/index.php?option=com_content&view=article&id=54
    void renderShadow(WorldGraphics *world, int elapsed, const Camera &cam);
    void clearRenderLists(WorldGraphics *world);

    WorldRender drawer;

    std::mutex mutex;

    bool recheck;
    int elapsed;
    bool render_shadow;

    Matrix4f shadow_matrix;
    bool init_gl;
    int screen_width,screen_height;

    LightDirectionalRender directional;
    LightAmbientRender ambient;
    FogRender fog;
    ShadowBuffer shadow;
    GBuffer gbuffer;

    VrFrameBuffer dbuffer;
    VrRender drender;
};


#endif // SCENERENDERERSYSTEM_H
