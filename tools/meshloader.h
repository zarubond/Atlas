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
#ifndef MESHLOADER_H
#define MESHLOADER_H

#include "./graphics.h"
#include "./math/math.h"


class Mesh;

/**
 * @brief The MeshLoader class
 */
class MeshLoader
{
public:
    MeshLoader();
    ~MeshLoader();

    virtual bool load(const char * file_name, Mesh *mesh)=0;
    virtual std::vector<std::string> meshFiles(const char *file_name)=0;
};

#endif // MESHLOADER_H
