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
#ifndef PROJECTHANDLER_H
#define PROJECTHANDLER_H

#include <QXmlStreamWriter>
#include <QSettings>
#include <QThread>
#if QT_VERSION > 0x050000
    #include <QOffscreenSurface>
#endif

#include "../world/map.h"
#include "project.h"
#include "terrainio.h"
#include "modelio.h"
#include "grassio.h"

class ProjectHandler: public QThread
{
    Q_OBJECT
public:
    explicit ProjectHandler();
    bool create(Project *project, const QString &path, const QString &name, int width, int height, int depth);
    bool load(Project * project, const QString &file_path);
    bool save(Project * project, const QString &file_path);
    int completed();
#if QT_VERSION > 0x050000
    void loadThread(Project *project, const QString &file_path, QOpenGLContext *context, QOffscreenSurface *scr);
#else
    void loadThread(Project *project, const QString &file_path, QOpenGLContext *context);
#endif
signals:
    void loadProgress(int completed);
private:
    void run();
    //to be
    void exportProject(const QString &dir);

    bool writeHeader(QXmlStreamWriter &xml);
    bool parseHeader(const QDomElement &root);

    int completed_percent;
    Project * project;
    QString file_path;
    QOpenGLContext * context;
    QThread * context_thread;
#if QT_VERSION > 0x050000
    QOffscreenSurface * scr;
#endif

};

#endif // PROJECTHANDLER_H
