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
#include "grassio.h"

GrassIO::GrassIO(GrassGraphics *base):base(*base)
{
}

bool GrassIO::load(const QDir & dir,const QDomElement &root,Map *map)
{
    part_dir=dir;
    QDomElement el;
    QDomNode child=root.firstChild();
    while(!child.isNull())
    {
        el=child.toElement();
        if(el.tagName()=="file" && !el.text().isEmpty())
        {
            FILE *file = fopen(part_dir.absoluteFilePath(el.text()).toUtf8().constData(), "r");
            if(file==NULL)
            {
                qDebug()<<"Unable to load grass file: "+part_dir.absolutePath();
                return false;
            }
            grass_file=part_dir.absoluteFilePath(el.text()).toUtf8().constData();

            char line[200];
            //disable localization for dot problem 0.0 vs 0,0
            setlocale(LC_NUMERIC, "C");
            Vertex3f tmp;
            while(fgets(line,200,file)!=NULL)
            {
                sscanf(line,"%f %f %f",&tmp[0],&tmp[1],&tmp[2]);
                base.addTuft(tmp);
            }
            setlocale(LC_ALL, "");
            fclose(file);
        }
        else if(el.tagName()=="texture" && !el.text().isEmpty())
        {
            grass_texture[0]=part_dir.filePath(el.text());
            Texture * texture=new Texture;
            texture->load(grass_texture[0].toUtf8().constData());
            base.setTexture(texture);
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
        qDebug()<<"Unable to grass data for save"<<grass_file;
        return false;
    }
    setlocale(LC_NUMERIC, "C");
    const Vertex3f * tufts=base.tuftList();
    for(int i=0;i<base.tuftCount();i++)
        fprintf(file,"%f %f %f \n",tufts[i][0],tufts[i][1],tufts[i][2]);

    fclose(file);
    setlocale(LC_ALL, "");

    xml.writeStartElement("grass");
    xml.writeTextElement("file",part_dir.relativeFilePath(grass_file));
    xml.writeTextElement("texture",part_dir.relativeFilePath(grass_texture[0]));
    xml.writeEndElement();

    return true;
}

bool GrassIO::create(const QDir &dir, Map *map)
{
    part_dir=dir;
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
    QImage img(path);
    if(info.exists() && img.save(new_path,"PNG"))
    {
        grass_texture[id]=new_path;
        //base.atlas.loadSub(new_path.toUtf8().constData(),id);

        Texture * texture=new Texture;
        if(texture->load(grass_texture[id].toUtf8().constData()))
            base.setTexture(texture);
       // emit newMaterial(id+1,img);
    }
    else
    {
        qDebug()<<"Unable to save image"<<new_path;
    }
    return false;
}

QString *GrassIO::getTexturePath(unsigned short id)
{
    return &grass_texture[id];
}
