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
#include "triangle.h"

Triangle::Triangle()
{
}

void Triangle::set(const Vertex3<float> &a, const Vertex3<float> &b, const Vertex3<float> &c)
{
    this->v0=a;
    this->v1=b;
    this->v2=c;
}

Vertex3<float> Triangle::collision(const Vertex3<double> &start, const Vertex3<double> &end)
{
    Vertex3<double> d=end-start;
    Vertex3<double> e1=v1-v0;
    Vertex3<double> e2=v2-v0;

    Vertex3<double> h=d*e2;

    double a=e1.dot(h);
    if (a > -0.000001 && a < 0.000001)
            return end;

    double f=1/a;

    Vertex3<double> s=start-v0;
    double u = f * (s.dot(h));
    if (u < 0.0 || u > 1.0)
            return end;

    Vertex3<double> q=s*e1;

    double v=f*d.dot(q);

    if (v < 0.0 || u + v > 1.0)
        return end;

    double t = f * e2.dot(q);
    if (t >=0 && t < 1.0)
    {
        d*=t;
        d+=start;
        return d;
    }

    return end;
}


void Triangle::vertex0(const Vertex3<float> &a)
{
    this->v0=a;
}

void Triangle::vertex0(float x, float y, float z)
{
    v0.set(x,y,z);
}

void Triangle::vertex1(const Vertex3<float> &b)
{
    this->v1=b;
}

void Triangle::vertex1(float x, float y, float z)
{
    v1.set(x,y,z);
}

void Triangle::vertex2(const Vertex3<float> &c)
{
    this->v2=c;
}

void Triangle::vertex2(float x, float y, float z)
{
    v2.set(x,y,z);
}

Vertex3<float> Triangle::normal()
{
    Vertex3<float> u(v1-v0),v(v2-v0);
    u*=v;
    return u;
}

Vertex3<float> Triangle::normal(const Vertex3<float> & a,const Vertex3<float> & b,const Vertex3<float> & c)
{
    Vertex3<float> u(b-a),v(c-a);
    u*=v;
    return u;//Vertex3f(u.y*v.z-u.z*v.y, u.z*v.x-u.x*v.z, u.x*v.y-u.y*v.x);
}
