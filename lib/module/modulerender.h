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

#ifndef MODULERENDER_H
#define MODULERENDER_H

#include "../math/math.h"
#include "../graphics/opengl.h"
#include "../environment/environment.h"
#include "../worldgraphics.h"

class ModuleRender/*: public OpenGL*/
{
public:

    virtual bool init()=0;

    /**
     * @brief render Render all visible object of given component.
     * @param mvp
     */
    virtual void render(WorldGraphics * world, const Environment *envi, const Matrix4f &mvp, unsigned int elapsed);
    /**
     * @brief render Render all visible object as shadow of given component.
     * @param mvp
     */
    virtual void renderShadow(WorldGraphics * world, const Environment *envi, const Matrix4f &mvp, unsigned int elapsed);
    virtual void renderPrev(WorldGraphics * world, const Environment * envi, const Camera &mvp, unsigned int elapsed);
private:
};


#endif // MODULERENDER_H
