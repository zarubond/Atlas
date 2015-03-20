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

#include "assetmeshdialog.h"

AssetMeshDialog::AssetMeshDialog(QObject *parent) :
    Dialog(parent),m_model(new QStandardItemModel)
{
    QHash<int, QByteArray> roleNames;
    roleNames[AssetMeshDialog::TitleRole] =  "title";
    roleNames[AssetMeshDialog::UrlRole] = "meshUrl";
    roleNames[AssetMeshDialog::PreviewRole] = "preview";
    roleNames[AssetMeshDialog::LicenceRole] = "licence";
    roleNames[AssetMeshDialog::AuthorRole] = "author";
    m_model->setItemRoleNames(roleNames);
    selected=NULL;
    m_project=NULL;
}

AssetMeshDialog::~AssetMeshDialog()
{
    delete m_model;
}

bool AssetMeshDialog::isSelected()
{
    return (selected!=NULL);
}

QString AssetMeshDialog::file()
{
    return selected->data(AssetMeshDialog::UrlRole).toString();
}

QString AssetMeshDialog::preview()
{
    return selected->data(AssetMeshDialog::PreviewRole).toString();
}

QString AssetMeshDialog::name()
{
    return selected->data(AssetMeshDialog::TitleRole).toString();
}

Project *AssetMeshDialog::project() const
{
    return m_project;
}

QStandardItemModel *AssetMeshDialog::model() const
{
    return m_model;
}

QUrl AssetMeshDialog::meshUrl() const
{
    return m_meshUrl;
}

void AssetMeshDialog::setProject(Project *arg)
{
    if (m_project != arg) {
        m_project = arg;
        if(m_project!=NULL)
            assets=m_project->assets;
        emit projectChanged(arg);
    }
}

void AssetMeshDialog::setMeshUrl(QUrl arg)
{
    if (m_meshUrl != arg) {
        m_meshUrl = arg;
        emit meshUrlChanged(arg);
    }
}

void AssetMeshDialog::opened()
{
    if(assets->mesh_file.isEmpty()) return;

    m_meshUrl.clear();

    QFile* file = new QFile(assets->mesh_file);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        //QMessageBox::critical(this,
          //                    "QXSRExample::parseXML",  "Couldn't open example.xml",  QMessageBox::Ok);
        //return false;
        cerr<<"Unable to open asset file:"<<assets->mesh_file<<endl;
        return;
    }

    QFileInfo info(assets->mesh_file);
    dir=info.absoluteDir();

    QXmlStreamReader xml(file);
    while(!xml.atEnd() && !xml.hasError())
    {
        QXmlStreamReader::TokenType token = xml.readNext();
        if(token == QXmlStreamReader::StartDocument)
            continue;

        if(token == QXmlStreamReader::StartElement)
        {
            if(xml.name() == "meshes")
                continue;
            else if(xml.name() == "mesh")
            {
                readMesh(xml);
            }
        }
    }
    emit modelChanged();
}

void AssetMeshDialog::readMesh(QXmlStreamReader &xml)
{
    if(xml.tokenType() != QXmlStreamReader::StartElement && xml.name() == "mesh")
        return;


    QStandardItem * item=new QStandardItem;
    xml.readNext();
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "mesh"))
    {
        if(xml.tokenType() == QXmlStreamReader::StartElement)
        {
            if(xml.name() == "file")
            {
                xml.readNext();
                if(xml.tokenType() != QXmlStreamReader::Characters)
                    return;
                QUrl url=QUrl::fromLocalFile(dir.absoluteFilePath(xml.text().toString()));
                item->setData(url,AssetMeshDialog::UrlRole);
            }
            else if(xml.name() == "preview") {
                xml.readNext();
                if(xml.tokenType() != QXmlStreamReader::Characters)
                    return;
                QUrl url(QUrl::fromLocalFile(dir.absoluteFilePath(xml.text().toString())));
                item->setData(url.url(),AssetMeshDialog::PreviewRole);
            }
            else if(xml.name() == "name") {
                xml.readNext();
                if(xml.tokenType() == QXmlStreamReader::Characters)
                    item->setData(xml.text().toString(),AssetMeshDialog::TitleRole);
                else
                    item->setData("???",AssetMeshDialog::TitleRole);
            }
            else if(xml.name() == "licence")
            {
                xml.readNext();
                if(xml.tokenType() == QXmlStreamReader::Characters)
                    item->setData(xml.text().toString(),AssetMeshDialog::LicenceRole);
                else
                    item->setData("???",AssetMeshDialog::LicenceRole);
            }
            else if(xml.name() == "author")
            {
                xml.readNext();
                if(xml.tokenType() == QXmlStreamReader::Characters)
                    item->setData(xml.text().toString(),AssetMeshDialog::AuthorRole);
                else
                    item->setData("???",AssetMeshDialog::AuthorRole);
            }
            else if(xml.name() == "type") {
            }
        }
        xml.readNext();
    }

    m_model->appendRow(item);
}
