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
#include "configdialog.h"
#include "ui_configdialog.h"

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);

    QListWidgetItem *configButton = new QListWidgetItem(ui->listWidget);
    //configButton->setIcon(QIcon(":/images/config.png"));
    configButton->setText(tr("Configuration"));
    configButton->setTextAlignment(Qt::AlignHCenter);
    configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *updateButton = new QListWidgetItem(ui->listWidget);
    //updateButton->setIcon(QIcon(":/images/update.png"));
    updateButton->setText(tr("Update"));
    updateButton->setTextAlignment(Qt::AlignHCenter);
    updateButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    loadAssets();
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}

void ConfigDialog::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;

    ui->stackedWidget->setCurrentIndex(ui->listWidget->row(current));
}

void ConfigDialog::loadAssets()
{
    ui->assetPath->setText(Settings::getString("assets/file"));
}

void ConfigDialog::saveAssets()
{
    Settings::setValue("assets/file",ui->assetPath->text());
}

void ConfigDialog::on_browseAsset_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Find Assets"),QString(),tr("Assets (*.aas)"));
    if(!file.isEmpty())
    {
        Settings::setValue("assets/file",file);
        ui->assetPath->setText(file);
    }
}

void ConfigDialog::on_buttonBox_accepted()
{
    saveAssets();
}
