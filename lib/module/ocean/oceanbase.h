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

#ifndef OCEANBASE_H
#define OCEANBASE_H

#include "../modulebase.h"
#include "../../math/math.h"

class OceanBase: public ModuleBase
{
public:
    OceanBase(int type);

    float getSeaLevel() const;
    void setSeaLevel(float height);
    float seaLevel() const;
    bool isEnabled();
    void setEnabled(bool enable);
    void setSize(int width, int height);
    const Vector3f &size();
    const ColorRGBA &color() const;
    void setColor(const ColorRGBA &color);
    void setColor(float r, float g, float b, float a);

protected:
    Vector3f m_size;
    float sea_level;
    bool enabled;
    ColorRGBA m_color;
};

#endif // OCEANBASE_H
