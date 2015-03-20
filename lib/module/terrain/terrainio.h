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

#ifndef TERRAINIO_H
#define TERRAINIO_H

#include <QElapsedTimer>
#include <QByteArray>
#include "../../tools/file.h"

#include "../moduleio.h"
#include "terraincompress.h"
#include "../../tools/elapsedcpu.h"

class TerrainIO: public ModuleIO
{
public:
    TerrainIO();/**
     * @brief setModel Set Terrain data model.
     * @param model Data model
     */
    void setModel(WorldGraphics * model);
    bool load(const QDir & dir, const QDomElement &root);
    bool load(const QDir & dir, const QDomElement &root, Map *map, TerrainBase *base);
    /**
     * @brief save Save current data model to hard drive;
     * @param xml
     * @return
     */
    bool save(QXmlStreamWriter &xml);
    /**
     * @brief create Inialize and create terrain on hard drive and world model.
     * @param dir
     * @param world
     * @return
     */
    bool create(const QDir &dir, WorldGraphics *world);
    /**
     * @brief importTexture Import texture form given path to terrain storage on hard drive.
     * @param id
     * @param path
     * @return
     */
    bool importTexture(unsigned short id, const std::string & path, const std::string &name=std::string());
    bool importNormalMap(unsigned short id, const std::string &path);

private:
    bool insert(Map *map);
    bool loadVoxel(TerrainBase *base, const std::string &file_name);
    bool saveVoxel(const std::string &file_name);

    void loadMaterials(const QDomElement &root);
    void loadMaterial(const QDomElement &root, int id);
    void decode(int node_size, int size, int x, int y, int z, signed char *voxel_data);


    struct TerrainHeader
    {
        int id;
        int node_size;
        int nodes_x,nodes_y,nodes_z;
    };

    struct TerrainHeader2
    {
        int id;
        int node_size;
        int nodes_x,nodes_y,nodes_z;
        int compress_type;
    };

    struct Patch
    {
        uint32_t x,y,z;
        uint32_t size;
    };

    struct Patch2
    {
        uint16_t x,y,z;
        uint16_t size;
        uint16_t type;
    };

    struct Patch3
    {
        uint16_t x,y,z;
        uint32_t size;
        uint16_t type;
    };

    TerrainGraphics * base;
    Map * map;
    std::string voxel_file;
};

#endif // TERRAINIO_H
