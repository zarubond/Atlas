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
#ifndef SHAPE_H
#define SHAPE_H

#include "../lib/math/math.h"
#include "../lib/tools/shell.h"

/**
 * @brief The Shape class
 */
class Shape
{
public:
    enum Form
    {
        SPHERE=0,
        BOX
    };

    enum Mode
    {
        MAGNET=0,
        POINTER,
        FLAT
    };

    Shape();
    void  setForm(Form form);
    float getRadius() const;
    void setRadius(float radius);
    void setPosition(float x, float y, float z);
    void setPosition(const Vector3f &position);
    Vector3f const& getPosition() const;
    virtual float isovalue(float x, float y, float z) const;
    float isovalue(const Vector3f &position) const;
    int maxX() const;
    int maxY() const;
    int maxZ() const;
    int minX() const;
    int minY() const;
    int minZ() const;
    float getX() const;
    float getY() const;
    float getZ() const;

    Form getForm() const;
protected:
    void updateExtreme();
    float radius;
    Vector3f pos;
    int min_x,min_y,min_z,max_x,max_y,max_z;
    Form form;
};

#endif // SHAPE_H
