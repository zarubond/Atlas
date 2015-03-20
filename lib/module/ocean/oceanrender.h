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

#ifndef OCEANRENDER_H
#define OCEANRENDER_H

#include "../../graphics/graphics.h"
#include "../modulerender.h"
#include "oceangraphics.h"


//http://www.gamedev.net/page/resources/_/technical/graphics-programming-and-theory/rendering-water-as-a-post-process-effect-r2642
//http://mtnphil.wordpress.com/2012/09/23/water-shader-part-3-deferred-rendering/
//http://www.digitalrune.com/Support/Blog/tabid/719/EntryId/208/Water-Rendering-Resources.aspx
class OceanRender: public ModuleRender
{
public:
    OceanRender();
    ~OceanRender();

    bool init();
    void render(WorldGraphics *world, const Environment *envi, const Matrix4f &mvp, unsigned int elapsed);
    void renderShadow(WorldGraphics * world, const Environment *envi, const Matrix4f &, unsigned int);
private:
    void setupRender(OceanGraphics *ocean);
    Shader shader;
    Uniform u_mvp, u_time, u_noise, u_water_height, u_size, u_color;
    Attribute a_vertex;
};

#endif // OCEANRENDER_H
