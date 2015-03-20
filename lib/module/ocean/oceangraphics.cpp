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

#include "oceangraphics.h"

OceanGraphics::OceanGraphics(int type): OceanBase(type)
{
    sea_level=0.0;
    m_color.set(0.0,0.45,0.5,0.5);
}

void OceanGraphics::load()
{
    createNormalMap();
    createMesh();
}

void OceanGraphics::setEnabled(bool enable)
{
    if(!buffer.isCreated())
    {
        load();
    }
    enabled=enable;
}

void OceanGraphics::createNormalMap()
{
    int width=512;
    int depth=512;

    unsigned char * data = new unsigned char[width*4*depth*4];

    PerlinNoise perlin;
    perlin.setSeed(3);
    perlin.initGradients();
    float frequency=200;

    for(int i=0; i<width;i++)
        for(int j=0; j<depth;j++)
        {
            /*
            float value=perlin.noise(frequency * i / float(width), frequency * j / float(depth), 0);

            data[(i*width+j)*4+0]=(value+1.0)/2.0*255.0;
            data[(i*width+j)*4+1]=(value+1.0)/2.0*255.0;
            data[(i*width+j)*4+2]=(value+1.0)/2.0*255.0;
            data[(i*width+j)*4+3]=255.0;
*/
            float v=perlin.noise(frequency * (i) / float(width), frequency * j / float(depth), 0);
            float v1=perlin.noise(frequency * (i+1) / float(width), frequency * j / float(depth), 0);
            float v2=perlin.noise(frequency * i / float(width), frequency * (j+1) / float(depth), 0);
            //2v = (f(x−1,y) − f(x+1,y), f(x,y−1) − f(x,y+1), 2);
            Vector3f vec((v-v1)*10.0,(v-v2)*10.0,1);
            float denom=sqrt(vec[0]*vec[0]+vec[1]*vec[1]+1);
            vec/=denom;
            vec*=0.5;
            vec+=Vector3f(0.5);

            data[(i*width+j)*4+0]=vec[0]*255.0;
            data[(i*width+j)*4+1]=0;//vec[1]*255.0;
            data[(i*width+j)*4+2]=vec[2]*255.0;
            data[(i*width+j)*4+3]=255.0;
        }

    normal_map.load(data, width, depth, GL_RGBA, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT);
    delete [] data;
}

void OceanGraphics::createMesh()
{
    /*
    GLfloat coordverts[]={
        1.0,0.0,1.0,
        1.0,0.0,0.0,
        0.0,0.0,0.0,

        0.0,0.0,0.0,
        0.0,0.0,1.0,
        1.0,0.0,1.0
    };*/
/*
    GLfloat coordverts[]={
        0.0,0.0,0.0,
        0.5,0.0,0.0,
        0.5,0.0,0.5,

        0.0,0.0,0.0,
        0.0,0.0,0.5,
        0.5,0.0,0.5,

        0.5,0.0,0.0,
        1.0,0.0,0.0,
        1.0,0.0,0.5,

        0.5,0.0,0.0,
        0.5,0.0,0.5,
        1.0,0.0,0.5,
    };*/
    GLfloat coordverts[4*4*18];
    int c=0;
    float s=4;
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
        {
            coordverts[c++]=(1.0/s)*i;
            coordverts[c++]=0.0;
            coordverts[c++]=(1.0/s)*j;

            coordverts[c++]=(1.0/s)*(i+1);
            coordverts[c++]=0.0;
            coordverts[c++]=(1.0/s)*j;

            coordverts[c++]=(1.0/s)*(i+1);
            coordverts[c++]=0.0;
            coordverts[c++]=(1.0/s)*(j+1);

            coordverts[c++]=(1.0/s)*i;
            coordverts[c++]=0.0;
            coordverts[c++]=(1.0/s)*j;

            coordverts[c++]=(1.0/s)*i;
            coordverts[c++]=0.0;
            coordverts[c++]=(1.0/s)*(j+1);

            coordverts[c++]=(1.0/s)*(i+1);
            coordverts[c++]=0.0;
            coordverts[c++]=(1.0/s)*(j+1);
        }

    buffer.create(sizeof(coordverts),coordverts);
}
