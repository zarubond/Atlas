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

#include "map.h"

Map::Map()
{
    space_width=0;
    space_height=0;
    space_depth=0;
    out_tree_count=0;
    m_changed=true;
}

Map::~Map()
{
}

bool Map::setup(int width, int height, int depth)
{
    this->space_width=width;
    this->space_height=height;
    this->space_depth=depth;
    int max=1;

    for(int i=0;i<16;i++) if(width<=max<<i){max=max<<i;break;}
    for(int i=0;i<16;i++) if(height<=max<<i){max=max<<i;break;}
    for(int i=0;i<16;i++) if(depth<=max<<i){max=max<<i;break;}

    root.build(0,0,0,max,nodeSize());
    return true;
}

void Map::buildRenderList(const Camera &camera)
{
    root.selectVisible(camera);
    for(int i=0;i<out_tree_count;i++)
        out_tree[i]->selectDraw(camera);
}

void Map::buildRenderListAll()
{
    root.selectAll();
    for(int i=0;i<out_tree_count;i++)
        out_tree[i]->selectDraw();
}

bool Map::insertObject(MapObject *object)
{
    m_changed=true;
    return root.add(object);
}

void Map::insertOutOfTree(MapObject *object)
{
    m_changed=true;
    out_tree[out_tree_count++]=object;
}

void Map::removeOutOfTree(MapObject *object)
{
    m_changed=true;
    for(int i=0; i<out_tree_count;i++)
        if(out_tree[i]==object)
        {
            out_tree_count--;
            for(int j=i;j<out_tree_count;j++)
                out_tree[j]=out_tree[i+1];
        }
}

bool Map::removeObject(MapObject *object)
{
    m_changed=true;
    return root.remove(object);
}

Collision Map::collision(const Vector3f a, const Vector3f b)
{
    Collision end;
    end.collision=b;
    end.normal=Vector3f(0.0f,0.0f,0.0f);
    end.object=NULL;
    Vector3f e=b;
    //Vertex3f v=b-a;
    //float x=0;
    /*if(e[1]<x)
    {
        e[0]=a[0]+((x-a[1])*v[0])/v[1];
        e[1]=x;
        e[2]=a[2]+((x-a[1])*v[2])/v[1];
    }

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

Collision Map::collisionVisible(const Vector3f & a, const Vector3f & b, const Camera &camera)
{
    Collision end;
    end.collision=b;
    end.normal=Vector3f(0.0f,0.0f,0.0f);
    end.object=NULL;
    Vector3f e=b;
    //Vertex3f v=b-a;
    //float x=0;


    /*if(e[1]<x)
    {
        e[0]=a[0]+((x-a[1])*v[0])/v[1];
        e[1]=x;
        e[2]=a[2]+((x-a[1])*v[2])/v[1];
    }

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
/*
    if(end.collision[0]>width())
    {
        float t=float(width())/end.collision[0];
        end.collision*=t;
    }

    if(end.collision[1]>height())
    {
        float t=float(height())/end.collision[1];
        end.collision*=t;
    }

    if(end.collision[2]>depth())
    {
        float t=float(depth())/end.collision[2];
        end.collision*=t;
    }
*/
    return end;
}

int Map::maxWidth() const
{
    return 4096;
}

int Map::maxHeight() const
{
    return 4096;
}

int Map::maxDepth() const
{
    return 4096;
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
    return 32;//32
}

void Map::checkBondarys()
{

}
bool Map::changed() const
{
    return m_changed;
}

void Map::setChanged(bool changed)
{
    m_changed = changed;
}


