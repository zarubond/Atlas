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

#ifndef SKYBOXRENDERFORWARD_H
#define SKYBOXRENDERFORWARD_H

#include "../modulerender.h"

class SkyBoxRenderForward : public ModuleRender
{
public:
    SkyBoxRenderForward();
    ~SkyBoxRenderForward();

    bool init();
    void renderPrev(WorldGraphics *world, const Environment *envi, const Camera &camera, unsigned int elapsed);
private:
    bool loadShader();
    void renderSetup(SkyBoxGraphics *skybox);
    float x,y,z;
    VertexBuffer vertices;
    IndexBuffer indices;

    Shader shader;
    Attribute a_coord;
    Uniform uniform_mvp, uniform_sun;
};

#endif // SKYBOXRENDERFORWARD_H
