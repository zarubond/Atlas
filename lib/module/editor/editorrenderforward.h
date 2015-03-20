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

#ifndef EDITORRENDERFORWARD_H
#define EDITORRENDERFORWARD_H

#include "../modulerender.h"
#include "editorbase.h"

class EditorRenderForward : public ModuleRender
{
public:
    EditorRenderForward();
    bool init();
    void render(WorldGraphics * world, const Environment *envi, const Matrix4f &mvp, unsigned int elapsed);

    void renderPrev(WorldGraphics *world, const Environment *envi, const Camera &camera, unsigned int elapsed);
private:
    void renderBox(WorldGraphics *world, const Environment *envi, const Matrix4f &mvp);
    void renderNavigator(EditorGraphics *editor, const Instrument *navig, const Environment *envi, const Matrix4f &mvp);
    void renderPositioning(EditorGraphics *editor, const Matrix4f &mvp);

    bool loadShader();

    void loadNavigatorSphere(EditorGraphics *editor);
    void loadNavigatorCube(EditorGraphics *editor);
    void loadBox(EditorGraphics *editor);
    void loadPosition(EditorGraphics *editor);
    void loadScale(EditorGraphics *editor);

    Shader program;
    Uniform uniform_mvp, uniform_pos, uniform_scale, uniform_dir;
    Attribute a_coord, a_color;

};

#endif // EDITORRENDERFORWARD_H
