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

#ifndef OCEANRENDERFORWARD_H
#define OCEANRENDERFORWARD_H

#include "../modulerender.h"
#include "oceangraphics.h"

class OceanRenderForward : public ModuleRender
{
public:
    OceanRenderForward();
    ~OceanRenderForward();
    bool init();
    void render(WorldGraphics * world, const Environment *envi, const Matrix4f &mvp, unsigned int elapsed);
private:
    Shader shader;
    Uniform uniform_diffuse, uniform_ambient, uniform_sun_dir;
    Uniform uniform_mvp, uniform_time, uniform_noise, uniform_water_height, u_size, uniform_color;
    Attribute a_vertex;
    VertexArray vao;
    void setupRender(OceanGraphics *ocean);
};

#endif // OCEANRENDERFORWARD_H
