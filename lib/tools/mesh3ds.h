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

#ifndef MESH3DS_H
#define MESH3DS_H

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
    std::vector<std::string> meshParts(const char *file_name);
private:
    struct Chunk
    {
        unsigned short id;                  // The chunk's ID
        unsigned int length;                    // The length of the chunk
        unsigned int read;                 // The amount of bytes read within that chunk
    };

    void readChunk(Chunk * chunk);
    void process(Mesh3ds::Chunk *parent);
    void processEditorChunk(Chunk *parent);
    void processMaterialChunk(Chunk * parent, MeshMaterial *material);
    void processObjectChunk(Chunk * parent, MeshPart *part);
    void processMeshChunk(Chunk * parent, MeshPart *part);
    void processMappingChunk(Chunk *parent, MeshMaterial *material);
    void processColorChunk(Chunk *parent);

    void readVertices(Chunk * parent);
    void readIndices(Chunk * parent, MeshPart *part);
    void readUVCoordinates(Chunk * parent);
    void readFaceMaterial(Chunk * parent, MeshPart *part);

    void readString(Mesh3ds::Chunk *parent, char *str);

    void objectBlock(FILE *file);
    void materialBlock(FILE *file);

    void computeNormals();

    FILE * file;
    std::string folder;
    Mesh * mesh;
    char buffer[500];

    float * vertices,*uv,*normals;
    unsigned short *indices;
    int uv_count,vertex_count,indices_count,normal_count;

};

#endif // MESH3DS_H
