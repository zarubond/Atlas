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

#include "grassio.h"

GrassIO::GrassIO()
{
}

void GrassIO::setModel(WorldGraphics *model)
{
    this->map = &model->map;
    this->base = &model->grass;
}

bool GrassIO::load(const QDir & dir, const QDomElement &root)
{
    part_dir=dir;
    QDomElement el;
    QDomNode child=root.firstChild();

    base->setMap(map);
    base->build(ceil(map->width()/map->nodeSize()),
                               ceil(map->height()/map->nodeSize()),
                               ceil(map->depth()/map->nodeSize()),map->nodeSize());

    while(!child.isNull())
    {
        el=child.toElement();
        if(el.tagName()=="file" && !el.text().isEmpty())
        {
            FILE *file = fopen(part_dir.absoluteFilePath(el.text()).toUtf8().constData(), "r");
            if(file==NULL)
            {
                cerr<<"Unable to load grass file: "<<part_dir.absolutePath()<<endl;
                return false;
            }
            grass_file=part_dir.absoluteFilePath(el.text()).toUtf8().constData();

            char line[200];
            //disable localization for dot problem 0.0 vs 0,0
            setlocale(LC_NUMERIC, "C");
            Vector3f tmp;
            while(fgets(line,200,file)!=NULL)
            {
                sscanf(line,"%f %f %f",&tmp[0],&tmp[1],&tmp[2]);
                base->addTuft(tmp);
            }
            setlocale(LC_ALL, "");
            fclose(file);
        }
        else if(el.tagName()=="texture" && !el.text().isEmpty())
        {
            grass_texture[0]=part_dir.filePath(el.text());
            Texture * texture=new Texture;
            texture->loadFile(grass_texture[0].toUtf8().constData());
            base->setTexture(texture);
        }

        child=child.nextSibling();
    }

    return true;
}

bool GrassIO::save(QXmlStreamWriter &xml)
{
    QFileInfo info(grass_file);
    if(!info.exists())
    {
        part_dir.mkpath(info.absolutePath());
    }

    FILE *file = fopen(grass_file.toUtf8().constData(), "w");
    if(file==NULL)
    {
        cerr<<"Unable to grass data for save"<<grass_file<<endl;
        return false;
    }
    setlocale(LC_NUMERIC, "C");
    GrassRegion * region;

    for(int x=0; x < base->nodesX(); x++)
        for(int y=0; y < base->nodesY(); y++)
            for(int z=0; z < base->nodesZ(); z++)
            {
                region=base->getRegion(x,y,z);
                if(region!=NULL)
                    for(int i=0;i<region->tuftCount();i++)
                        fprintf(file,"%f %f %f \n",region->getTuft(i)[0],region->getTuft(i)[1],region->getTuft(i)[2]);
            }

    fclose(file);
    setlocale(LC_ALL, "");

    xml.writeStartElement("grass");
    xml.writeTextElement("file",part_dir.relativeFilePath(grass_file));
    xml.writeTextElement("texture",part_dir.relativeFilePath(grass_texture[0]));
    xml.writeEndElement();

    return true;
}

bool GrassIO::create(const QDir &dir, WorldGraphics *world)
{
    part_dir=dir;
    base=&world->grass;
    grass_file=part_dir.absoluteFilePath("grass/grass.bin").toUtf8().constData();
    return true;
}

bool GrassIO::loadTuft()
{
    return false;
}

int GrassIO::textureNum()
{
    return 1;
}

bool GrassIO::importTexture(unsigned short id, const QString &path)
{
    if(id!=0) return false;//HACK

    QFileInfo info(path);
    QDir t_dir=part_dir;
    t_dir.cd("grass");
    QString name="grasstexture";
    name+=QString::number(id+1);
    name+=".png";
    QString new_path=t_dir.absoluteFilePath(name);
    Image img(path.toUtf8().constData());
    if(info.exists() && img.save(new_path.toUtf8().constData(),"PNG"))
    {
        grass_texture[id]=new_path;
        //base.atlas.loadSub(new_path.toUtf8().constData(),id);

        Texture * texture=new Texture;
        if(texture->loadFile(grass_texture[id].toUtf8().constData()))
            base->setTexture(texture);
       // emit newMaterial(id+1,img);
    }
    else
    {
        cerr<<"Unable to save image"<<new_path<<endl;
    }
    return false;
}

QString *GrassIO::getTexturePath(unsigned short id)
{
    return &grass_texture[id];
}
