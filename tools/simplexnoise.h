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
#ifndef SIMPLEXNOISE_H
#define SIMPLEXNOISE_H

#include <cmath>
#include <iostream>

//Most of the code is form http://www.6by9.net/simplex-noise-for-c-and-python/
class SimplexNoise
{
public:
    SimplexNoise();
    float octave2d(const float octaves, const float persistence, const float scale, float seed, const float x, const float y);
    float octave3d(const float octaves, const float persistence, const float scale, const float x, const float y, const float z);
    float raw3d(const float x, const float y, const float z);
    float raw2d(const float x, const float y);

private:
    int fastfloor(const float x);
    float dot(const int *g, const float x, const float y);
    float dot(const int *g, const float x, const float y, const float z);

    static const int perm[512];
    static const int grad3[12][3];
};

#endif // SIMPLEXNOISE_H
