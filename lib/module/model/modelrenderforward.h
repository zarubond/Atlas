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

#ifndef MODELRENDERFORWARD_H
#define MODELRENDERFORWARD_H

#include"../modulerender.h"
#include "modelgraphics.h"

class ModelRenderForward : public ModuleRender
{
public:
    ModelRenderForward();
    bool init();
    void render(WorldGraphics * world, const Environment *envi, const Matrix4f &mvp, unsigned int);
private:
    void renderMesh(ModelMesh *mesh);
    void setupRender(ModelMesh * mesh);

    Shader program;
    Attribute attribute_coord, attribute_normal, attribute_texcoord;
    Uniform  uniform_mvp, uniform_mv, uniform_texture, uniform_selected;
    Uniform uniform_diffuse, uniform_ambient, uniform_sun_dir;
};

#endif // MODELRENDERFORWARD_H
