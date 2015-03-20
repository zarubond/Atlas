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

#include "skyboxio.h"

SkyBoxIO::SkyBoxIO(SkyBoxGraphics *base):base(*base)
{
}

bool SkyBoxIO::load(const QDir &dir, const QDomElement &root)
{
    return true;
}

bool SkyBoxIO::save(QXmlStreamWriter &xml)
{
    return true;
}

bool SkyBoxIO::create(const QDir &dir, WorldGraphics *world)
{
    return false;
}

