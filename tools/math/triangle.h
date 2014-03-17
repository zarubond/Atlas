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
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "math.h"

class Triangle
{
public:
    Triangle();
    /**
     * @brief set Setup triangle.
     * @param a
     * @param b
     * @param c
     */
    void set(const Vertex3<float> & a,const Vertex3<float> & b,const Vertex3<float> & c);
    /**
     * @brief collision Ray-triangle collision.
     * @param start
     * @param end
     * @return
     */
    Vertex3<float> collision(const Vertex3<double> & start,const Vertex3<double> & end);
    /**
     * @brief vertex0 Set vertex position
     * @param a
     */
    void vertex0(const Vertex3<float> & a);
    void vertex0(float x,float y,float z);
    void vertex1(const Vertex3<float> & b);
    void vertex1(float x,float y,float z);
    void vertex2(const Vertex3<float> & c);
    void vertex2(float x,float y,float z);
    /**
     * @brief normal Triangle normal.
     * @return
     */
    Vertex3<float> normal();
    /**
     * @brief normal Normal of given triangle/
     * @param a
     * @param b
     * @param c
     * @return
     */
    static Vertex3<float> normal(const Vertex3<float> &a, const Vertex3<float> &b, const Vertex3<float> &c);
private:
    Vertex3<float> v0,v1,v2;

};

#endif // TRIANGLE_H
