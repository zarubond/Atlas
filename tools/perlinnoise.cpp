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
#include "perlinnoise.h"

PerlinNoise::PerlinNoise(): GradientSizeTable(256)
{
    seed=1;
}

void PerlinNoise::setSeed(unsigned int seed)
{
    this->seed=seed;
}

float PerlinNoise::noise(float x, float y, float z)
{
    int ix = floor(x);
    float fx0 = x - ix;
    float fx1 = fx0 - 1;
    float wx = smooth(fx0);

    int iy = floor(y);
    float fy0 = y - iy;
    float fy1 = fy0 - 1;
    float wy = smooth(fy0);

    int iz = floor(z);
    float fz0 = z - iz;
    float fz1 = fz0 - 1;
    float wz = smooth(fz0);

    float vx0 = lattice(ix, iy, iz, fx0, fy0, fz0);
    float vx1 = lattice(ix + 1, iy, iz, fx1, fy0, fz0);
    float vy0 = lerp(wx, vx0, vx1);

    vx0 = lattice(ix, iy + 1, iz, fx0, fy1, fz0);
    vx1 = lattice(ix + 1, iy + 1, iz, fx1, fy1, fz0);
    float vy1 = lerp(wx, vx0, vx1);

    float vz0 = lerp(wy, vy0, vy1);

    vx0 = lattice(ix, iy, iz + 1, fx0, fy0, fz1);
    vx1 = lattice(ix + 1, iy, iz + 1, fx1, fy0, fz1);
    vy0 = lerp(wx, vx0, vx1);

    vx0 = lattice(ix, iy + 1, iz + 1, fx0, fy1, fz1);
    vx1 = lattice(ix + 1, iy + 1, iz + 1, fx1, fy1, fz1);
    vy1 = lerp(wx, vx0, vx1);

    float vz1 = lerp(wy, vy0, vy1);
    return lerp(wz, vz0, vz1);
}

void PerlinNoise::initGradients()
{
    srand(seed);
    for (int i = 0; i < GradientSizeTable; i++)
    {
        float z = 1.0f - 2.0f * ((float)rand()/(float)RAND_MAX);
        float r = sqrt(1.0f - z * z);
        float theta = 2 * M_PI * ((float)rand()/(float)RAND_MAX);
        _gradients[i * 3] = r * cos(theta);
        _gradients[i * 3 + 1] = r * (float)sin(theta);
        _gradients[i * 3 + 2] = z;
    }
}

int PerlinNoise::permutate(int x)
{
    const int mask = GradientSizeTable - 1;
    return _perm[x & mask];
}

int PerlinNoise::index(int ix, int iy, int iz)
{
    return permutate(ix + permutate(iy + permutate(iz)));
}

float PerlinNoise::lattice(int ix, int iy, int iz, float fx, float fy, float fz)
{
    int ind = index(ix, iy, iz);
    int g = ind * 3;
    return _gradients[g] * fx + _gradients[g + 1] * fy + _gradients[g + 2] * fz;
}

float PerlinNoise::lerp(float t, float value0, float value1)
{
    return value0 + t * (value1 - value0);
}

float PerlinNoise::smooth(float x)
{
    return x * x * (3 - 2 * x);
}

unsigned char PerlinNoise::_perm[]={
    225,155,210,108,175,199,221,144,203,116, 70,213, 69,158, 33,252,
    5, 82,173,133,222,139,174, 27,  9, 71, 90,246, 75,130, 91,191,
    169,138,  2,151,194,235, 81,  7, 25,113,228,159,205,253,134,142,
    248, 65,224,217, 22,121,229, 63, 89,103, 96,104,156, 17,201,129,
    36,  8,165,110,237,117,231, 56,132,211,152, 20,181,111,239,218,
    170,163, 51,172,157, 47, 80,212,176,250, 87, 49, 99,242,136,189,
    162,115, 44, 43,124, 94,150, 16,141,247, 32, 10,198,223,255, 72,
    53,131, 84, 57,220,197, 58, 50,208, 11,241, 28,  3,192, 62,202,
    18,215,153, 24, 76, 41, 15,179, 39, 46, 55,  6,128,167, 23,188,
    106, 34,187,140,164, 73,112,182,244,195,227, 13, 35, 77,196,185,
    26,200,226,119, 31,123,168,125,249, 68,183,230,177,135,160,180,
    12,  1,243,148,102,166, 38,238,251, 37,240,126, 64, 74,161, 40,
    184,149,171,178,101, 66, 29, 59,146, 61,254,107, 42, 86,154,  4,
    236,232,120, 21,233,209, 45, 98,193,114, 78, 19,206, 14,118,127,
    48, 79,147, 85, 30,207,219, 54, 88,234,190,122, 95, 67,143,109,
    137,214,145, 93, 92,100,245,  0,216,186, 60, 83,105, 97,204, 52};
