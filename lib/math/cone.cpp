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

#include "cone.h"

Cone::Cone()
{
}

void Cone::triangleMesh(float *&data, int &count, int step, float height, float radius, const Vector3<float> & position)
{
    float a=2.0*M_PI/float(step);
    count=step*6*3;
    data=new float[count];
    int d=0;

    for(int i=0;i<step;i++)
    {
        //top
        data[d++]=height+position[0];
        data[d++]=0.0f+position[1];
        data[d++]=0.0f+position[2];
/*
        data[d++]=1.0f;
        data[d++]=0.0f;
        data[d++]=0.0f;
*/
        data[d++]=0.0f+position[0];
        data[d++]=cos(i*a)*radius+position[1];
        data[d++]=sin(i*a)*radius+position[2];
/*
        data[d++]=0.0f;
        data[d++]=0.0f;
        data[d++]=0.0f;
*/
        data[d++]=0.0f+position[0];
        data[d++]=cos((i+1.0)*a)*radius+position[1];
        data[d++]=sin((i+1.0)*a)*radius+position[2];
/*
        data[d++]=0.0f;
        data[d++]=0.0f;
        data[d++]=0.0f;
*/
        //bottom
        data[d++]=0.0f+position[0];
        data[d++]=0.0f+position[1];
        data[d++]=0.0f+position[2];
/*
        data[d++]=-1.0f;
        data[d++]=0.0f;
        data[d++]=0.0f;
*/
        data[d++]=0.0f+position[0];
        data[d++]=cos(i*a)*radius+position[1];
        data[d++]=sin(i*a)*radius+position[2];
/*
        data[d++]=0.0f;
        data[d++]=0.0f;
        data[d++]=0.0f;
*/
        data[d++]=0.0f+position[0];
        data[d++]=cos((i+1.0)*a)*radius+position[1];
        data[d++]=sin((i+1.0)*a)*radius+position[2];
/*
        data[d++]=0.0f;
        data[d++]=0.0f;
        data[d++]=0.0f;*/
    }
}
