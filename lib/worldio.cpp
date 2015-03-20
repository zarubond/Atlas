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

#include "worldio.h"

WorldIO::WorldIO()
{
    completed_percent=0;
    world=NULL;
    module_cnt=0;

    modules[module_cnt++]=&terrain_io;
    modules[module_cnt++]=&model_io;
    modules[module_cnt++]=&ocean_io;
    //modules[module_cnt++]=&grass_io;
    for(int i=0;i<module_cnt;i++)
        modules[i]->setProgressListener(this);
}

WorldIO::~WorldIO()
{
}

bool WorldIO::create(WorldGraphics * world,const std::string &path, const std::string &name,const std::string &author,int width,int height,int depth)
{
    this->world=world;
    QDir dir(path.c_str());
    dir.mkpath(path.c_str());
    if(dir.exists())
    {
        world->name=name;
        world->author=author;
        root_dir=path.c_str();

        world->editor.init();

        QFileInfo info(dir.absoluteFilePath(QString(name.c_str())+".apro"));
        world->file=info.absoluteFilePath().toStdString();

        world->map.setup(width,height,depth);
        bool ok=true;

        for(int i=0; i<module_cnt; i++)
        {
            if(ok)
                ok=modules[i]->create(dir,world);
            else
                break;
        }

        if(ok)
        {
            return save(world,info.absoluteFilePath().toStdString());
        }
    }
    return false;
}

bool WorldIO::load(WorldGraphics *world, const std::string &file_path)
{
    completed_percent=0;
    emit loadProgress("Loading...",completed_percent);
    this->world=world;
    world->name="Unknown";
    world->file=file_path;

    QFile file(file_path.c_str());
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        cerr<<"Unable to read project file "<<file_path<<endl;
        file.close();
        return false;
    }

    QFileInfo fi(file_path.c_str());

    QString error_str;
    int error_line, error_column;

    QDomDocument doc("AtlasProject");

    if(!doc.setContent(&file,false,&error_str,&error_line,&error_column))
    {
        cerr<<"Error in xml file "<<file_path<<" : "<<qPrintable(error_str)<<endl;
    }

    QDomElement root=doc.documentElement();
    if(root.tagName() != "project")
    {
        return false;
    }

    world->editor.init();

    QDomNode child=root.firstChild();
    //header must be loaded first for setup
    QDomNodeList header_list=root.elementsByTagName("header");
    if(header_list.count()==1)
    {
        QDomElement header=header_list.at(0).toElement();
        if(!parseHeader(header,&world->map))
            return false;
    }
    else
    {
        cerr<<"Project header not found!"<<header_list.count()<<endl;
        return false;
    }

    completed_percent+=10;
    emit loadProgress("Loading world...",completed_percent);

    loadParts((WorldGraphics*)world,fi.absoluteDir(),child);

    file.close();
    completed_percent=100;
    emit loadProgress("Loading done.",completed_percent);

    cerr<<"Loading done"<<endl;
    return true;
}

bool WorldIO::load(WorldBase *world, const std::string &file_path)
{
    world->name="Unknown";

    QFile file(file_path.c_str());
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        cerr<<"Unable to read project file "<<file_path<<endl;
        return false;
    }

    QFileInfo fi(file_path.c_str());

    QString error_str;
    int error_line, error_column;

    QDomDocument doc("AtlasProject");

    if(!doc.setContent(&file,false,&error_str,&error_line,&error_column))
    {
        cerr<<"Error in xml file "<<file_path<<" : "<<qPrintable(error_str)<<endl;
    }

    QDomElement root=doc.documentElement();
    if(root.tagName() != "project")
    {
        return false;
    }

    QDomNode child=root.firstChild();
    //header must be loaded first for setup
    QDomNodeList header_list=root.elementsByTagName("header");
    if(header_list.count()==1)
    {
        QDomElement header=header_list.at(0).toElement();
        if(!parseHeader(header,&world->map))
            return false;
    }
    else
    {
        cerr<<"Project header not found!"<<header_list.count()<<endl;
        return false;
    }

    completed_percent+=10;
    loadParts(world,fi.absoluteDir(),child);
    root_dir=fi.absoluteDir();
    file.close();
    completed_percent=100;

    return true;
}

bool WorldIO::importSnapshot(const std::string &file_path)
{
    QFileInfo info(file_path.c_str());
    QDir t_dir=root_dir;
    QString name="snapshot.png";
    QString new_path=t_dir.absoluteFilePath(name);
    Image img(file_path);
    if(info.exists() && img.save(new_path.toStdString(),"PNG"))
    {
        world->snapshot="snapshot.png";
        return true;
    }
    else
    {
        cerr<<"Unable to save snapshot image"<<new_path<<endl;
    }
    return false;
}

const std::string &WorldIO::worldFile()
{
    return world_file;
}

bool WorldIO::parseHeader(const QDomElement &root,Map * map)
{
    QDomElement el;

    QDomNode child=root.firstChild();
    int width=0,height=0,depth=0;
    while(!child.isNull())
    {
        el=child.toElement();
        if(el.tagName()=="name")            world->name=el.text().toUtf8().constData();
        else if(el.tagName()=="width")      width=el.text().toUInt();
        else if(el.tagName()=="height")     height=el.text().toUInt();
        else if(el.tagName()=="depth")      depth=el.text().toUInt();
        else if(el.tagName()=="info")       world->info=el.text().toUtf8().constData();
        else if(el.tagName()=="snapshot")   world->snapshot=el.text().toUtf8().constData();
        else if(el.tagName()=="author")     world->author=el.text().toUtf8().constData();
        else cerr<<"Unknown tag name: "<<el.tagName().toUtf8().constData()<<endl;

        child=child.nextSibling();
    }

    if(width>0 && width<=map->maxWidth()
            && height>0 && height<=map->maxHeight()
            && depth>0 && depth<=map->maxDepth())
        map->setup(width,height,depth);
    else
        return false;
    return true;
}

bool WorldIO::save(WorldGraphics *world, const std::string &file_path)
{
    this->world=world;

    QByteArray domDoc;
    QXmlStreamWriter xml(&domDoc);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("project");

    writeHeader(xml);
    environment.save(&world->environment,xml);

    bool ok=true;

    for(int i=0;i<module_cnt;i++)
    {
        ok=modules[i]->save(xml);
        if(!ok) break;
    }

    xml.writeEndDocument();

    if(!ok)
    {
        cerr<<"Unable to save one of modules"<<endl;
        return false;
    }

    QFile file(file_path.c_str());
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        cerr<<"Unable to open project file "<<file_path<<"!"<<endl;
        return false;
    }

    file.write(domDoc);
    file.close();
    return true;
}

int WorldIO::completed()
{
    return completed_percent;
}

bool WorldIO::writeHeader(QXmlStreamWriter &xml)
{
    xml.writeStartElement("header");
    xml.writeTextElement("name",world->name.c_str());
    xml.writeTextElement("author",world->author.c_str());
    xml.writeTextElement("snapshot",world->snapshot.c_str());
    xml.writeTextElement("info",world->info.c_str());
    xml.writeTextElement("width",QString::number(world->map.width()));
    xml.writeTextElement("height",QString::number(world->map.height()));
    xml.writeTextElement("depth",QString::number(world->map.depth()));
    xml.writeEndElement();

    return true;
}

void WorldIO::exportProject(const QString & dir)
{/*
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
        }*
    }*/
}

void WorldIO::progress(ModuleIO *part, std::string message, float p)
{
    emit loadProgress(message.c_str(),p);
}

bool WorldIO::loadParts(WorldGraphics * world,const QDir & dir, const QDomNode & node)
{
    for(int i=0;i<module_cnt;i++)
    {
        modules[i]->setModel(world);
    }

    QString tag_name;
    QDomNode child=node;
    while(!child.isNull())
    {
        tag_name=child.toElement().tagName();
        if(tag_name=="environment")
        {
            environment.load(&world->environment, child.toElement());
        }
        else if(tag_name=="terrain")
        {
            if(!terrain_io.load(dir,child.toElement()))
                return false;
            completed_percent+=50;
            emit loadProgress("Loading terrain done...",completed_percent);
        }
        else if(tag_name=="model")
        {
            if(!model_io.load(dir,child.toElement()))
                return false;
            completed_percent+=10;
            emit loadProgress("Loading models done...",completed_percent);
        }
        else if(tag_name=="grass")
        {
           //if(!grass_io.load(dir,child.toElement()))
           //   return false;
           //completed_percent+=20;
           //emit loadProgress("Loading grass done...",completed_percent);
        }
        else if(tag_name=="water")
        {
            if(!ocean_io.load(dir,child.toElement()))
                return false;
            completed_percent+=20;
            emit loadProgress("Loading grass done...",completed_percent);
        }
        child = child.nextSibling();
    }


    return true;
}

bool WorldIO::loadParts(WorldBase * world,const QDir & dir, const QDomNode & node)
{
    QString tag_name;
    QDomNode child=node;
    while(!child.isNull())
    {
        tag_name=child.toElement().tagName();

        if(tag_name=="terrain")
        {
            if(!terrain_io.load(dir,child.toElement(),&world->map,&world->terrain))
                return false;
            completed_percent+=50;
            //emit loadProgress(completed_percent);
        }/*
        else if(tag_name=="model")
        {
            if(!model_io.load(dir,child.toElement(),&world->map,&world->model))
                return false;
            completed_percent+=10;
            emit loadProgress(completed_percent);
        }
        else if(tag_name=="grass")
        {
            if(!grass_io.load(dir,child.toElement(),&world->map,&world->grass))
                return false;
            completed_percent+=20;
            emit loadProgress(completed_percent);
        }*/
        child = child.nextSibling();
    }

    return true;
}
