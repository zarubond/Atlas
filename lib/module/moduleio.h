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

#ifndef PARTIO_H
#define PARTIO_H

#include <QXmlStreamWriter>
#include <QDomElement>
#include <QDir>
#include <QObject>
#include "../math/math.h"
#include "../worldgraphics.h"

class ModuleIO;

class ModuleProgressListener
{
public:
    ModuleProgressListener(){}
    virtual void progress(ModuleIO * part,std::string message, float p)=0;
};

class ModuleIO
{
public:
    ModuleIO();
    virtual void setModel(WorldGraphics * model)=0;
    virtual bool load(const QDir & dir,const QDomElement &root)=0;
    virtual bool save(QXmlStreamWriter &xml)=0;
    virtual bool create(const QDir &, WorldGraphics *){ return true;}
    void setProgressListener(ModuleProgressListener * listener);
    void sendProgress(const std::string & message, float p);

    enum QUALITY
    {
        GOOD,
        BETTER,
        BEST
    };

protected:
    bool removeDir(const QDir &dir);
    QDir part_dir;

private:
    ModuleProgressListener * listener;
};

#endif // PARTIO_H
