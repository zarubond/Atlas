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
#ifndef PROJECT_H
#define PROJECT_H

#include <string>
#include <iostream>
#include <fstream>
#include <stdint.h>

#include <QDir>

#include "../world/world.h"

#include "terrainio.h"
#include "modelio.h"
#include "grassio.h"

class Project
{
public:
    Project();

    TerrainGraphics terrain;
    ModelGraphics model;
    GrassGraphics grass;
    LightManager light;

    TerrainIO terrain_io;
    ModelIO model_io;
    GrassIO grass_io;

//first to be deleted is map
    Map map;

    QDir root_dir;

    QString name;
    QString project_file;

private:
};

#endif // PROJECT_H
