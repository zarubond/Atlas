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
#ifndef GRASSGRAPHICS_H
#define GRASSGRAPHICS_H

#include "grassbase.h"

#include "./grassgarden.h"

class GrassGraphics: public GrassBase
{
public:
    GrassGraphics(int type);
    ~GrassGraphics();
    void build(unsigned short nodes_x, unsigned short nodes_y, unsigned short nodes_z, unsigned short node_size);
    void setTexture(Texture *texture);
    Texture *getTexture();

    void pushRender(GrassGarden * surface);
    GrassGarden * popRender();
    int renderCount();
    void clearRender();

private:
    Texture * texture;
    GrassGarden ** grid;
    GrassGarden ** render_list;
    int render_count;
    unsigned short nodes_x,nodes_y,nodes_z,node_size;
};

#endif // GRASSGRAPHICS_H
