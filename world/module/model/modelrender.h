/**
 *  Atlas - Volumetric terrain editor
 *  Copyright (C) 2012-2013  Ondřej Záruba
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
#ifndef MODELRENDER_H
#define MODELRENDER_H

#include "../partdrawer.h"
#include "modelgraphics.h"

class ModelRender: public ModuleDrawer
{
public:
    ModelRender();

    bool init();
    void load(ModelGraphics * base);
    void render(const Matrix4f &mvp, unsigned int);
    void renderShadow(const Matrix4f &mvp,unsigned int);

    void clearRenderList();

    Model *selectModel(int x, int y, const Camera &camera);

    void renderModel(Model *model);
private:

    ModelGraphics * base;

    Shader program;
    GLint attribute_v_coord,attribute_normal,attribute_texcoord;
    GLint  uniform_mvp,uniform_mv,uniform_texture,uniform_selected,uniform_model;
};

#endif // MODELRENDER_H
