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
    pos_x=0;
    pos_y=0;
    pos_z=0;
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
    pos_x=x;
    pos_y=y;
    pos_z=z;
    updateExtreme();
}

void Shape::setPosition(Vertex3f position)
{
    pos_x=position[0];
    pos_y=position[1];
    pos_z=position[2];
}

Vertex3f Shape::getPosition() const
{
    return Vertex3f(pos_x,pos_y,pos_z);
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
    return pos_z;
}

float Shape::getY() const
{
    return pos_y;
}

float Shape::getZ() const
{
    return pos_x;
}

void Shape::updateExtreme()
{
    float r=ceil(radius)+1.0;
    min_x=pos_x-r;
    if(min_x<0.0) min_x=0.0;
    min_y=pos_y-r;
    if(min_y<0.0) min_y=0.0;
    min_z=pos_z-r;
    if(min_z<0.0) min_z=0.0;

    max_x=pos_x+r;
    max_y=pos_y+r;
    max_z=pos_z+r;
}

float Shape::isovalue(int x, int y, int z) const
{
    //tmp HACK for sphere
    float val=0;
    float a=x-this->pos_x, b=y-this->pos_y, c=z-this->pos_z;

    if(form==SPHERE)
    {
        float r=a*a+b*b+c*c;
        val= (sqrt(r)-radius)*(128.0/radius);
    }
    else//BOX
    {
/*
        Vertex3f ray(a,b,c);
        Vertex3f ray1,ray2,ray3;

        ray1.set(radius,b,c);
        ray2.set(a,radius,c);
        ray3.set(a,b,radius);

        float l1=ray1.length(),l2=ray2.length(),l3=ray3.length();
        if(l1<l2)
        {
            if(l1<l3)
                val=l1;
            else
                val=l3;
        }
        else
        {
            if(l2<l3)
                val=l2;
            else
                val=l3;
        }

        val=ray.length()-val-0.000001;
        val*=127;
*/

        if(x>pos_x-radius && x<pos_x+radius && y>pos_y-radius && y<pos_y+radius && z>pos_z-radius && z<pos_z+radius)
        {
            val=-127.0;
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

float Shape::isovalue(Vertex3f position) const
{
    return isovalue(position[0],position[1],position[2]);
}
