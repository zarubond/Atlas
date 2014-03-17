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
#include "meshdialog.h"
#include "ui_meshdialog.h"

MeshDialog::MeshDialog(Assets *assets,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MeshDialog)
{
    ui->setupUi(this);
    this->assets=assets;
    selected=NULL;
}

MeshDialog::~MeshDialog()
{
    delete ui;
}

bool MeshDialog::isSelected()
{
    return (selected!=NULL);
}

QString MeshDialog::file()
{
    return selected->file;
}

QString MeshDialog::preview()
{
    return selected->preview;
}

QString MeshDialog::name()
{
    return selected->name;
}

void MeshDialog::load()
{
    if(assets->mesh_file.isEmpty()) return;

    QFile* file = new QFile(assets->mesh_file);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        //QMessageBox::critical(this,
          //                    "QXSRExample::parseXML",  "Couldn't open example.xml",  QMessageBox::Ok);
        //return false;
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
    ui->listWidget->setIconSize(QSize(100,100));
}

void MeshDialog::readMesh(QXmlStreamReader &xml)
{
    if(xml.tokenType() != QXmlStreamReader::StartElement && xml.name() == "mesh")
        return;

    Item * item=new Item;
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

                item->file=dir.absoluteFilePath(xml.text().toString());
            }
            else if(xml.name() == "preview") {
                xml.readNext();
                if(xml.tokenType() != QXmlStreamReader::Characters)
                    return;


                item->preview=dir.absoluteFilePath(xml.text().toString());
                item->setIcon(QIcon(QPixmap(item->preview).scaled(100,100,Qt::KeepAspectRatio)));
            }
            else if(xml.name() == "name") {
                xml.readNext();
                if(xml.tokenType() != QXmlStreamReader::Characters)
                    return;

                item->name=xml.text().toString();
                item->setText(item->name);
            }
            else if(xml.name() == "type") {
            }
        }
        xml.readNext();
    }
    ui->listWidget->addItem(item);
}

void MeshDialog::showEvent(QShowEvent *)
{
    load();
}

void MeshDialog::on_buttonBox_accepted()
{
    QList<QListWidgetItem *> items=ui->listWidget->selectedItems();
    if(items.size()==1)
    {
        Item * item=(Item*)items.back();
        selected=item;
    }
}

void MeshDialog::on_listWidget_itemClicked(QListWidgetItem *item)
{
    Item * it=(Item*)item;
    ui->name->setText(it->name);
    QPixmap pixmap(it->preview);
    if(!pixmap.isNull())
    {
        ui->preview->setPixmap(pixmap.scaled(QSize(300,300),Qt::KeepAspectRatio));
    }
}
