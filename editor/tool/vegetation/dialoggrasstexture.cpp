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

#include "dialoggrasstexture.h"

DialogGrassTexture::DialogGrassTexture(GrassGraphics *grass, GrassIO *grass_io, Assets *assets, QWindow *parent) :
    Dialog(parent)
{
    this->assets=assets;
    this->grass_io=grass_io;

    QHash<int, QByteArray> roleNames;
    roleNames[DialogGrassTexture::TitleRole] =  "title";
    roleNames[DialogGrassTexture::NormalRole] = "normal";
    roleNames[DialogGrassTexture::TextureRole] = "texture";
    roleNames[DialogGrassTexture::MaterialIdRole] = "material_id";
    roleNames[DialogGrassTexture::ChangedRole] = "changed";
    model.setItemRoleNames(roleNames);
}

DialogGrassTexture::~DialogGrassTexture()
{
}

const char *DialogGrassTexture::uiFile()
{
    return "tools/dialogGrassTexture.qml";
}

void DialogGrassTexture::setupUi(QQuickItem *ui)
{
    connect(ui,SIGNAL(accepted()),this,SLOT(accepted()));
    connect(ui,SIGNAL(rejected()),this,SLOT(rejected()));
    connect(ui,SIGNAL(importAsset(int)),this,SLOT(importAsset(int)));
    connect(ui,SIGNAL(replaceTexture(int,QString)),this,SLOT(replaceTexture(int,QString)));

    std::string supportedImageFormats=Image::supportedFormats();
    ui->setProperty("supported_formats",QString(supportedImageFormats.c_str()));

    QString st;
    int max_grass=1;
    for(int i=0;i<max_grass;i++)
    {
        if(!grass_io->getTexturePath(i)->isEmpty())
        {
            QFileInfo info(*(grass_io->getTexturePath(i)));
            if(info.exists())
            {
                QStandardItem * item=new QStandardItem;
                QUrl url(QUrl::fromLocalFile(info.absoluteFilePath()));
                item->setData(url.url(),TextureRole);
                item->setData(false,ChangedRole);
            }
        }
    }
}

void DialogGrassTexture::replaceTexture(int index, QString path)
{
    QStandardItem * item=model.item(index);
    QUrl url(path);
    QFileInfo info(url.toLocalFile());
    if(item!=NULL && info.exists())
    {
        item->setData(path,TextureRole);
        item->setData(true,ChangedRole);
    }
}

void DialogGrassTexture::importAsset(int index)
{
    QStandardItem * item=model.item(index);
    if(item!=NULL)
    {
        //AssetTextureDialog dialog(assets);
        //dialog.exec();
/*
        if(dialog.isSelected())
        {
            QFileInfo info(dialog.texture());
            QUrl url(info.absoluteFilePath());
            item->setData(url.url(),TextureRole);
            item->setData(true,ChangedRole);
        }*/
    }
}


void DialogGrassTexture::rejected()
{
    close();
}

void DialogGrassTexture::accepted()
{
    QStandardItem * item;
    QUrl url;
    int id;

    for(int i=0;i<model.rowCount();i++)
    {
        item=model.item(i);
        if(item->data(ChangedRole).toBool())
        {
            id=item->data(MaterialIdRole).toInt();

            url.setPath(item->data(TextureRole).toString());
            grass_io->importTexture(id,url.toLocalFile());
        }
    }
    close();
}
