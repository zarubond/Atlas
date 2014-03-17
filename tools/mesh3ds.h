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
#ifndef MESH3DS_H
#define MESH3DS_H

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <stdio.h>


#include "meshloader.h"
//http://www.gamedev.net/page/resources/_/technical/game-programming/loading-3ds-files-r1259
//http://www.spacesimulator.net/wiki/index.php?title=Tutorials:3ds_Loader

class Mesh3ds: public MeshLoader
{
public:
    Mesh3ds();
    bool load(const char *file_name,Mesh * mesh);
    std::vector<std::string> meshFiles(const char *file_name);

private:
    void readChunk(int len);
    void objectBlock(FILE *file);
    void materialBlock(FILE *file);

    unsigned int readInt();
    unsigned long readLong();
    unsigned char readChar();

    FILE * file;
};

#endif // MESH3DS_H
