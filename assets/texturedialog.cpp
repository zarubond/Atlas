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
#include "texturedialog.h"
#include "ui_texturedialog.h"

TextureDialog::TextureDialog(Assets *assets, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TextureDialog)
{
    ui->setupUi(this);
    this->assets=assets;
    ui->listWidget->setIconSize(QSize(100,100));
    selected=NULL;

    setWindowTitle("Select Texture");
}

TextureDialog::~TextureDialog()
{
    delete ui;
}

bool TextureDialog::isSelected()
{
    return (selected!=NULL);
}

QString TextureDialog::file()
{
    return selected->file;
}

QString TextureDialog::normal()
{
    return selected->normal;
}

QString TextureDialog::name()
{
    return selected->name;
}

void TextureDialog::load()
{
    if(assets->texture_file.isEmpty()) return;

    QFile* file = new QFile(assets->texture_file);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }
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
    ui->listWidget->setIconSize(QSize(100,100));
}

int TextureDialog::exec()
{
    return QDialog::exec();
}

void TextureDialog::readTexture(QXmlStreamReader &xml)
{
    if(xml.tokenType() != QXmlStreamReader::StartElement && xml.name() == "texture")
        return;

    Item * item=new Item;
    QFileInfo info;
    xml.readNext();
    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "texture"))
    {
        if(xml.tokenType() == QXmlStreamReader::StartElement)
        {
            if(xml.name() == "file")
            {
                xml.readNext();
                if(xml.tokenType() != QXmlStreamReader::Characters)
                    return;

                item->file=dir.absoluteFilePath(xml.text().toString());
                info.setFile(item->file);

                if(info.exists())
                {
                    item->setIcon(QIcon(QPixmap(item->file).scaled(100,100,Qt::KeepAspectRatio)));
                }
                else
                {
                    qDebug()<<"Unable to load asset:"<<xml.text().toString();
                }

            }
            else if(xml.name() == "normal")
            {
                xml.readNext();
                if(xml.tokenType() != QXmlStreamReader::Characters)
                    return;

                item->normal=dir.absoluteFilePath(xml.text().toString());
                info.setFile(item->normal);
                if(!info.exists())
                {
                    qDebug()<<"Unable to load asset:"<<xml.text().toString();
                }
            }
            else if(xml.name() == "name")
            {
                xml.readNext();
                if(xml.tokenType() != QXmlStreamReader::Characters)
                    return;

                item->name=xml.text().toString();
                item->setText(item->name);
            }
            else if(xml.name() == "type")
            {
            }
        }
        xml.readNext();
    }
    ui->listWidget->addItem(item);
}

void TextureDialog::on_buttonBox_accepted()
{
    QList<QListWidgetItem *> items=ui->listWidget->selectedItems();
    if(items.size()==1)
    {
        Item * item=(Item*)items.back();
        selected=item;
    }
}

void TextureDialog::showEvent(QShowEvent *)
{
    load();
}

void TextureDialog::on_listWidget_itemClicked(QListWidgetItem *item)
{
    Item * it=(Item*)item;
    ui->name->setText(it->name);
    QPixmap pixmap(it->file);
    if(!pixmap.isNull())
    {
        ui->preview->setPixmap(pixmap.scaled(QSize(300,300),Qt::KeepAspectRatio));
    }
}
