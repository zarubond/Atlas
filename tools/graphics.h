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
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "opengl.h"
#include "texture.h"
#include "indexbuffer.h"
#include "vertexbuffrer.h"
#include "shader.h"
#include "texturecube.h"
#include "texture3d.h"
#include "texturearray.h"

class Graphics
{
public:
    Graphics();
    void init();
    static int error();
private:
    //OpenGL ogl;
};

#endif // GRAPHICS_H
