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

#ifndef SCENERENDER_H
#define SCENERENDER_H

#include "../graphics/opengl.h"
#include "../tools/camera.h"
#include "../worldgraphics.h"

class SceneRender : public OpenGL
{
public:
    SceneRender();
    virtual ~SceneRender();
    /**
     * @brief init Initialize rendering system.
     * @return Success.
     */
    virtual bool init()=0;
    /**
     * @brief renderScene Rendef final image of the scene on the screen.
     * @param camera Look which will be rendered.
     */
    virtual void renderScene(WorldGraphics * world, const Camera &camera)=0;
    /**
     * @brief renderBuffers Render images to buffers (for deferred shading)
     * @param camera Look which will be rendered.
     */
    virtual void renderBuffers(WorldGraphics * world, const Camera &camera)=0;
    /**
     * @brief renderSceneVR Render scene as wirtual reality, ie. two images side by side.
     * @param camera Look which will be rendered.
     */
    virtual void renderSceneVR(WorldGraphics * world, const Camera &camera)=0;
    /**
     * @brief renderBuffersVR
     * @param world
     * @param camera
     */
    virtual void renderBuffersVR(WorldGraphics * world, const Camera & camera)=0;
    /**
     * @brief resize the Renderable canvas has been resized.
     * @param w Width of the canvas.
     * @param h Height of the canvas.
     */
    virtual void resize(int w, int h);
};

#endif // SCENERENDER_H
