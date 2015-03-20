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

#ifndef PERLINNOISE_H
#define PERLINNOISE_H

#include <cstdlib>
#include "../math/math.h"

class PerlinNoise
{
public:
    PerlinNoise();
    void setSeed(unsigned int seed);

    float noise(float x, float y, float z);
    void initGradients();
    int permutate(int x);
    int index(int ix, int iy, int iz);
    float lattice(int ix, int iy, int iz, float fx, float fy, float fz);
    float lerp(float t, float value0, float value1);
    float smooth(float x);

private:
    unsigned int seed;
    const int GradientSizeTable;
    float _gradients[256*3];

    static unsigned char _perm[];
};

#endif // PERLINNOISE_H
