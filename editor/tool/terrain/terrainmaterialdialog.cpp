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

#include "terrainmaterialdialog.h"

#include "terraincontrol.h"

TerrainMaterialDialog::TerrainMaterialDialog(QObject *parent):
    Dialog(parent),m_model(new QStandardItemModel)
{
    QHash<int, QByteArray> roleNames;
    roleNames[TerrainMaterialDialog::TitleRole] =  "title";
    roleNames[TerrainMaterialDialog::NormalRole] = "normal";
    roleNames[TerrainMaterialDialog::TextureRole] = "texture";
    roleNames[TerrainMaterialDialog::MaterialIdRole] = "material_id";
    roleNames[TerrainMaterialDialog::ChangedRole] = "changed";
    m_model->setItemRoleNames(roleNames);
    m_maximumMaterials=4;

    m_project=NULL;
    m_tool=NULL;
    terrain_io=NULL;
}

TerrainMaterialDialog::~TerrainMaterialDialog()
{
    delete m_model;
}

QStandardItemModel *TerrainMaterialDialog::model() const
{
    return m_model;
}

QString TerrainMaterialDialog::supportedFormats() const
{
    return m_supportedFormats;
}

int TerrainMaterialDialog::maximumMaterials() const
{
    return m_maximumMaterials;
}

Project *TerrainMaterialDialog::project() const
{
    return m_project;
}

TerrainControl *TerrainMaterialDialog::tool() const
{
    return m_tool;
}

void TerrainMaterialDialog::setProject(Project *arg)
{
    if (m_project != arg) {
        m_project = arg;
        if(m_project!=NULL)
        {
            terrain = &m_project->world.terrain;
            terrain_io=&m_project->io->terrain_io;
            m_maximumMaterials=terrain->maxMaterials();
            emit maximumMaterials();
        }
        emit projectChanged(arg);

    }
}

void TerrainMaterialDialog::setMaterial(int index, const QString &name, const QUrl &diffuse, const QUrl &normal, const QUrl &specular)
{
    QStandardItem * item=m_model->item(index);
    if(item!=NULL)
    {
        item->setData(name,TitleRole);
        item->setData(diffuse,TextureRole);
        item->setData(normal,NormalRole);
        item->setData(true,ChangedRole);
        emit modelChanged();
    }
}

void TerrainMaterialDialog::setMaterialName(int index, const QString &name)
{
    QStandardItem * item=m_model->item(index);
    if(item!=NULL)
    {
        item->setData(name,TitleRole);
        item->setData(true,ChangedRole);
        emit modelChanged();
    }
}

void TerrainMaterialDialog::replaceTexture(int index, QUrl url)
{
    QStandardItem * item=m_model->item(index);
    QFileInfo info(url.toLocalFile());
    if(item!=NULL && info.exists())
    {
        item->setData(url,TextureRole);
        item->setData(true,ChangedRole);
    }

}

void TerrainMaterialDialog::replaceNormalMap(int index, QUrl url)
{
    QStandardItem * item=m_model->item(index);
    QFileInfo info(url.toLocalFile());
    if(item!=NULL && info.exists())
    {
        item->setData(url,NormalRole);
        item->setData(true,ChangedRole);
    }
}

void TerrainMaterialDialog::addMaterial()
{
    if(m_model->rowCount()<terrain->maxMaterials())
    {
        int * id=new int[terrain->maxMaterials()];
        for(int i=0;i<terrain->maxMaterials();i++) id[i]=-1;
        for(int i=0;i<m_model->rowCount();i++)
        {
            id[m_model->item(i)->data(MaterialIdRole).toInt()]=i;
        }
        int index=-1;
        for(int i=0;i<terrain->maxMaterials();i++)
            if(id[i]==-1)
            {
                index=i;
                break;
            }

        if(index!=-1)
        {
            QStandardItem * item=new QStandardItem;
            item->setData("Unknown",TitleRole);
            item->setData(QUrl(),TextureRole);
            item->setData(QUrl(),NormalRole);
            item->setData(index,MaterialIdRole);
            m_model->appendRow(item);
        }
        delete [] id;
    }
}

void TerrainMaterialDialog::setTool(TerrainControl *arg)
{
    if (m_tool != arg) {
        m_tool = arg;
        emit toolChanged(arg);
    }
}

void TerrainMaterialDialog::changeName(int index, const QString & name)
{
    QStandardItem * item=m_model->item(index);
    if(item!=NULL)
        item->setData(name,TerrainMaterialDialog::TitleRole);
}

void TerrainMaterialDialog::reject()
{
    close();
}

void TerrainMaterialDialog::setMaximumMaterials(int count)
{
    if(m_maximumMaterials!=count)
    {
        m_maximumMaterials=count;
        emit maximumMaterialsChanged(count);
    }
}

void TerrainMaterialDialog::opened()
{
    QFileInfo info;
    QUrl url;
    for(int i=0;i<terrain->maxMaterials();i++)
    {
        TerrainMaterial * material=terrain->getMaterial(i);
        if(material!=NULL && material->enabled())
        {
            QStandardItem * item=new QStandardItem;
            info.setFile(QString(material->texture_path.c_str()));
            url=QUrl::fromLocalFile(info.filePath());
            item->setData(url,TextureRole);
            info.setFile(QString(material->normal_map_path.c_str()));
            url=QUrl::fromLocalFile(info.filePath());
            item->setData(url,NormalRole);
            item->setData(i,MaterialIdRole);
            item->setData(QString(material->name.c_str()),TitleRole);
            item->setData(false,ChangedRole);
            m_model->appendRow(item);
        }
    }
    emit modelChanged();

    setMaximumMaterials(terrain->maxMaterials());

    m_supportedFormats=Image::supportedFormats().c_str();
    emit supportedFormatsChanged(m_supportedFormats);
}

void TerrainMaterialDialog::accept()
{
    QStandardItem * item;
    QUrl url;
    int id;

    for(int i=0;i<m_model->rowCount();i++)
    {
        item=m_model->item(i);
        if(item->data(ChangedRole).toBool())
        {
            id=item->data(MaterialIdRole).toInt();

            url=item->data(TextureRole).toUrl();
            if(!url.isEmpty())
                terrain_io->importTexture(id, url.toLocalFile().toStdString(), item->data(TitleRole).toString().toStdString());

            url=item->data(NormalRole).toUrl();
            if(!url.isEmpty())
                terrain_io->importNormalMap(id,url.toLocalFile().toStdString());
        }
    }
    m_tool->reloadMaterials();

    close();
}
