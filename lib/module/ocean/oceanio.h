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

#ifndef OCEANIO_H
#define OCEANIO_H

#include "../moduleio.h"
#include "oceangraphics.h"

class OceanIO : public ModuleIO
{
public:
    OceanIO();
    void setModel(WorldGraphics * model);
    bool load(const QDir & dir,const QDomElement &root);
    bool save(QXmlStreamWriter &xml);
    bool create(const QDir & dir, WorldGraphics * world);
private:
    void loadOcean(const QDomElement &root);

    OceanGraphics * base;
    Map * map;
};

#endif // OCEANIO_H
