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

#include "oceanbase.h"

OceanBase::OceanBase(int type): ModuleBase(type)
{
    visible=true;
    enabled=false;
    sea_level=0;
}

float OceanBase::seaLevel() const
{
    return sea_level;
}

bool OceanBase::isEnabled()
{
    return enabled;
}

void OceanBase::setEnabled(bool enable)
{
    enabled=enable;
}

void OceanBase::setSeaLevel(float height)
{
    sea_level=height;
}

float OceanBase::getSeaLevel() const
{
    return sea_level;
}

void OceanBase::setSize(int width, int height)
{
    m_size[0]=width;
    m_size[2]=height;
}

const Vector3f &OceanBase::size()
{
    return m_size;
}
const ColorRGBA & OceanBase::color() const
{
    return m_color;
}

void OceanBase::setColor(const ColorRGBA &color)
{
    m_color = color;
}

void OceanBase::setColor(float r, float g, float b, float a)
{
    m_color.set(r,g,b,a);
}

