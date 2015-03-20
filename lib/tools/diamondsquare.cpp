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

#include "diamondsquare.h"

DiamondSquare::DiamondSquare()
{
}

void DiamondSquare::run(int seed, double *array, int width, int height)
{
    this->height=height;
    this->width=width;
    values=array;

    int featuresize=512;
    //values = new float[width * height];
    srand (seed);

    for( int y = 0; y < height; y += featuresize)
        for (int x = 0; x < width; x += featuresize)
        {
            setSample(x, y, random());
            //setSample(x,y,0);
        }

    int samplesize = featuresize;

    double scale = 1.0;

    while (samplesize > 1)
    {

        diamondSquare(samplesize, scale);

        samplesize /= 2;
        scale /= 2.0;
    }

    double val;
    for( int y = 0; y < height; y ++)
        for (int x = 0; x < width; x ++)
        {
            val=sample(x,y);
            if(val>1.0)
                setSample(x, y, 1.0);
            else if(val<-1.0)
                setSample(x,y,-1.0);
            //setSample(x,y,0);
        }
}

double DiamondSquare::sample(int x, int y)
{
    return values[(x & (width - 1)) + (y & (height - 1)) * width];
}

double DiamondSquare::random()
{
    return 1.0 - 2.0 * ((double)rand()/(double)RAND_MAX);
}

void DiamondSquare::setSample(int x, int y, double value)
{
    values[(x & (width - 1)) + (y & (height - 1)) * width] = value;
}

void DiamondSquare::sampleSquare(int x, int y, int size, double value)
{
    int hs = size / 2;

    // a     b
    //
    //    x
    //
    // c     d

    double a = sample(x - hs, y - hs);
    double b = sample(x + hs, y - hs);
    double c = sample(x - hs, y + hs);
    double d = sample(x + hs, y + hs);

    setSample(x, y, ((a + b + c + d) / 4.0) + value);

}

void DiamondSquare::sampleDiamond(int x, int y, int size, double value)
{
    int hs = size / 2;

    //   c
    //
    //a  x  b
    //
    //   d

    double a = sample(x - hs, y);
    double b = sample(x + hs, y);
    double c = sample(x, y - hs);
    double d = sample(x, y + hs);

    setSample(x, y, ((a + b + c + d) / 4.0) + value);
}

void DiamondSquare::diamondSquare(int stepsize, double scale)
{

    int halfstep = stepsize / 2;

    for (int y = halfstep; y < height + halfstep; y += stepsize)
    {
        for (int x = halfstep; x < width + halfstep; x += stepsize)
        {
            sampleSquare(x, y, stepsize, random() * scale);
        }
    }

    for (int y = 0; y < height; y += stepsize)
    {
        for (int x = 0; x < width; x += stepsize)
        {
            sampleDiamond(x + halfstep, y, stepsize, random() * scale);
            sampleDiamond(x, y + halfstep, stepsize, random() * scale);
        }
    }

}
