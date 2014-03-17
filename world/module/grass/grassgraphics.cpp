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
#include "grassgraphics.h"

GrassGraphics::GrassGraphics(int type):GrassBase(type)
{
    texture=NULL;
}

GrassGraphics::~GrassGraphics()
{
    delete texture;
}

void GrassGraphics::build(unsigned short nodes_x, unsigned short nodes_y, unsigned short nodes_z, unsigned short node_size)
{
    render_list = new GrassGarden*[nodes_x*nodes_y*nodes_z];
    this->nodes_x=nodes_x;
    this->nodes_y=nodes_y;
    this->nodes_z=nodes_z;
    this->node_size=node_size;

    grid=new GrassGarden *[nodes_x*nodes_y*nodes_z];

    for(int i=0;i<nodes_x*nodes_y*nodes_z;i++)
        grid[i]=NULL;

    render_count=0;

}

void GrassGraphics::setTexture(Texture *texture)
{
    if(this->texture!=NULL)
        delete this->texture;
    this->texture=texture;
}

Texture *GrassGraphics::getTexture()
{
    return texture;
}

void GrassGraphics::pushRender(GrassGarden *surface)
{
    render_list[render_count++]=surface;
}

GrassGarden *GrassGraphics::popRender()
{
    return render_list[--render_count];
}

int GrassGraphics::renderCount()
{
    return render_count;
}

void GrassGraphics::clearRender()
{
    render_count=0;
}

