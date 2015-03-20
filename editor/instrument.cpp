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

#include "instrument.h"

Instrument::Instrument():
    visible(false)
{
    distance=5.0;
    mouse_x=0;
    mouse_y=0;
    lock_x=false;
    lock_y=false;
    lock_z=false;
    offset=0;

    radius=1;
    form=SPHERE;
    mode=POINTER;
    pos.set(0.0,0.0,0.0);
}

float Instrument::getOffset() const
{
    return offset;
}

void Instrument::setOffset(float value)
{
    offset = value;
}

float Instrument::maxRadius()
{
    return 50;
}

float Instrument::minRadius()
{
    return 0.5;
}

void Instrument::setRadius(float radius)
{
    if(radius<maxRadius() && radius>minRadius())
    {
        Shape::setRadius(radius);
    }
}

void Instrument::setPosition(float x, float y, float z)
{
    if(form==BOX)
    {
        //0.5 - must be in center of box
        if(!lock_x)
            pos[0]=(int)x+0.5;
        if(!lock_y)
            pos[1]=(int)y+0.5;
        if(!lock_z)
            pos[2]=(int)z+0.5;
    }
    else
    {
        if(!lock_x)
            pos[0]=x;
        if(!lock_y)
            pos[1]=y;
        if(!lock_z)
            pos[2]=z;
    }
    updateExtreme();
}

void Instrument::setPosition(const Vector3f &pos)
{
    setPosition(pos[0],pos[1],pos[2]);
}

void Instrument::setMouse(int x, int y)
{
    mouse_x=x;
    mouse_y=y;
}

void Instrument::setDistance(float distance)
{
    this->distance=distance;
}

void Instrument::lockX(bool lock)
{
    lock_x=lock;
}

void Instrument::lockY(bool lock)
{
    lock_y=lock;
}

void Instrument::lockZ(bool lock)
{
    lock_z=lock;
}

bool Instrument::isLockedX()
{
    return lock_x;
}

bool Instrument::isLockedY()
{
    return lock_y;
}

bool Instrument::isLockedZ()
{
    return lock_z;
}

void Instrument::updatePosition(Camera *camera, Map * map)
{

    if(!visible) return;

    Vector3f dir=camera->unproject(mouse_x,mouse_y,0.0);
    dir.normalize();

    Vector3f start(camera->position());
    Vector3f target=pos;

    switch(mode)
    {
    case POINTER:
        target=camera->position();
        target+=Vector3f(distance*dir[0], distance*dir[1], distance*dir[2]);
        //target.set(camera->getX() + distance*dir[0],camera->getY() + distance*dir[1],camera->getZ() + distance*dir[2]);
    break;
    case MAGNET:
    {
        Vector3f end(camera->position());
        end+=Vector3f(3000*dir[0], 3000*dir[1], 3000*dir[2]);//(camera->getX() + 3000*dir[0],camera->getY() + 3000*dir[1],camera->getZ() + 3000*dir[2]);

        Collision c=map->collisionVisible(start,end, *camera);
        target=start-end;
        target.normalize();
        target*=offset*radius;
        target+=c.collision;
    }
        break;
    case FLAT:
        break;
    default:
        break;
    }


    if(lock_x)
    {
        Vector3f t=target-start;
        t*=(pos[0]-camera->position()[0])/t[0];
        target=start+t;
    }

    if(lock_y)
    {
        Vector3f t=target-start;
        t*=(pos[1]-camera->position()[1])/t[1];
        target=start+t;
    }

    if(lock_z)
    {
        Vector3f t=target-start;
        t*=(pos[2]-camera->position()[2])/t[2];
        target=start+t;
    }

    setPosition(target);
}

Instrument::Mode Instrument::getMode() const
{
    return mode;
}

void Instrument::setMode(Instrument::Mode mode)
{
    this->mode=mode;
}

void Instrument::show()
{
    visible=true;
}

void Instrument::hide()
{
    visible=false;
}

bool Instrument::isVisible() const
{
    return visible;
}

/*
void Navigator::distance(GLfloat dist)
{
    this->dist=dist;
}

void Navigator::angle(GLfloat x, GLfloat y)
{
    angle_x=x;
    angle_y=y;
}

void Navigator::setMode(NavigatorMode mode)
{
    this->mode=mode;
}
*/
