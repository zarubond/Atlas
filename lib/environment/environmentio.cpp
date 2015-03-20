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

#include "environmentio.h"

EnvironmentIO::EnvironmentIO()
{
}

bool EnvironmentIO::load(Environment * environment, const QDomElement &root)
{
    parseEnvironment(environment, root);
    return true;
}

bool EnvironmentIO::save(Environment * environment, QXmlStreamWriter &xml)
{
    xml.writeStartElement("environment");

    xml.writeStartElement("sun");
    Vector3f t=environment->ambient.getColor();

    QString ambient=t.toString().c_str();
    xml.writeTextElement("ambient",ambient);

    t=environment->sun.getSpecular();
    QString specular=t.toString().c_str();
    xml.writeTextElement("specular",specular);

    t=environment->sun.getDiffuse();
    QString diffuse=t.toString().c_str();
    xml.writeTextElement("diffuse",diffuse);

    t=environment->sun.getDirection();
    QString direction=t.toString().c_str();
    xml.writeTextElement("direction",direction);

    xml.writeTextElement("shininess",QString::number(environment->sun.getShininess()));
    xml.writeEndElement();

    xml.writeStartElement("fog");
    if(environment->fog.isEnabled())
        xml.writeTextElement("enabled","true");
    else
        xml.writeTextElement("enabled","false");
    xml.writeTextElement("density",QString::number(environment->fog.getDensity()));
    QString cfog=environment->fog.getColor().toString().c_str();
    xml.writeTextElement("color",cfog);
    xml.writeEndElement();

    xml.writeEndElement();
    return true;
}


void EnvironmentIO::parseEnvironment(Environment * environment, const QDomElement &root)
{
    QDomElement el;
    QDomNode child=root.firstChild();

    while(!child.isNull())
    {
        el=child.toElement();
        if(el.tagName() == "sun")     parseSun(environment, el.toElement());
        else if(el.tagName() == "fog")parseFog(environment, el.toElement());

        child=child.nextSibling();
    }
}

void EnvironmentIO::parseSun(Environment * environment, const QDomElement &root)
{
    QDomElement el;
    QDomNode child=root.firstChild();
    ColorRGB vec;
    std::string str;
    while(!child.isNull())
    {
        el=child.toElement();
        if(el.tagName() == "ambient")
        {
            str = el.text().toUtf8().constData();
            vec.fromString(str.c_str());
            environment->ambient.setColor(vec);
        }
        else if(el.tagName() == "direction")
        {
            str= el.text().toUtf8().constData();
            vec.fromString(str.c_str());
            environment->sun.setDirection(vec);
        }
        else if(el.tagName() == "diffuse")
        {
            str = el.text().toUtf8().constData();
            vec.fromString(str.c_str());
            environment->sun.setDiffuse(vec);
        }
        else if(el.tagName() == "specular")
        {
            str= el.text().toUtf8().constData();
            vec.fromString(str.c_str());
            environment->sun.setSpecular(vec);
        }
        else if(el.tagName() == "shininess")
        {
            environment->sun.setShininess(el.text().toFloat());
        }

        child=child.nextSibling();
    }
}

void EnvironmentIO::parseFog(Environment * environment, const QDomElement &root)
{
    QDomElement el;
    QDomNode child=root.firstChild();

    Vector3f vec;
    std::string str;
    while(!child.isNull())
    {
        el=child.toElement();
        if(el.tagName() == "density")
        {
            environment->fog.setDensity(el.text().toFloat());
        }
        else if(el.tagName() == "color")
        {
            str= el.text().toUtf8().constData();
            vec.fromString(str.c_str());
            environment->fog.setColor(vec);
        }
        else if(el.tagName() == "enabled")
        {
            if(el.text()=="true")
                environment->fog.setEnable(true);
            else
                environment->fog.setEnable(false);
        }

        child=child.nextSibling();
    }
}
