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
#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include "../lib/tools/camera.h"
#include "../lib/map.h"
#include "shape.h"

#define DTOR 0.01745329252

/**
 * @brief The Navigator class
 */
class Instrument: public Shape
{
public:

    Instrument();
    bool init();

    void render(const Matrix4f & mvp);
    void setDistance(float distance);
    void lockX(bool lock);
    void lockY(bool lock);
    void lockZ(bool lock);
    bool isLockedX();
    bool isLockedY();
    bool isLockedZ();
    Mode getMode() const;
    void setMode(Mode mode);
    void show();
    void hide();
    bool isVisible() const;

    void updatePosition(Camera *camera, Map *map);
    void setPosition(float x, float y, float z);
    void setPosition(const Vector3f &pos);
    void setMouse(int x,int y);
    float getOffset() const;
    void setOffset(float value);

    float maxRadius();
    float minRadius();
    void setRadius(float radius);
private:

    bool lock_x,lock_y,lock_z;
    float distance;
    bool visible;
    int mouse_x,mouse_y;
    float offset;

    Instrument::Mode mode;
};

#endif // INSTRUMENT_H
