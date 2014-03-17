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
#include "math.h"

Math::Math()
{
}

float fabs(float val)
{
    if(val<0)
        return -val;
    return val;
}

/*
void printGLM(const glm::mat4 & m)
{
    std::cout<<m[0][0]<<" "<<m[0][1]<<" "<<m[0][2]<<" "<<m[0][3]<<std::endl;
    std::cout<<m[1][0]<<" "<<m[1][1]<<" "<<m[1][2]<<" "<<m[1][3]<<std::endl;
    std::cout<<m[2][0]<<" "<<m[2][1]<<" "<<m[2][2]<<" "<<m[2][3]<<std::endl;
    std::cout<<m[3][0]<<" "<<m[3][1]<<" "<<m[3][2]<<" "<<m[3][3]<<std::endl;
}

glm::mat4 matrixGLM(const Matrix4f & mat)
{
    glm::mat4 m;
    m[0][0]=mat[0];
    m[0][1]=mat[1];
    m[0][2]=mat[2];
    m[0][3]=mat[3];

    m[1][0]=mat[4];
    m[1][1]=mat[5];
    m[1][2]=mat[6];
    m[1][3]=mat[7];

    m[2][0]=mat[8];
    m[2][1]=mat[9];
    m[2][2]=mat[10];
    m[2][3]=mat[11];

    m[3][0]=mat[12];
    m[3][1]=mat[13];
    m[3][2]=mat[14];
    m[3][3]=mat[15];
    return m;
}

void printGLM4(const glm::vec4 & v)
{
    std::cout<<v[0]<<" "<<v[1]<<" "<<v[2]<<" "<<v[3]<<std::endl;
}

void printGLM3(const glm::vec3 & v)
{
    std::cout<<v[0]<<" "<<v[1]<<" "<<v[2]<<std::endl;
}
*/
