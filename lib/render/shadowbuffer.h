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

#ifndef SHADOWBUFFER_H
#define SHADOWBUFFER_H

#include "../graphics/graphics.h"

class ShadowBuffer : public OpenGL
{
public:
    ShadowBuffer();
    bool init();
    void resize(int m_width, int m_height);
    void bind();
    void unbind();
    void bindTexture();
    int width()const;
    int height()const;

private:
    int m_width, m_height;
    GLuint texture;
    GLuint fbo;
};

#endif // SHADOWBUFFER_H
