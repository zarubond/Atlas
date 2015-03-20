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

#include "triangle.h"

#include "../tools/shell.h"

Triangle::Triangle()
{
}

void Triangle::set(const Vector3<float> &a, const Vector3<float> &b, const Vector3<float> &c)
{
    this->v0=a;
    this->v1=b;
    this->v2=c;
}

Vector3<float> Triangle::collision(const Vector3<double> &start, const Vector3<double> &end)
{
    Vector3<double> d=end-start;
    Vector3<double> e1=v1-v0;
    Vector3<double> e2=v2-v0;

    Vector3<double> h=d*e2;

    double a=e1.dot(h);
    if (a > -0.000001 && a < 0.000001)
            return end;

    double f=1/a;

    Vector3<double> s=start-v0;
    double u = f * (s.dot(h));
    if (u < 0.0 || u > 1.0)
            return end;

    Vector3<double> q=s*e1;

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


void Triangle::setVertex0(const Vector3<float> &a)
{
    this->v0=a;
}

void Triangle::setVertex0(float x, float y, float z)
{
    v0.set(x,y,z);
}

void Triangle::setVertex1(const Vector3<float> &b)
{
    this->v1=b;
}

void Triangle::setVertex1(float x, float y, float z)
{
    v1.set(x,y,z);
}

void Triangle::setVertex2(const Vector3<float> &c)
{
    this->v2=c;
}

void Triangle::setVertex2(float x, float y, float z)
{
    v2.set(x,y,z);
}

Vector3<float> &Triangle::vertex0()
{
    return v0;
}

Vector3<float> &Triangle::vertex1()
{
    return v1;
}

Vector3<float> &Triangle::vertex2()
{
    return v2;
}

Vector3<float> Triangle::normal()
{/*
    Vector3<float> u(v1-v0),v(v2-v0);
    u*=v;
    return u;*/
    Vector3<float> u(v1-v0),v(v2-v0);
    return Vector3<float>(u.y()*v.z()-u.z()*v.y(), u.z()*v.x()-u.x()*v.z(), u.x()*v.y()-u.y()*v.x());
}

Vector3<float> Triangle::normal(const Vector3<float> & a,const Vector3<float> & b,const Vector3<float> & c)
{/*
    Vector3<float> u(b-a),v(c-a);
    u*=v;
    return u;//Vertex3f(u.y*v.z-u.z*v.y, u.z*v.x-u.x*v.z, u.x*v.y-u.y*v.x);*/

    Vector3<float> u(b-a),v(c-a);
    return Vector3<float>(u.y()*v.z()-u.z()*v.y(), u.z()*v.x()-u.x()*v.z(), u.x()*v.y()-u.y()*v.x());
}
