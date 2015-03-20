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

#include "assets.h"

Assets::Assets()
{
}

bool Assets::load(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        cerr<<"Asset file not found: "<<path<<endl;
        return false;
    }
    QFileInfo info(path);
    QDir dir(info.absoluteDir());

    QXmlStreamReader xml(&file);
    while(!xml.atEnd() && !xml.hasError())
    {
        QXmlStreamReader::TokenType token = xml.readNext();
        if(token == QXmlStreamReader::StartDocument)
            continue;

        if(token == QXmlStreamReader::StartElement)
        {
            if(xml.name() == "assets")
                continue;
            if(xml.name() == "textures")
            {
                xml.readNext();
                texture_file=dir.absoluteFilePath(xml.text().toString());
            }
            if(xml.name() == "meshes")
            {
                xml.readNext();
                mesh_file=dir.absoluteFilePath(xml.text().toString());
            }
        }
    }
    return true;
}
