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

#include "oceanio.h"

OceanIO::OceanIO()
{
}

void OceanIO::setModel(WorldGraphics *model)
{
    this->base = &model->ocean;
    this->map = &model->map;
}

bool OceanIO::load(const QDir & dir, const QDomElement &root)
{
    base->setSize(map->width(), map->depth());

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
    //base->load();
    return true;
}

bool OceanIO::save(QXmlStreamWriter &xml)
{
    if(base->isEnabled())
    {
        xml.writeStartElement("water");
        xml.writeStartElement("ocean");
        QString color=base->color().toString().c_str();
        xml.writeTextElement("color",color);
        xml.writeTextElement("sealevel",QString::number(double(base->seaLevel())));

        xml.writeEndElement();
        xml.writeEndElement();
    }

    return true;
}

bool OceanIO::create(const QDir & dir, WorldGraphics * world)
{
    this->base=&world->ocean;
    base->setSize(world->map.width(), world->map.depth());
    base->setSeaLevel(0);
    base->setEnabled(false);
    return true;
}

void OceanIO::loadOcean(const QDomElement &root)
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
        else if(el.tagName()=="color" && !el.text().isEmpty())
        {
            ColorRGBA color;
            color.fromString(el.text().toUtf8().constData());
            base->setColor(color);
        }

        child=child.nextSibling();
    }

    //if(i==2)
    base->setEnabled(true);
}
