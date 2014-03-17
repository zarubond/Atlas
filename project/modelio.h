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
#ifndef MODELIO_H
#define MODELIO_H

#include <QXmlStreamWriter>
#include <QStandardItemModel>
#include "./partio.h"

class ModelIO: public PartIO
{
public:
    ModelIO(ModelGraphics *base);
    bool load(const QDir & dir,const QDomElement &root,Map *map);
    bool save(QXmlStreamWriter &xml);
    bool create(const QDir &dir, Map *map);
    ModelMesh *importMesh(const QString & file_name);
    void removeMesh(ModelMesh *mesh);
private:

    std::string model_dir;
    QString model_file;
    Map * map;

    struct ModelHeader
    {
        char name[64];
        float position[3];
        float scale[3];
        float rotation[4];
        int mesh_id;
    };

    ModelGraphics &base;
};

#endif // MODELIO_H
