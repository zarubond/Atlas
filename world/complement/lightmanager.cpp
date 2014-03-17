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
#include "lightmanager.h"

LightManager::LightManager()
{
    sun.setDiffuse(0.99,0.99,0.99);
    sun.setDirection(0.3,-0.5,0.3);
    sun.setPosition(0,0,0);
    sun.setShininess(7);
    sun.setSpecular(0.7,0.7,0.7);
    sun.setType(Light::DIRECTIONAL);
    ambient_color.set(0.4,0.4,0.4);
}

void LightManager::setAmbient(float r, float g, float b)
{
    ambient_color.set(r,g,b);
}

const ColorRGB &LightManager::getAmbient() const
{
    return ambient_color;
}
