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
#include "dialogsun.h"
#include "ui_dialogsun.h"

DialogSun::DialogSun(LightManager *light, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSun),light(light)
{
    ui->setupUi(this);
    Vertex3f color=light->sun.getDiffuse();
    color*=255;
    ui->pushChoose_Diffuse->setPalette(QPalette(QColor(color[0],color[1],color[2])));
    color=light->sun.getSpecular();
    color*=255;
    ui->pushChoose_Specular->setPalette(QPalette(QColor(color[0],color[1],color[2])));
    color=light->getAmbient();
    color*=255;
    ui->pushChoose_Ambient->setPalette(QPalette(QColor(color[0],color[1],color[2])));

    ui->sliderShiniess->setValue(light->sun.getShininess()*light->sun.getShininess());

    Vertex3f dir=light->sun.getDirection();
    ui->dialX->setValue(atan2(dir[1],dir[0])*57.2957795);
    ui->dialY->setValue(acos(dir[2])*57.2957795);
}

DialogSun::~DialogSun()
{
    delete ui;
}

void DialogSun::diffuseColorChange(const QColor &color)
{
    light->sun.setDiffuse(color.red()/255.0,color.green()/255.0,color.blue()/255.0);
    ui->pushChoose_Diffuse->setPalette(QPalette(color));
}

void DialogSun::specularColorChange(const QColor &color)
{
    light->sun.setSpecular(color.red()/255.0,color.green()/255.0,color.blue()/255.0);
    ui->pushChoose_Specular->setPalette(QPalette(color));
}

void DialogSun::ambientColorChange(const QColor &color)
{
    light->setAmbient(color.red()/255.0,color.green()/255.0,color.blue()/255.0);
    ui->pushChoose_Ambient->setPalette(QPalette(color));
}

void DialogSun::on_pushChoose_Ambient_clicked()
{
    Vertex3f col=light->getAmbient();
    col*=255;
    QColorDialog * dialog=new QColorDialog(QColor(col[0],col[1],col[2]),this);
    connect(dialog,SIGNAL(currentColorChanged(QColor)),this,SLOT(ambientColorChange(QColor)));
    dialog->show();
}

void DialogSun::on_pushChoose_Specular_clicked()
{
    Vertex3f col=light->sun.getSpecular();
    col*=255;
    QColorDialog * dialog=new QColorDialog(QColor(col[0],col[1],col[2]),this);
    connect(dialog,SIGNAL(currentColorChanged(QColor)),this,SLOT(specularColorChange(QColor)));
    dialog->show();
}

void DialogSun::on_pushChoose_Diffuse_clicked()
{
    Vertex3f col=light->sun.getDiffuse();
    col*=255;
    QColorDialog * dialog=new QColorDialog(QColor(col[0],col[1],col[2]),this);
    connect(dialog,SIGNAL(currentColorChanged(QColor)),this,SLOT(diffuseColorChange(QColor)));
    dialog->show();
}

void DialogSun::on_sliderShiniess_valueChanged(int value)
{
    float v=sqrt(value+1.0);
    light->sun.setShininess(v);
}

void DialogSun::on_dialX_valueChanged(int value)
{
    Vertex3f dir=light->sun.getDirection();
    float theta=acos(dir[2]);
    float fi=value*0.0174532925;

    dir[0]=sin(theta)*cos(fi);
    dir[1]=sin(theta)*sin(fi);
    dir[2]=cos(theta);

    light->sun.setDirection(dir);
}

void DialogSun::on_dialY_valueChanged(int value)
{
    Vertex3f dir=light->sun.getDirection();
    float theta=value*0.0174532925;
    theta/=2;
    float fi=atan2(dir[1],dir[0]);

    dir[0]=sin(theta)*cos(fi);
    dir[1]=sin(theta)*sin(fi);
    dir[2]=cos(theta);
      //  dir.print();
    light->sun.setDirection(dir);
}
