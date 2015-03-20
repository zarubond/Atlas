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
#ifndef PROJECTIO_H
#define PROJECTIO_H

#include <QXmlStreamWriter>
#include <QThread>
#include <QOffscreenSurface>

#include "lib/worldio.h"
#include "lib/worldbase.h"
#include "lib/worldgraphics.h"


class ProjectIO : public QThread
{
    Q_OBJECT
public:
    ProjectIO();
    ~ProjectIO();

    bool create(WorldGraphics *world, const QString &path, const QString &name, const QString &author, int width, int height, int depth);

    bool save(WorldGraphics *world, const QString &file_path);
    int completed();

    bool load(WorldGraphics *world, const QString &file_path);
    //This is temporal HACK
    bool load(WorldBase * world,const QString & file_path);
    void loadThread(WorldGraphics *world, const QString &file_path, QOpenGLContext *shared_context);
    WorldIO *worldIO();
    void setProgressListener();
signals:
    void loadProgress(QString caption,int completed);
    void completed(bool success);
private:
    void init(QOpenGLContext *shared_context);
    void run();

    QString file_path;

    WorldIO io;
    WorldGraphics * world;
    QOpenGLContext * context;
    QOpenGLContext * shared_context;
    QOffscreenSurface * surface;

};

#endif // PROJECTIO_H
