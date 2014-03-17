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
#ifndef TERRAINRENDER_H
#define TERRAINRENDER_H


#include "../../../tools/math/math.h"
#include "../../../tools/texture.h"
#include "../../../tools/shader.h"

#include "terraingraphics.h"
#include "../partdrawer.h"


class TerrainRender: public ModuleDrawer
{
public:
    TerrainRender();
    ~TerrainRender();
    /**
     * @brief init Initialize rendering.
     * @param vertex_file Vertex shader file.
     * @param fragment_file Fragment shader file.
     * @return True on success.
     */
    bool init();

    void load(TerrainGraphics * base);
    /**
     * @brief add Add surface to be drawn.
     * @param box
     */
    void clearRenderList();
    /**
     * @brief render Render all added surfaces.
     * @param mvp Model view projection matrix.
     * @param cam_pos Camera position.
     */
    void render(const Matrix4f &mvp, unsigned int);
    void renderShadow(const Matrix4f &mvp,unsigned int);

private:
    GLint attribute_coord3d, attribute_texcoord,attribute_normal,attribute_shadow_coord3d;
    GLint uniform_mvp,uniform_cam_pos,uniform_shadow_mvp;
    GLint uniform_texture_atlas,uniform_texture_normal;
    Shader program,shader_shadow;

    TerrainGraphics * base;

    int sampler_count;
};

#endif // TERRAINRENDER_H
