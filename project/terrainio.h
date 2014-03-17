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
#ifndef TERRAINIO_H
#define TERRAINIO_H

#include <QElapsedTimer>

#include "./partio.h"

class TerrainIO: public QObject,public PartIO
{
    Q_OBJECT
public:
    TerrainIO(TerrainGraphics *base);
    bool load(const QDir & dir,const QDomElement &root,Map *map);
    bool save(QXmlStreamWriter &xml);
    bool create(const QDir &dir, Map *map);
    bool importTexture(unsigned short id, const QString & path);
    bool importNormalMap(unsigned short id, const QString & path);
    QString *getTexturePath(unsigned short id);
    QString *getNormalMapPath(unsigned short id);

    bool setTexture(int id, const char *file_name);
signals:
    void newMaterial(int id,QImage & img);
private:
    bool insert(Map *map);

    QString texture_file[8],voxel_file;
    QString normalmap_file[8];
    bool loadVoxel(const QString &file_name);
    bool saveVoxel(const QString &file_name);

    bool loadTexture(const QString &file_name, int id);
    bool loadNormalMap(const QString &file_name, int id);


    struct TerrainHeader
    {
        int id;
        int node_size;
        int nodes_x,nodes_y,nodes_z;
    };

    struct Voxel
    {
        char value;
        char texture;
    };

    struct Patch
    {
        uint32_t x,y,z;
        uint32_t size;
    };

    TerrainGraphics & base;
};

#endif // TERRAINIO_H
