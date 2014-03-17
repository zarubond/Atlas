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
#include "map.h"

Map::Map()
{
    space_width=0;
    space_height=0;
    space_depth=0;
    special_count=0;
}

Map::~Map()
{
}

bool Map::setup(int width, int height, int depth)
{
    this->space_width=width;
    this->space_height=height;
    this->space_depth=depth;

    root.build(0,0,0,1024,nodeSize());
    return true;
}

void Map::buildRenderList(const Camera &camera)
{
    root.selectVisible(camera);
    for(int i=0;i<special_count;i++)
        special[i]->selectDraw(camera);

}

bool Map::insertObject(MapObject *object)
{
    return root.add(object);
}

void Map::insertOutOfTree(MapObject *object)
{
    special[special_count++]=object;
}

bool Map::removeObject(MapObject *object)
{
    return root.remove(object);
}

Collision Map::collision(const Vertex3f a, const Vertex3f b)
{
    Collision end;
    end.collision=b;
    end.normal=Vertex3f(0.0);
    end.object=NULL;
    Vertex3f e=b;
    Vertex3f v=b-a;
    float x=0;
    if(e[1]<x)
    {
        e[0]=a[0]+((x-a[1])*v[0])/v[1];
        e[1]=x;
        e[2]=a[2]+((x-a[1])*v[2])/v[1];
    }
    /*
    if(start[1] < pos_y)
    {
        start[0]=a[0]+((pos_y-a[1])*v[0])/v[1];
        start[1]=pos_y;
        start[2]=a[2]+((pos_y-a[1])*v[2])/v[1];
    }


    if(e[0] < 0)
    {
        start[0]=pos_x;
        start[1]=a[1]+((pos_x-a[0])*v[1])/v[0];
        start[2]=a[2]+((pos_x-a[0])*v[2])/v[0];
    }

    if(e[1] < pos_y)
    {
        start[0]=a[0]+((pos_y-a[1])*v[0])/v[1];
        start[1]=pos_y;
        start[2]=a[2]+((pos_y-a[1])*v[2])/v[1];
    }

    if(e[2] < pos_z)
    {
        start[0]=a[0]+((pos_z-a[2])*v[0])/v[2];
        start[1]=a[1]+((pos_z-a[2])*v[1])/v[2];
        start[2]=pos_z;
    }*/

    end.collision=e;

    if(a!=b)
        root.collision(a,end);

    return end;
}

Collision Map::collisionVisible(const Vertex3f & a, const Vertex3f & b, const Camera &camera)
{
    Collision end;
    end.collision=b;
    end.normal=Vertex3f(0.0);
    end.object=NULL;
    Vertex3f e=b;
    Vertex3f v=b-a;
    float x=0;
    if(e[1]<x)
    {
        e[0]=a[0]+((x-a[1])*v[0])/v[1];
        e[1]=x;
        e[2]=a[2]+((x-a[1])*v[2])/v[1];
    }
    /*
    if(start[1] < pos_y)
    {
        start[0]=a[0]+((pos_y-a[1])*v[0])/v[1];
        start[1]=pos_y;
        start[2]=a[2]+((pos_y-a[1])*v[2])/v[1];
    }


    if(e[0] < 0)
    {
        start[0]=pos_x;
        start[1]=a[1]+((pos_x-a[0])*v[1])/v[0];
        start[2]=a[2]+((pos_x-a[0])*v[2])/v[0];
    }

    if(e[1] < pos_y)
    {
        start[0]=a[0]+((pos_y-a[1])*v[0])/v[1];
        start[1]=pos_y;
        start[2]=a[2]+((pos_y-a[1])*v[2])/v[1];
    }

    if(e[2] < pos_z)
    {
        start[0]=a[0]+((pos_z-a[2])*v[0])/v[2];
        start[1]=a[1]+((pos_z-a[2])*v[1])/v[2];
        start[2]=pos_z;
    }*/

    end.collision=e;

    if(a!=b)
        root.collisionVisible(a,end,camera);

    return end;
}

int Map::maxWidth() const
{
    return 1024;
}

int Map::maxHeight() const
{
    return 1024;
}

int Map::maxDepth() const
{
    return 1024;
}

int Map::width() const
{
    return space_width;
}

int Map::height() const
{
    return space_height;
}

int Map::depth() const
{
    return space_depth;
}

int Map::nodeSize() const
{
    return 32;
}

