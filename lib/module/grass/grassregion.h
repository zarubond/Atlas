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

#ifndef GRASSREGION_H
#define GRASSREGION_H

#include "../../math/math.h"
#include "../../mapobject.h"

class GrassGraphics;

class GrassTuft
{
public:
    Vector3f position;
    float height;
};

class GrassRegion: public MapObject
{
public:
    GrassRegion(GrassGraphics *parent, int x, int y, int z, int size);
    ~GrassRegion();

    void addTuft(const Vector3f &tuft);
    bool removeTuft(const Vector3f & tuft);
    bool removeTuft(int x, int z, float min_y, float max_y);
    bool removeTuft(int id);

    Vector3f getTuft(unsigned id) const;
    const Vector3f *tuftList() const;
    int tuftCount() const;

    void selectDraw(const Camera &);
    void selectDraw();
    int type();
    bool inBox(int x, int y, int z, int w) const;

private:
    Vector3f * tuft_list;
    int tuft_count;
    GrassGraphics * parent;
    int pos_x,pos_y,pos_z,size;
};

#endif // GRASSREGION_H
