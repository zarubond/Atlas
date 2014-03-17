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
#include "projecthandler.h"

ProjectHandler::ProjectHandler()
{
    completed_percent=0;
}

bool ProjectHandler::create(Project * project,const QString & path, const QString &name,int width,int height,int depth)
{
    this->project=project;
    QDir dir(path);
    if(dir.exists())
    {
        project->root_dir=dir;
        project->name=name;

        QFileInfo info(dir.absoluteFilePath(name+".apro"));
        project->project_file=info.absoluteFilePath();

        project->map.setup(width,height,depth);

        if(project->terrain_io.create(dir,&project->map)
                && project->model_io.create(dir,&project->map)
                && project->grass_io.create(dir,&project->map))
        {
            return save(project,info.absoluteFilePath());
        }
    }
    return false;
}

bool ProjectHandler::load(Project *project, const QString &file_path)
{
    this->project=project;
    QFile file(file_path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"Unable to read project file "+file_path;
        return false;
    }

    QFileInfo fi(file_path);

    project->root_dir=fi.absoluteDir();
    project->project_file=fi.absoluteFilePath();
    project->name="Unknown";

    QString error_str;
    int error_line, error_column;

    QDomDocument doc("AtlasProject");

    if(!doc.setContent(&file,false,&error_str,&error_line,&error_column))
    {
        qDebug()<<"Error in xml file "+file_path+" : "+qPrintable(error_str);
    }

    QDomElement root=doc.documentElement();
    if(root.tagName() != "project")
    {
        return false;
    }

    QDomNode child=root.firstChild();
    QString tag_name;
    //header must be loaded first fo setup
    QDomNodeList header_list=root.elementsByTagName("header");
    if(header_list.count()==1)
    {
        QDomElement header=header_list.at(0).toElement();
        if(!parseHeader(header))
            return false;
    }
    else
    {
        qDebug()<<"Project header not found!"<<header_list.count();
        return false;
    }

    completed_percent+=10;
    emit loadProgress(completed_percent);

    QDir dir=project->root_dir;
    while(!child.isNull())
    {
        tag_name=child.toElement().tagName();

        if(tag_name=="terrain")
        {
            if(!project->terrain_io.load(dir,child.toElement(),&project->map))
                return false;
            completed_percent+=50;
            emit loadProgress(completed_percent);
        }
        else if(tag_name=="model")
        {
            if(!project->model_io.load(dir,child.toElement(),&project->map))
                return false;
            completed_percent+=10;
            emit loadProgress(completed_percent);
        }
        else if(tag_name=="grass")
        {
            if(!project->grass_io.load(dir,child.toElement(),&project->map))
                return false;
            completed_percent+=20;
            emit loadProgress(completed_percent);
        }
        child = child.nextSibling();
    }

    file.close();
    completed_percent=100;
    emit loadProgress(completed_percent);

    return true;
}
#if QT_VERSION > 0x050000
void ProjectHandler::loadThread(Project *project, const QString &file_path, QOpenGLContext *context,QOffscreenSurface * scr)
#else
void ProjectHandler::loadThread(Project *project, const QString &file_path, QOpenGLContext *context)
#endif
{
    completed_percent=0;
    this->project=project;
    this->file_path=file_path;
    this->context=context;
#if QT_VERSION > 0x050000
    this->scr=scr;
    context_thread=context->thread();
    context->moveToThread(this);
#endif
    start();
}

bool ProjectHandler::parseHeader(const QDomElement &root)
{
    QDomElement el;

    QDomNode child=root.firstChild();
    int width=0,height=0,depth=0;
    while(!child.isNull())
    {
        el=child.toElement();
        if(el.tagName()=="name")
            project->name=el.text();
        else if(el.tagName()=="width")
            width=el.text().toUInt();
        else if(el.tagName()=="height")
            height=el.text().toUInt();
        else if(el.tagName()=="depth")
            depth=el.text().toUInt();

        child=child.nextSibling();
    }

    if(width>0 && width<=project->map.maxWidth() && height>0 && height<=project->map.maxHeight() && depth>0 && depth<=project->map.maxDepth())
        project->map.setup(width,height,depth);
    else
        return false;
    return true;
}

bool ProjectHandler::save(Project *project, const QString &file_path)
{
    this->project=project;
    QFile file(file_path);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug()<<"Unable to save project";
        return false;
    }
    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("project");

    writeHeader(xml);
    if(!project->terrain_io.save(xml) || !project->grass_io.save(xml) || !project->model_io.save(xml))
    {
        xml.writeEndDocument();
        file.close();
        return false;
    }

    xml.writeEndDocument();
    file.close();

    std::cout<<"saved"<<std::endl;
    return true;
}

int ProjectHandler::completed()
{
    return completed_percent;
}

void ProjectHandler::run()
{
#if QT_VERSION < 0x050000
    context->makeCurrent();
#else
    context->makeCurrent(scr);
#endif

    load(project,file_path);
    context->doneCurrent();
#if QT_VERSION > 0x050000
    context->moveToThread(context_thread);
#endif
}

bool ProjectHandler::writeHeader(QXmlStreamWriter &xml)
{
    xml.writeStartElement("header");
    xml.writeTextElement("name",project->name);
    xml.writeTextElement("width",QString::number(project->map.width()));
    xml.writeTextElement("height",QString::number(project->map.height()));
    xml.writeTextElement("depth",QString::number(project->map.depth()));
    xml.writeEndElement();
    return true;
}

void ProjectHandler::exportProject(const QString & dir)
{
    QDir directory(dir);
    //settings
    QString file=directory.absoluteFilePath("map.txt");

    QSettings settings(file,QSettings::IniFormat);
    settings.clear();
    settings.beginGroup ( "Atlas map" );
    settings.setValue("name","map_name");

    //voxel data
    file=directory.absoluteFilePath("voxelmap.txt");
    settings.setValue("voxelmap","voxelmap.txt");

    //textures
    QImage img;
    QFileInfo info;
    for(int i=0;i<8;i++)
    {
        /*if(!terrain_textures[i].isEmpty())
        {
            info.setFile(terrain_textures[i]);
            if(img.load(root_dir.absoluteFilePath(terrain_textures[i])))
            {
                //HACK saveTexture(img,directory.absoluteFilePath(info.baseName()+".tga"));

                settings.setValue(QString("texture_")+QString::number(i),info.baseName()+".tga");
            }
        }*/
    }
}
