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
#include "octreenode.h"

OctreeNode::OctreeNode()
{
    for(int i=0;i<8;i++)
        octant[i]=NULL;
    pos_x=0;
    pos_z=0;
    pos_y=0;
    object_list=NULL;
    cnt=0;
}

OctreeNode::OctreeNode(unsigned short x, unsigned short y, unsigned short z, unsigned short node_size, const unsigned short leaf_size)
{
    for(int i=0;i<8;i++)
        octant[i]=NULL;
    object_list=NULL;
    cnt=0;
    build(x,y,z,node_size,leaf_size);
}

OctreeNode::~OctreeNode()
{
    MapObject * tmp=object_list;

    while(object_list!=NULL)
    {
        tmp=object_list;
        object_list=object_list->next;
        delete tmp;
    }
    for(int i=0;i<8;i++)
        delete octant[i];
}

void OctreeNode::build( unsigned short x, unsigned short y,unsigned short z, unsigned short node_size,const unsigned short leaf_size)
{
    this->node_size=node_size;
    this->leaf_size=leaf_size;

    pos_x=x;
    pos_y=y;
    pos_z=z;

    Vertex3f pos(x,y,z);
    bbox.set(pos,node_size);

    float r=(node_size*sqrt(3))/2;
    sphere.set(Vertex3f(x+node_size/2,y+node_size/2,z+node_size/2),r);
}

bool OctreeNode::add(MapObject *obj)
{
    if(obj->inBox(pos_x,pos_y,pos_z,node_size))
    {
        if(node_size==leaf_size)
        {
            obj->next=object_list;
            object_list=obj;
            cnt++;
        }
        else
        {
            if(obj->inBox(pos_x               ,pos_y              ,pos_z              ,node_size/2))
            {
                if(octant[0]==NULL) octant[0]=new OctreeNode(pos_x               ,pos_y              ,pos_z              ,node_size/2,leaf_size);
                octant[0]->add(obj);
            }
            else if(obj->inBox(pos_x+node_size/2   ,pos_y              ,pos_z              ,node_size/2))
            {
                if(octant[1]==NULL) octant[1]=new OctreeNode(pos_x+node_size/2   ,pos_y              ,pos_z              ,node_size/2,leaf_size);
                octant[1]->add(obj);
            }
            else if(obj->inBox(pos_x+node_size/2   ,pos_y+node_size/2  ,pos_z              ,node_size/2))
            {
                if(octant[2]==NULL) octant[2]=new OctreeNode(pos_x+node_size/2   ,pos_y+node_size/2  ,pos_z              ,node_size/2,leaf_size);
                octant[2]->add(obj);
            }
            else if(obj->inBox(pos_x+node_size/2   ,pos_y+node_size/2  ,pos_z+node_size/2  ,node_size/2))
            {
                if(octant[3]==NULL) octant[3]=new OctreeNode(pos_x+node_size/2   ,pos_y+node_size/2  ,pos_z+node_size/2  ,node_size/2,leaf_size);
                octant[3]->add(obj);
            }
            else if(obj->inBox(pos_x               ,pos_y+node_size/2  ,pos_z+node_size/2  ,node_size/2))
            {
                if(octant[4]==NULL) octant[4]=new OctreeNode(pos_x               ,pos_y+node_size/2  ,pos_z+node_size/2  ,node_size/2,leaf_size);
                octant[4]->add(obj);
            }
            else if(obj->inBox(pos_x+node_size/2   ,pos_y              ,pos_z+node_size/2  ,node_size/2))
            {
                if(octant[5]==NULL) octant[5]=new OctreeNode(pos_x+node_size/2   ,pos_y              ,pos_z+node_size/2  ,node_size/2,leaf_size);
                octant[5]->add(obj);
            }
            else if(obj->inBox(pos_x               ,pos_y+node_size/2  ,pos_z              ,node_size/2))
            {
                if(octant[6]==NULL) octant[6]=new OctreeNode(pos_x               ,pos_y+node_size/2  ,pos_z              ,node_size/2,leaf_size);
                octant[6]->add(obj);
            }
            else if(obj->inBox(pos_x               ,pos_y              ,pos_z+node_size/2  ,node_size/2))
            {
                if(octant[7]==NULL) octant[7]=new OctreeNode(pos_x               ,pos_y              ,pos_z+node_size/2  ,node_size/2,leaf_size);
                octant[7]->add(obj);
            }
            else
            {
                obj->next=object_list;
                object_list=obj;
                cnt++;
            }
        }

        return true;
    }

    return false;
}

bool OctreeNode::remove(MapObject *obj)
{
    if(obj->inBox(pos_x,pos_y,pos_z,node_size))
    {
        if(octant[0]!=NULL && octant[0]->remove(obj)) return true;
        else if(octant[1]!=NULL && octant[1]->remove(obj)) return true;
        else if(octant[2]!=NULL && octant[2]->remove(obj)) return true;
        else if(octant[3]!=NULL && octant[3]->remove(obj)) return true;
        else if(octant[4]!=NULL && octant[4]->remove(obj)) return true;
        else if(octant[5]!=NULL && octant[5]->remove(obj)) return true;
        else if(octant[6]!=NULL && octant[6]->remove(obj)) return true;
        else if(octant[7]!=NULL && octant[7]->remove(obj)) return true;
        else
        {
             MapObject * tmp=object_list;
             if(tmp!=NULL)
             {
                if(tmp==obj)
                {
                    object_list=tmp->next;
                    return true;
                }

                while(tmp->next!=NULL)
                {
                    if(tmp->next==obj)
                    {
                        tmp->next=tmp->next->next;
                        return true;
                    }
                    tmp=tmp->next;
                }
            }
        }
    }

    return false;
}

bool OctreeNode::removeForce(MapObject *obj)
{
    return false;
}

bool OctreeNode::add(Light *light)
{
    return false;
}

void OctreeNode::selectVisible(const Camera &camera)
{
    if(!camera.sphereInFrustum(sphere))
    {
        return;
    }

    MapObject * tmp=object_list;

    while(tmp!=NULL)
    {
        tmp->selectDraw(camera);
        tmp=tmp->next;
    }

    if(octant[0]!=NULL) octant[0]->selectVisible(camera);
    if(octant[1]!=NULL) octant[1]->selectVisible(camera);
    if(octant[2]!=NULL) octant[2]->selectVisible(camera);
    if(octant[3]!=NULL) octant[3]->selectVisible(camera);

    if(octant[4]!=NULL) octant[4]->selectVisible(camera);
    if(octant[5]!=NULL) octant[5]->selectVisible(camera);
    if(octant[6]!=NULL) octant[6]->selectVisible(camera);
    if(octant[7]!=NULL) octant[7]->selectVisible(camera);
}

void OctreeNode::collision(const Vertex3f &a, Collision &b)
{
    if(bbox.collision(a,b.collision))
    {
        MapObject * tmp=object_list;

        while(tmp!=NULL)
        {
            tmp->collision(a,b);
            tmp=tmp->next;
        }

        if(octant[0]!=NULL) octant[0]->collision(a,b);
        if(octant[1]!=NULL) octant[1]->collision(a,b);
        if(octant[2]!=NULL) octant[2]->collision(a,b);
        if(octant[3]!=NULL) octant[3]->collision(a,b);

        if(octant[4]!=NULL) octant[4]->collision(a,b);
        if(octant[5]!=NULL) octant[5]->collision(a,b);
        if(octant[6]!=NULL) octant[6]->collision(a,b);
        if(octant[7]!=NULL) octant[7]->collision(a,b);
    }
}

void OctreeNode::collisionVisible(const Vertex3f &a, Collision &b, const Camera & camera)
{
    if(!camera.sphereInFrustum(sphere))
    {
        return;
    }

    if(bbox.collision(a,b.collision))
    {
        MapObject * tmp=object_list;

        while(tmp!=NULL)
        {
            tmp->collision(a,b);
            tmp=tmp->next;
        }

        if(octant[0]!=NULL) octant[0]->collisionVisible(a,b,camera);
        if(octant[1]!=NULL) octant[1]->collisionVisible(a,b,camera);
        if(octant[2]!=NULL) octant[2]->collisionVisible(a,b,camera);
        if(octant[3]!=NULL) octant[3]->collisionVisible(a,b,camera);

        if(octant[4]!=NULL) octant[4]->collisionVisible(a,b,camera);
        if(octant[5]!=NULL) octant[5]->collisionVisible(a,b,camera);
        if(octant[6]!=NULL) octant[6]->collisionVisible(a,b,camera);
        if(octant[7]!=NULL) octant[7]->collisionVisible(a,b,camera);
    }
}
