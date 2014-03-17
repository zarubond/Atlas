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
#include "dialoggrasstexture.h"
#include "ui_dialoggrasstexture.h"

DialogGrassTexture::DialogGrassTexture(GrassGraphics *grass, GrassIO *grass_io, Assets *assets, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogGrassTexture)
{
    ui->setupUi(this);

    this->assets=assets;
    this->grass_io=grass_io;

    QLabel * label;
    QPushButton * button,*button_asset;
    int max_grass=1;

    labels_texture=new QLabel*[max_grass];
    texture_path=new QString[max_grass];

    for(int i=0;i<max_grass;i++)
    {
        label=new QLabel(tr("Texture %1").arg(i));
        labels_texture[i]=label;

        button=new QPushButton("Import Texture");
        texture_group.addButton(button,i);
        button_asset=new QPushButton("Assets");
        assets_buttons.addButton(button_asset,i);

        if(!grass_io->getTexturePath(i)->isEmpty())
        {
            QPixmap pixmap(*(grass_io->getTexturePath(i)));
            if(!pixmap.isNull())
            {
                label->setPixmap(pixmap.scaledToWidth(100));
            }
        }

        ui->textureGrid->addWidget(label,i,0);
        ui->textureGrid->addWidget(button,i,2);
        ui->textureGrid->addWidget(button_asset,i,4);
    }

    connect(&texture_group,SIGNAL(buttonClicked(int)),this,SLOT(importTexture(int)));
    connect(&assets_buttons,SIGNAL(buttonClicked(int)),this,SLOT(importAsset(int)));
    changed=false;
}

DialogGrassTexture::~DialogGrassTexture()
{
    delete ui;
}

void DialogGrassTexture::importTexture(int id)
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

void DialogGrassTexture::importAsset(int id)
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
    }
}

void DialogGrassTexture::on_buttonBox_accepted()
{
    int max_textures=1;
    if(changed)
        for(int i=0;i<max_textures;i++)
        {
            if(!texture_path[i].isEmpty())
                grass_io->importTexture(i,texture_path[i]);
        }
}
