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

#include "grassgraphics.h"

GrassGraphics::GrassGraphics(int type)
    :GrassBase(type)
{
    texture=NULL;
    render_count=0;
    render_list=new GrassRegion *[1024];
}

GrassGraphics::~GrassGraphics()
{
    delete texture;
    delete [] render_list;
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

void GrassGraphics::pushRender(GrassRegion *surface)
{
    render_list[render_count++]=surface;
}

GrassRegion *GrassGraphics::popRender()
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

void GrassGraphics::createRegion(int x, int y, int z)
{
    GrassRegion * tmp=new GrassRegion(this,x*region_size,y*region_size,z*region_size,region_size);
    grid[x*nodes_z*nodes_y +y*nodes_z+z]=tmp;
    map->insertObject(tmp);
}

