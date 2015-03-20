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

#include "assettexturedialog.h"

AssetTextureDialog::AssetTextureDialog(QObject *parent) :
    Dialog(parent),m_model(new QStandardItemModel)
{
    QHash<int, QByteArray> roleNames;
    roleNames[AssetTextureDialog::TitleRole] =  "title";
    roleNames[AssetTextureDialog::DiffuseRole] = "diffuse";
    roleNames[AssetTextureDialog::NormalRole] = "normal";
    roleNames[AssetTextureDialog::SpecularRole] = "specular";
    roleNames[AssetTextureDialog::LicenceRole] = "licence";
    roleNames[AssetTextureDialog::AuthorRole] = "author";
    m_model->setItemRoleNames(roleNames);
    selected=NULL;

    m_project=NULL;
}

AssetTextureDialog::~AssetTextureDialog()
{
    delete m_model;
}

QStandardItemModel *AssetTextureDialog::model() const
{
    return m_model;
}

Project *AssetTextureDialog::project() const
{
    return m_project;
}

QUrl AssetTextureDialog::materialDiffuse() const
{
    return m_materialDiffuse;
}

QUrl AssetTextureDialog::materialNormal() const
{
    return m_materialNormal;
}

QUrl AssetTextureDialog::materialSpecular() const
{
    return m_materialSpecular;
}

QString AssetTextureDialog::materialName() const
{
    return m_materialName;
}

void AssetTextureDialog::setModel(QStandardItemModel *arg)
{
    if (m_model != arg) {
        m_model = arg;
        emit modelChanged();
    }
}

void AssetTextureDialog::setProject(Project *arg)
{
    if (m_project != arg) {
        m_project = arg;
        if(m_project!=NULL) assets=m_project->assets;
        emit projectChanged(arg);
    }
}

void AssetTextureDialog::setMaterialName(QString arg)
{
    if (m_materialName != arg) {
        m_materialName = arg;
        emit materialNameChanged(arg);
    }
}

void AssetTextureDialog::setMaterialDiffuse(QUrl arg)
{
    if (m_materialDiffuse != arg) {
        m_materialDiffuse = arg;
        emit materialDiffuseChanged(arg);
    }
}

void AssetTextureDialog::setMaterialNormal(QUrl arg)
{
    if (m_materialNormal != arg) {
        m_materialNormal = arg;
        emit materialNormalChanged(arg);
    }
}

void AssetTextureDialog::setMaterialSpecular(QUrl arg)
{
    if (m_materialSpecular != arg) {
        m_materialSpecular = arg;
        emit materialSpecularChanged(arg);
    }
}

void AssetTextureDialog::readTexture(QXmlStreamReader &xml)
{
    if(xml.tokenType() != QXmlStreamReader::StartElement && xml.name() == "texture")
        return;

    QStandardItem * item=new QStandardItem;
    QFileInfo info;
    xml.readNext();
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "texture"))
    {
        if(xml.tokenType() == QXmlStreamReader::StartElement)
        {
            if(xml.name() == "file" || xml.name() == "diffuse")
            {
                xml.readNext();
                if(xml.tokenType() != QXmlStreamReader::Characters)
                    return;

                info.setFile(dir.absoluteFilePath(xml.text().toString()));

                if(info.exists())
                {
                    item->setData(QUrl::fromLocalFile(info.filePath()),AssetTextureDialog::DiffuseRole);
                }
                else
                {
                    cerr<<"Unable to load asset: "<<xml.text().toString()<<endl;
                }

            }
            else if(xml.name() == "normal")
            {
                xml.readNext();
                if(xml.tokenType() != QXmlStreamReader::Characters)
                    return;

                info.setFile(dir.absoluteFilePath(xml.text().toString()));
                if(info.exists())
                {
                    item->setData(QUrl::fromLocalFile(info.filePath()),AssetTextureDialog::NormalRole);
                }
                else
                {
                    cerr<<"Unable to load asset: "<<xml.text().toString()<<endl;
                }
            }
            else if(xml.name() == "specular")
            {
                xml.readNext();
                if(xml.tokenType() != QXmlStreamReader::Characters)
                    return;

                info.setFile(dir.absoluteFilePath(xml.text().toString()));
                if(info.exists())
                {
                    item->setData(QUrl::fromLocalFile(info.filePath()),AssetTextureDialog::SpecularRole);
                }
                else
                {
                    cerr<<"Unable to load asset: "<<xml.text().toString()<<endl;
                }
            }
            else if(xml.name() == "name")
            {
                xml.readNext();
                if(xml.tokenType() == QXmlStreamReader::Characters)
                    item->setData(xml.text().toString(),AssetTextureDialog::TitleRole);
                else
                    item->setData("???",AssetTextureDialog::TitleRole);
            }
            else if(xml.name() == "licence")
            {
                xml.readNext();
                if(xml.tokenType() == QXmlStreamReader::Characters)
                    item->setData(xml.text().toString(),AssetTextureDialog::LicenceRole);
                else
                    item->setData("???",AssetTextureDialog::LicenceRole);
            }
            else if(xml.name() == "author")
            {
                xml.readNext();
                if(xml.tokenType() == QXmlStreamReader::Characters)
                    item->setData(xml.text().toString(),AssetTextureDialog::AuthorRole);
                else
                    item->setData("???",AssetTextureDialog::AuthorRole);
            }
            else if(xml.name() == "type")
            {
            }
        }
        xml.readNext();
    }
    m_model->appendRow(item);
    emit modelChanged();
}

void AssetTextureDialog::opened()
{
    if(assets->texture_file.isEmpty()) return;

    QFile* file = new QFile(assets->texture_file);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    m_materialNormal=QUrl();
    m_materialDiffuse=QUrl();
    m_materialSpecular=QUrl();
    m_materialName=QString();

    QFileInfo info(assets->texture_file);
    dir=info.absoluteDir();

    QXmlStreamReader xml(file);
    while(!xml.atEnd() && !xml.hasError())
    {
        QXmlStreamReader::TokenType token = xml.readNext();
        if(token == QXmlStreamReader::StartDocument)
            continue;

        if(token == QXmlStreamReader::StartElement)
        {
            if(xml.name() == "textures")
                continue;
            if(xml.name() == "texture")
            {
                readTexture(xml);
            }
        }
    }
}
