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
#include "cube.h"

Cube::Cube()
{
}

void Cube::set(Vertex3<float> pos, int size)
{
    corner[0]=pos;
    corner[1]=pos;
    corner[1]+=size;
}

bool Cube::collision(const Vertex3<float> &a, const Vertex3<float> &b)
{
    Vertex3<float> dir=b-a;
    Vertex3<float> inv_dir(1.0);
    inv_dir/=dir;
    Vertex3<int> sign(inv_dir[0]<0,inv_dir[1]<0,inv_dir[2]<0);

    float tmin = (corner[sign[0]  ][0] - a[0]) * inv_dir[0];
    float tmax = (corner[1-sign[0]][0] - a[0]) * inv_dir[0];
    float tymin =(corner[sign[1]  ][1] - a[1]) * inv_dir[1];
    float tymax =(corner[1-sign[1]][1] - a[1]) * inv_dir[1];

    if ( (tmin > tymax) || (tymin > tmax) )
      return false;
    if (tymin > tmin)
      tmin = tymin;
    if (tymax < tmax)
      tmax = tymax;

    float tzmin = (corner[sign[2]][2] - a[2])   * inv_dir[2];
    float tzmax = (corner[1-sign[2]][2] - a[2]) * inv_dir[2];
    if ( (tmin > tzmax) || (tzmin > tmax) )
      return false;
    if (tzmin > tmin)
      tmin = tzmin;
    if (tzmax < tmax)
      tmax = tzmax;

    return ( (tmin < 1) && (tmax > 0) );
}

void Cube::cubeVertices(GLfloat *& array, GLint &count)
{
    GLfloat cube_vertices[] = {
    // front
    -1.0, -1.0,  1.0,
     1.0, -1.0,  1.0,
     1.0,  1.0,  1.0,
    -1.0,  1.0,  1.0,
    // top
    -1.0,  1.0,  1.0,
     1.0,  1.0,  1.0,
     1.0,  1.0, -1.0,
    -1.0,  1.0, -1.0,
    // back
     1.0, -1.0, -1.0,
    -1.0, -1.0, -1.0,
    -1.0,  1.0, -1.0,
     1.0,  1.0, -1.0,
    // bottom
    -1.0, -1.0, -1.0,
     1.0, -1.0, -1.0,
     1.0, -1.0,  1.0,
    -1.0, -1.0,  1.0,
    // left
    -1.0, -1.0, -1.0,
    -1.0, -1.0,  1.0,
    -1.0,  1.0,  1.0,
    -1.0,  1.0, -1.0,
    // right
     1.0, -1.0,  1.0,
     1.0, -1.0, -1.0,
     1.0,  1.0, -1.0,
     1.0,  1.0,  1.0,
    };

    count=sizeof(cube_vertices)/sizeof(GLfloat);
    array=new GLfloat[count];
    for(int i=0;i<count;i++)
    {
        array[i]=cube_vertices[i];
    }
}

void Cube::verticesLines(GLfloat *&array, GLint &count)
{
    GLfloat cube_vertices[] = {
        //bottom
        -0.5,-0.5,-0.5,
        0.5,-0.5,-0.5,

        -0.5,-0.5,0.5,
        0.5,-0.5,0.5,

        -0.5,-0.5,-0.5,
        -0.5,-0.5,0.5,

        0.5,-0.5,-0.5,
        0.5,-0.5,0.5,
//top
        -0.5,0.5,-0.5,
        0.5,0.5,-0.5,

        -0.5,0.5,0.5,
        0.5,0.5,0.5,

        -0.5,0.5,-0.5,
        -0.5,0.5,0.5,

        0.5,0.5,-0.5,
        0.5,0.5,0.5,
        //side

        0.5,-0.5,0.5,
        0.5,0.5,0.5,

        0.5,-0.5,-0.5,
        0.5,0.5,-0.5,

        -0.5,-0.5,0.5,
        -0.5,0.5,0.5,

        -0.5,-0.5,-0.5,
        -0.5,0.5,-0.5,
    };

    count=sizeof(cube_vertices)/sizeof(GLfloat);
    array=new GLfloat[count];
    for(int i=0;i<count;i++)
    {
        array[i]=cube_vertices[i];
    }
}

void Cube::cubeElements(GLushort *&array, GLint &count)
{
    GLushort cube_elements[] = {
        // front
        0,  1,  2,
        2,  3,  0,
        // top
        4,  5,  6,
        6,  7,  4,
        // back
        8,  9, 10,
        10, 11,  8,
        // bottom
        12, 13, 14,
        14, 15, 12,
        // left
        16, 17, 18,
        18, 19, 16,
        // right
        20, 21, 22,
        22, 23, 20,
    };

    count=sizeof(cube_elements)/sizeof(GLushort);
    array=new GLushort[count];
    for(int i=0;i<count;i++)
        array[i]=cube_elements[i];
}
