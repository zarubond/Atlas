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

#ifndef GRASSGRAPHICS_H
#define GRASSGRAPHICS_H

#include "grassbase.h"
#include "../../graphics/graphics.h"


class GrassGraphics: public GrassBase
{
public:
    GrassGraphics(int type);
    ~GrassGraphics();
    void setTexture(Texture *texture);
    Texture *getTexture();

    void pushRender(GrassRegion * surface);
    GrassRegion * popRender();
    int renderCount();
    void clearRender();

private:
    void createRegion(int x, int y, int z);

    Texture * texture;
    GrassRegion ** render_list;
    int render_count;
};

#endif // GRASSGRAPHICS_H
