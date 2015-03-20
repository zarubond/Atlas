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
#ifndef PROJECTHANDLER_H
#define PROJECTHANDLER_H

#include <QXmlStreamWriter>

#include "module/moduleio.h"
#include "module/terrain/terrainio.h"
#include "module/model/modelio.h"
#include "module/grass/grassio.h"
#include "module/ocean/oceanio.h"
#include "worldbase.h"
#include "worldgraphics.h"
#include "environment/environmentio.h"

class WorldIO: public QObject, public ModuleProgressListener
{
    Q_OBJECT
public:
    explicit WorldIO();
    ~WorldIO();
    /**
     * @brief create Create new project.
     * @param world
     * @param path
     * @param name
     * @param author
     * @param width
     * @param height
     * @param depth
     * @return
     */
    bool create(WorldGraphics *world, const std::string &path, const std::string &name, const std::string &author, int width, int height, int depth);
    /**
     * @brief save Save current project
     * @param world
     * @param file_path
     * @return
     */
    bool save(WorldGraphics *world, const std::string &file_path);
    /**
     * @brief completed Number of competer percent.
     * @return Value between 0 and 100 %
     */
    int completed();
    /**
     * @brief load Load project from file.
     * @param world
     * @param file_path
     * @return
     */
    bool load(WorldGraphics *world, const std::string &file_path);
    //This is temporal HACK
    bool load(WorldBase * world,const std::string & file_path);
    /**
     * @brief importSnapshot Import project snapshot.
     * @param file_path
     * @return success
     */
    bool importSnapshot(const std::string & file_path);

    const std::string &worldFile();

    TerrainIO terrain_io;
    ModelIO model_io;
   // GrassIO grass_io;
    OceanIO ocean_io;
signals:
    void loadProgress(QString caption,int completed);
private:
    //to be
    void exportProject(const QString &dir);
    void progress(ModuleIO * part,std::string message, float p);

    bool loadParts(WorldBase *world, const QDir &dir, const QDomNode &node);
    bool loadParts(WorldGraphics *world, const QDir &dir, const QDomNode &node);
    bool writeHeader(QXmlStreamWriter &xml);
    bool parseHeader(const QDomElement &root, Map *map);

    int completed_percent;
    QDir root_dir;
    WorldGraphics * world;
    ModuleIO * modules[10];//TBD
    EnvironmentIO environment;
    int module_cnt;
    std::string world_file;
};

#endif // PROJECTHANDLER_H
