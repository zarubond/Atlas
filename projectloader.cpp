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

#include "projectloader.h"

ProjectLoader::ProjectLoader(QQuickItem *parent) :
    QQuickItem(parent)
{
    connect(&handler,SIGNAL(loadProgress(QString,int)),this,SLOT(updateProgress(QString,int)));
    m_loadingProgress=0;
    m_loadingMessage="Loader";
}

void ProjectLoader::openProject(const QString &file_path, QOpenGLContext *context)
{
    QFileInfo file(file_path);
    if(file.exists())
    {
        project=new Project;
        project->io=handler.worldIO();
        bool thread=true;
        if(thread)
        {
            connect(&handler,SIGNAL(completed(bool)),this,SLOT(loadingDone(bool)));
            handler.loadThread(&project->world,file.filePath(),context);
        }
        else if(handler.load(&project->world,file.filePath()))
        {
            loadingDone(true);
        }
        else
        {
            loadingDone(false);
        }
    }
}

void ProjectLoader::createProject(const QString &path, const QString &name, const QString &author, int width, int height, int depth)
{
    project=new Project;
    project->io=handler.worldIO();
    handler.create(&project->world,path,name,author,width,height,depth);
    addRecentProject(project->world.file.c_str());
    loadingDone(true);
}

void ProjectLoader::saveProject()
{
    if(project!=NULL)
    {
        handler.save(&project->world, project->world.file.c_str());
    }
}

void ProjectLoader::saveProjectAs(const QUrl &save_folder)
{
    if(project!=NULL)
    {
        //handler.save(&project->world, save_folder.toLocalFile().toStdString());
    }
}

float ProjectLoader::loadingProgress() const
{
    return m_loadingProgress;
}

QString ProjectLoader::loadingMessage() const
{
    return m_loadingMessage;
}

void ProjectLoader::setLoadingProgress(float arg)
{
    if (m_loadingProgress != arg) {
        m_loadingProgress = arg;
        emit loadingProgressChanged(arg);
    }
}

void ProjectLoader::setLoadingMessage(QString &arg)
{
    if (m_loadingMessage != arg) {
        m_loadingMessage = arg;
        emit loadingMessageChanged(arg);
    }
}

void ProjectLoader::updateProgress(QString caption,int value)
{
    float val=value;
    val/=100.0;
    setLoadingMessage(caption);
    setLoadingProgress(val);
}

void ProjectLoader::loadingDone(bool status)
{
    if(status)
    {
        addRecentProject(project->world.file.c_str());
        emit projectLoaded(project);
    }
    else
    {
        delete project;
        project=NULL;
        cerr<<"Unable to load project "<<endl;

        emit projectLoaded(NULL);
    }
}

void ProjectLoader::addRecentProject(const QString & project_file)
{
    QStringList files = GlobalSettings::instance()->recentProjects();
    files.removeAll(project_file);
    files.prepend(project_file);
    while(files.size()>5)
        files.removeLast();

    GlobalSettings::instance()->setRecentProjects(files);
}

