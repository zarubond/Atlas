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
#include "session.h"

Session::Session()
{
    project=NULL;
}

Session::~Session()
{
    delete project;
}

void Session::setup(Canvas *space, Editor *editor)
{
    this->space=space;
    this->editor=editor;
#ifdef Q_WS_ANDROID
   // openProject("/storage/sdcard1/android/android.apro");
#else/*
    QString project_path=settings.value("last_opened_project").toString();

    qDebug()<<project_path;
    if(!project_path.isEmpty())
    {

        openProject(project_path);
    }*/
#endif
    //settings.setValue("assets","/mnt/disk/Projects/Atlas-v2/assets/assets.aas");
}

bool Session::openProject(const QString &file_name)
{
    QFileInfo file(file_name);

    if(file.exists())
    {
        project=new Project;

        space->drawer.terrain.load(&project->terrain);
        space->drawer.model.load(&project->model);
        space->drawer.grass.load(&project->grass);

        if(handler.load(project,file.filePath()))
        {
            editor->setProject(project);

            QStringList files = settings.value("recentFileList").toStringList();
            files.removeAll(project->project_file);
            files.prepend(project->project_file);
            while(files.size()>5)
                files.removeLast();

            settings.setValue("recentFileList", files);

            space->setProject(project);
            return true;
        }
        else
        {
            delete project;
            project=NULL;
            space->setProject(NULL);
            qDebug()<<"Unable to load project "+file_name;
        }
    }
    return false;
}

bool Session::createProject(const QString & project_path,const QString & name,int width,int height,int depth)
{
    QDir dir(project_path);
    if(dir.mkpath(project_path))
    {
        editor->unsetProject();
        delete project;
        project=new Project;

        space->drawer.terrain.load(&project->terrain);
        space->drawer.model.load(&project->model);
        space->drawer.grass.load(&project->grass);

        if(handler.create(project,project_path,name,width,height,depth))
        {
            space->setProject(project);
            editor->setProject(project);

            QStringList files = settings.value("recentFileList").toStringList();
            files.removeAll(project->project_file);
            files.prepend(project->project_file);
            while(files.size()>5)
                files.removeLast();

            settings.setValue("recentFileList", files);
            return true;
        }
        else
            std::cerr<<"Unable to create project!!!"<<std::endl;
    }
    return false;
}

void Session::saveProject()
{
    if(project!=NULL)
    {
        handler.save(project,project->project_file);
    }
}

bool Session::closeProject()
{
    if(project!=NULL)
    {
        editor->unsetProject();
        delete project;
        project= NULL;
        return true;
    }
    return false;
}

bool Session::isLoaded()
{
    if(project!=NULL)
        return true;
    return false;
}
QSettings *Session::getSettings()
{
    return &settings;
}
