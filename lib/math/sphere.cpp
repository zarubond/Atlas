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

#include "sphere.h"

Sphere::Sphere()
{
    radius=0;
    center.set(0,0,0);
}

Sphere::Sphere(Vector3<float> center, float radius)
{
    this->center=center;
    this->radius=radius;
}

void Sphere::set(Vector3<float> center, float radius)
{
    this->center=center;
    this->radius=radius;
}

void Sphere::set(float x, float y, float z, float radius)
{
    center.set(x,y,z);
    this->radius=radius;
}

Vector3<float> Sphere::intersection(const Vector3<float> &start, const Vector3<float> &end)
{/*
    float a,b,c;
    Vertex3f d=end-start;
    //d-=end;

    a = d.x*d.x + d.y*d.y + d.z*d.z;
    b = 2*d.x*(start.x-center.x) +  2*d.y*(start.y-center.y) +  2*d.z*(start.z-center.z);
    c = center.x*center.x + center.y*center.y + center.z*center.z + start.x*start.x + start.y*start.y + start.z*start.z
             -2*(center.x*start.x + center.y*start.y + center.z*start.z) - radius*radius;

    float disc=b*b-4*a*c;
    if(disc>=0 && radius<1)
    {
        std::cout<<"jop"<<std::endl;
        float p=(-b+sqrt(disc))/(2*a);
        d*=p;
        d+=start;
        std::cout<<d.x<<" "<<d.y<<" "<<d.z<<std::endl;
        std::cout<<radius<<std::endl;
        return d;
    }*/

    //Vertex3d v=end-start;
    //Vertex3d ret=end;
    Vector3<double> v(end[0]-start[0],end[1]-start[1],end[2]-start[2]);
    Vector3<double> ret(end[0],end[1],end[2]);
    double a,b,c;
    a=v[0]*v[0]+v[1]*v[1]+v[2]*v[2];
    b=2*(v[0]*(start[0]-center[0])+v[1]*(start[1]-center[1])+v[2]*(start[2]-center[2]));
    c=start[0]*start[0]+start[1]*start[1]+start[2]*start[2]+center[0]*center[0] + center[1]*center[1] + center[2]*center[2]
            -2*(center[0]*start[0] + center[1]*start[1] + center[2]*start[2])-radius*radius;
    double disc=b*b-4*a*c;
    if(disc>=0)
    {
        double p=(-b+sqrt(disc))/(2*a);
        ret=v;
        ret*=p;
        ret+=start;
    }


    return Vector3<float>(ret[0],ret[1],ret[2]);
}

void Sphere::createSphere(float *& vertices,int &count)
{
    int n=0;
    float verts[2000];
    float r=1;
    const int space = 20;

    for(float b = 0; b <= 180 - space; b+=space)
    {
        for(float a = 0; a <= 360 - space; a+=space)
        {
            verts[n++] = r * sin((a) / 180 * M_PI) * sin((b) / 180 *M_PI);
            verts[n++] = r * cos((a) / 180 * M_PI) * sin((b) / 180 *M_PI);
            verts[n++] = r * cos((b) / 180 * M_PI);
            /*
            texcoord[t++] = (2 * b) / 360;
            texcoord[t++] = (a) / 360;*/

            verts[n++] = r * sin((a) / 180 * M_PI) * sin((b + space) / 180 * M_PI);
            verts[n++] = r * cos((a) / 180 * M_PI) * sin((b + space) / 180 * M_PI);
            verts[n++] = r * cos((b + space) / 180 * M_PI);
            /*
            texcoord[t++] = (2 * (b + space)) / 360;
            texcoord[t++] = (a) / 360;*/

            verts[n++] = r * sin((a + space) / 180 * M_PI) * sin((b) / 180 * M_PI);
            verts[n++] = r * cos((a + space) / 180 * M_PI) * sin((b) / 180 * M_PI);
            verts[n++] = r * cos((b) / 180 * M_PI);
            /*
            texcoord[t++] = (2 * b) / 360;
            texcoord[t++] = (a + space) / 360;*/

            verts[n++] = r * sin((a + space) / 180 * M_PI) * sin((b + space) /180 * M_PI);
            verts[n++] = r * cos((a + space) / 180 * M_PI) * sin((b + space) /180 * M_PI);
            verts[n++] = r * cos((b + space) / 180 * M_PI);
            /*
            texcoord[t++] = (2 * (b + space)) / 360;
            texcoord[t++] = (a + space) / 360;*/
        }

    }
    vertices=verts;
    count=n/3;
}

bool Sphere::inside(Vector3<float> point)
{
    point-=center;
    if(point.length()<radius)
        return true;

    return false;
}

float Sphere::getRadius() const
{
    return radius;
}

const Vector3<float> &Sphere::getPosition() const
{
    return center;
}
