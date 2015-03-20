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

#ifndef MESHOBJ_H
#define MESHOBJ_H

#include <vector>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include "file.h"

#include "meshloader.h"


/**
 * @brief The MeshObj class is loading obj file format 3d object.
 */
class MeshObj: public MeshLoader
{
public:
    MeshObj();
    /**
     * @brief Load obj 3d model
     * @param file_name Path to file.
     * @return  Success.
     */
    bool load(const char * file_name, Mesh *mesh);
    /**
     * @brief meshFiles Get all files from mesh
     * @param file_name
     * @return
     */
    std::vector<std::string> meshFiles(const char *file_name);
    std::vector<std::string> meshParts(const char *file_name);
private:
    /**
     * @brief Load model material
     * @param file_name Material file.
     * @param folder Model root folder.
     */
    void loadMTL(const std::string &file_name, const std::string &folder);
    void resize(float *& array,int & max,int size);
    void resize(uint *&array, int & max, int size);
    void resize(unsigned short *&array, int &max, int size);
    Mesh * mesh;
};

#endif // MESHOBJ_H
