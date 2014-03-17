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
#include "dialogterrainimport.h"
#include "ui_dialogterrainimport.h"

#include "../terraintool.h"

DialogTerrainImport::DialogTerrainImport(TerrainTool *parent) :
    QDialog(parent),
    ui(new Ui::DialogTerrainImport)
{
    ui->setupUi(this);
    tool=parent;
    terrain=tool->getTerrain();
    texture=1;

    ui->spinDepth->setMaximum(terrain->regionsX()*terrain->regionSize());
    ui->spinDepth->setValue(terrain->regionsX()*terrain->regionSize());
    ui->spinWidth->setMaximum(terrain->regionsZ()*terrain->regionSize());
    ui->spinWidth->setValue(terrain->regionsZ()*terrain->regionSize());
    ui->spinMaxHeight->setMaximum(terrain->regionsY()*terrain->regionSize());
    ui->spinMaxHeight->setValue(terrain->regionsY()*terrain->regionSize());

    QFileInfo info;

    for(int i=0;i<terrain->maxTextures();i++)
    {
        /*info.setFile(terrain->texture_file[i].c_str());*/
        ui->combo_texture->addItem(info.fileName(),QVariant(i+1));
    }
}

DialogTerrainImport::~DialogTerrainImport()
{
    delete ui;
}

void DialogTerrainImport::on_pushButton_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this,
        tr("Import Image"),"", tr("Image Files (*.png *.jpg *.bmp *.tga)"));

    if(image.load(file_name) && !image.isNull())
    {
        QRgb col;
        int gray;
        int width = image.width();
        int height = image.height();
        for (int i = 0; i < width; ++i)
        {
            for (int j = 0; j < height; ++j)
            {
                col = image.pixel(i, j);
                gray = qGray(col);
                image.setPixel(i, j, qRgb(gray, gray, gray));
            }
        }
        int w=ui->spinWidth->value();
        int h=ui->spinDepth->value();
        QImage img(image.scaled(w,h));

        ui->image->setPixmap(QPixmap::fromImage(img));
    }
}

void DialogTerrainImport::on_buttonBox_accepted()
{
    if(image.isNull())
    {
        close();
        return;
    }

    int w=ui->spinWidth->value();
    int d=ui->spinDepth->value();
    int max_height=ui->spinMaxHeight->value();
    int min_height=ui->spinMinHeight->value();

    QImage img(image.scaled(w,d));

    for(int i=0; i<w;i++)
        for(int j=0; j<d;j++)
        {
            QRgb rgb=img.pixel(i,j);

            float value=qRed(rgb)*(max_height/256.0f)+min_height;

            float val;
            int max=terrain->regionsY()*terrain->regionSize()-1;
            for(int h=0;h<max;h++)
            {
                val=(h-value)*8.0f;
                if(val>127)
                    break;

                if(val<-127)
                    val=-127.0;

                terrain->setVoxel(i,h,j,val,texture);
            }
        }
    terrain->rebuid();

    close();
}

void DialogTerrainImport::on_buttonBox_rejected()
{
    close();
}

void DialogTerrainImport::on_spinMinHeight_valueChanged(int arg1)
{
    if(arg1>=ui->spinMaxHeight->value())
    {
        ui->spinMinHeight->setValue(ui->spinMaxHeight->value()-1);
    }
    else
    {
        int w=ui->spinWidth->value();
        int h=ui->spinDepth->value();
        if(!image.isNull())
            ui->image->setPixmap(QPixmap::fromImage(image.scaled(w,h)));
    }
}

void DialogTerrainImport::on_spinMaxHeight_valueChanged(int arg1)
{
    if(ui->spinMinHeight->value()>=arg1)
    {
        ui->spinMaxHeight->setValue(ui->spinMinHeight->value()+1);
    }
    else
    {
        int w=ui->spinWidth->value();
        int h=ui->spinDepth->value();
        if(!image.isNull())
            ui->image->setPixmap(QPixmap::fromImage(image.scaled(w,h)));
    }
}

void DialogTerrainImport::on_combo_texture_currentIndexChanged(int index)
{
    QVariant var=ui->combo_texture->itemData(index);
    texture=var.toInt();
}
