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

#include "projectio.h"

#include <QOpenGLContext>

ProjectIO::ProjectIO()
{
    context=NULL;
    surface=NULL;
    connect(&io,SIGNAL(loadProgress(QString,int)),this,SIGNAL(loadProgress(QString,int)));
}

ProjectIO::~ProjectIO()
{
   // thread.join();
    delete context;
    delete surface;
}

void ProjectIO::init(QOpenGLContext *shared_context)
{
    //must be in main thread for Windows
    if(surface==NULL)
    {
        surface=new QOffscreenSurface;
        surface->create();
        surface->moveToThread(this);
    }

    if(context==NULL)
    {
        context=new QOpenGLContext;
        context->setShareContext(shared_context);
        context->create();
        context->moveToThread(this);
    }
}

bool ProjectIO::create(WorldGraphics *world, const QString &path, const QString &name, const QString &author, int width, int height, int depth)
{
    bool success=io.create(world, path.toStdString(), name.toStdString(), author.toStdString(), width, height, depth);
    emit completed(success);
    return success;
}

bool ProjectIO::save(WorldGraphics *world, const QString &file_path)
{
    return io.save(world,file_path.toStdString());
}

int ProjectIO::completed()
{
    return io.completed();
}

bool ProjectIO::load(WorldGraphics *world, const QString &file_path)
{
    bool success = io.load(world,file_path.toStdString());
    emit completed(success);
    return success;
}

bool ProjectIO::load(WorldBase *world, const QString &file_path)
{
    bool success= io.load(world,file_path.toStdString());
    //emit onCompleted(succeess);
    return success;
}

void ProjectIO::loadThread(WorldGraphics *world, const QString &file_path, QOpenGLContext *shared_context)
{
    this->world=world;
    this->file_path=file_path;
    this->shared_context=shared_context;

    init(shared_context);

    start();
}

WorldIO *ProjectIO::worldIO()
{
    return &io;
}

void ProjectIO::run()
{
    context->makeCurrent(surface);

    bool success=io.load(world,file_path.toStdString());
    context->doneCurrent();

    emit completed(success);
}
