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
#ifndef PARTIO_H
#define PARTIO_H

#include <QXmlStreamWriter>
#include <QDomElement>
#include <QDir>
#include <QDebug>
#include <QObject>
#include "../tools/math/math.h"
#include "../world/world.h"

class PartIO
{
public:
    PartIO()
    {}

    virtual bool load(const QDir & dir,const QDomElement &root,Map *map)=0;
    virtual bool save(QXmlStreamWriter &xml)=0;
    virtual bool create(const QDir &dir, Map *map)=0;

    enum QUALITY
    {
        GOOD,
        BETTER,
        BEST
    };

protected:

    bool removeDir(const QDir &dir);

    QDir part_dir;

};

#endif // PARTIO_H
