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

#include "shape.h"

Shape::Shape()
{
    min_x=0;
    min_y=0;
    min_z=0;

    max_x=0;
    max_y=0;
    max_z=0;

    radius=0;
    pos.set(0,0,0);
    form=SPHERE;
}

void Shape::setForm(Shape::Form form)
{
    this->form=form;
}

float Shape::getRadius() const
{
    return radius;
}

void Shape::setRadius(float radius)
{
    this->radius=radius;
    updateExtreme();
}

void Shape::setPosition(float x, float y, float z)
{
    pos.set(x,y,z);
    updateExtreme();
}

void Shape::setPosition(const Vector3f & position)
{
    pos=position;
}

const Vector3f &Shape::getPosition() const
{
    return pos;
}

int Shape::maxX() const
{
    return max_x;
}

int Shape::maxY() const
{
    return max_y;
}

int Shape::maxZ() const
{
    return max_z;
}

int Shape::minX() const
{
    return min_x;
}

int Shape::minY() const
{
    return min_y;
}

int Shape::minZ() const
{
    return min_z;
}

float Shape::getX() const
{
    return pos[2];
}

float Shape::getY() const
{
    return pos[1];
}

float Shape::getZ() const
{
    return pos[0];
}

Shape::Form Shape::getForm() const
{
    return form;
}

void Shape::updateExtreme()
{
    float r=ceil(radius)+1.0;
    min_x=pos[0]-r;
    if(min_x<0.0) min_x=0.0;
    min_y=pos[1]-r;
    if(min_y<0.0) min_y=0.0;
    min_z=pos[2]-r;
    if(min_z<0.0) min_z=0.0;

    max_x=pos[0]+r;
    max_y=pos[1]+r;
    max_z=pos[2]+r;
}

float Shape::isovalue(float x, float y, float z) const
{
    //tmp HACK for sphere
    float val=0;
    float a=x-this->pos[0], b=y-this->pos[1], c=z-this->pos[2];

    if(form==SPHERE)
    {
        float r=a*a+b*b+c*c;
        val= (sqrt(r)-radius)*(127.0/radius);
    }
    else//BOX
    {
        if(x>pos[0]-radius && x<pos[0]+radius && y>pos[1]-radius && y<pos[1]+radius && z>pos[2]-radius && z<pos[2]+radius)
        {
            val=-1.0;
        }
        else
            val=127.0;

    }

    if(val>127.0)
        return 127.0;
    if(val<-127.0)
        return -127;
    return val;
}

float Shape::isovalue(const Vector3f & position) const
{
    return isovalue(position[0],position[1],position[2]);
}
