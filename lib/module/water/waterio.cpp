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

#include "waterio.h"

WaterIO::WaterIO()
{
}

void WaterIO::setModel(WorldGraphics *model)
{
    this->base = &model->ocean;
    this->map = &model->map;
}

bool WaterIO::load(const QDir & dir, const QDomElement &root)
{
    part_dir=dir;

    QDomElement el;
    QDomNode child=root.firstChild();
    while(!child.isNull())
    {
        el=child.toElement();
        if(el.tagName()=="ocean" && !el.text().isEmpty())
        {
            loadOcean(el);
        }

        child=child.nextSibling();
    }
    return true;
}

bool WaterIO::save(QXmlStreamWriter &xml)
{
    if(base->isEnabled())
    {
        xml.writeStartElement("water");
        xml.writeStartElement("ocean");

        xml.writeTextElement("texture",part_dir.relativeFilePath(water_texture));
        xml.writeTextElement("sealevel",QString::number(double(base->seaLevel())));

        xml.writeEndElement();
        xml.writeEndElement();
    }

    return true;
}

bool WaterIO::create(const QDir &dir, WorldGraphics *world)
{
    this->base=&world->ocean;
    return true;
}

void WaterIO::loadOcean(const QDomElement &root)
{
    QDomElement el;
    QDomNode child=root.firstChild();
    int i=0;
    while(!child.isNull())
    {
        el=child.toElement();
        if(el.tagName()=="sealevel" && !el.text().isEmpty())
        {
            base->setSeaLevel(el.text().toFloat());
            i++;
        }
        else if(el.tagName()=="texture" && !el.text().isEmpty())
        {
            water_texture=part_dir.absoluteFilePath(el.text());
            base->normal_map.loadFile(part_dir.absoluteFilePath(el.text()).toUtf8().constData(),GL_LINEAR_MIPMAP_LINEAR,GL_REPEAT);
            i++;
        }

        child=child.nextSibling();
    }

    if(i==2)
        base->setEnabled(true);
}
