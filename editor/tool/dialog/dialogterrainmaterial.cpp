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
#include "dialogterrainmaterial.h"
#include "ui_dialogterraintexture.h"

#include "../terraintool.h"

DialogTerrainMaterial::DialogTerrainMaterial(TerrainGraphics * terrain, TerrainIO * terrain_io,Assets * assets, QWidget *parent):
    QDialog(parent),
    ui(new Ui::DialogTerrainTexture),terrain(terrain),terrain_io(terrain_io)
{
    ui->setupUi(this);

    this->assets=assets;

    QLabel * label;
    QLabel * label_normal;
    QPushButton * button,*button_normal,*button_asset;

    labels_texture=new QLabel*[terrain->maxTextures()];
    labels_normal=new QLabel*[terrain->maxTextures()];
    texture_path=new QString[terrain->maxTextures()];
    normalmap_path=new QString[terrain->maxTextures()];

    for(int i=0;i<terrain->maxTextures();i++)
    {
        label=new QLabel(tr("Texture %1").arg(i));
        labels_texture[i]=label;
        label_normal=new QLabel(tr("Normal %1").arg(i));
        labels_normal[i]=label_normal;

        button=new QPushButton("Import Texture");
        texture_group.addButton(button,i);
        button_normal=new QPushButton("Import Normal");
        normalmap_buttons.addButton(button_normal,i);
        button_asset=new QPushButton("Assets");
        assets_buttons.addButton(button_asset,i);

        if(!terrain_io->getTexturePath(i)->isEmpty())
        {
            QPixmap pixmap(*(terrain_io->getTexturePath(i)));
            QPixmap normalmap(*(terrain_io->getNormalMapPath(i)));
            if(!pixmap.isNull())
            {
                label->setPixmap(pixmap.scaledToWidth(100));
                label_normal->setPixmap(normalmap.scaledToWidth(100));
            }
        }

        ui->textureGrid->addWidget(label,i,0);
        ui->textureGrid->addWidget(button,i,2);
        ui->textureGrid->addWidget(label_normal,i,1);
        ui->textureGrid->addWidget(button_normal,i,3);
        ui->textureGrid->addWidget(button_asset,i,4);
    }

    connect(&texture_group,SIGNAL(buttonClicked(int)),this,SLOT(importTexture(int)));
    connect(&normalmap_buttons,SIGNAL(buttonClicked(int)),this,SLOT(importNormalMap(int)));
    connect(&assets_buttons,SIGNAL(buttonClicked(int)),this,SLOT(importAsset(int)));
    changed=false;
}

DialogTerrainMaterial::~DialogTerrainMaterial()
{
    delete [] texture_path;
    delete [] normalmap_path;
    delete ui;
}

void DialogTerrainMaterial::importTexture(int id)
{
    QString supportedImageFormats;
    for (int formatIndex = 0; formatIndex < QImageReader::supportedImageFormats().count(); formatIndex++) {
        supportedImageFormats += QLatin1String(" *.") + QImageReader::supportedImageFormats()[formatIndex];
    }

    QString file_name = QFileDialog::getOpenFileName(this,
        tr("Import Image"),"", tr("Images (%1)").arg(supportedImageFormats));

    QImage image(file_name);
    if(!image.isNull())
    {
        QPixmap pixmap=QPixmap::fromImage(image);
        labels_texture[id]->setPixmap(pixmap.scaledToWidth(100));
        texture_path[id]=file_name;
        changed=true;
    }
}

void DialogTerrainMaterial::importNormalMap(int id)
{
    QString supportedImageFormats;
    for (int formatIndex = 0; formatIndex < QImageReader::supportedImageFormats().count(); formatIndex++) {
        supportedImageFormats += QLatin1String(" *.") + QImageReader::supportedImageFormats()[formatIndex];
    }

    QString file_name = QFileDialog::getOpenFileName(this,
        tr("Import Normal Map"),"", tr("Images (%1)").arg(supportedImageFormats));

    QImage image(file_name);
    if(!image.isNull())
    {
        QPixmap pixmap=QPixmap::fromImage(image);
        labels_normal[id]->setPixmap(pixmap.scaledToWidth(100));
        normalmap_path[id]=file_name;
        changed=true;
    }
}

void DialogTerrainMaterial::importAsset(int id)
{
    TextureDialog dialog(assets);
    dialog.exec();

    if(dialog.isSelected())
    {
        QImage image(dialog.file());
        if(!image.isNull())
        {
            QPixmap pixmap=QPixmap::fromImage(image);
            labels_texture[id]->setPixmap(pixmap.scaledToWidth(100));
            texture_path[id]=dialog.file();
            changed=true;
        }

        image.load(dialog.normal());
        if(!image.isNull())
        {
            QPixmap pixmap=QPixmap::fromImage(image);
            labels_normal[id]->setPixmap(pixmap.scaledToWidth(100));
            normalmap_path[id]=dialog.normal();
            changed=true;
        }

    }

}

void DialogTerrainMaterial::on_buttonBox_accepted()
{
    if(changed)
        for(int i=0;i<terrain->maxTextures();i++)
        {
            if(!texture_path[i].isEmpty())
                terrain_io->importTexture(i,texture_path[i]);

            if(!normalmap_path[i].isEmpty())
                terrain_io->importNormalMap(i,normalmap_path[i]);
        }
}
