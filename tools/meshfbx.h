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
#ifndef MESHFBX_H
#define MESHFBX_H

//http://irrext.svn.sourceforge.net/viewvc/irrext/trunk/extensions/scene/IMeshLoader/fbx/
//http://blenderartists.org/forum/showthread.php?104305-FBX-Importer-v0-001
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <stdio.h>

#include "meshloader.h"

class MeshFBX : public MeshLoader
{
public:
    MeshFBX();
    /**
     * @brief Load obj 3d model
     * @param file_name Path to file.
     * @return  Success.
     */
    bool load(const char * file_name, Mesh *mesh);
};

#endif // MESHFBX_H
