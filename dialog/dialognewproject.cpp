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
#include "dialognewproject.h"
#include "ui_dialognewproject.h"

DialogNewProject::DialogNewProject(QWidget *parent,Session * session) :
    QDialog(parent),
    ui(new Ui::DialogNewProject)
{
    ui->setupUi(this);
    this->session=session;

    ui->project_path->setText(QDir::home().path());
}

DialogNewProject::~DialogNewProject()
{
    delete ui;
}

void DialogNewProject::on_pushButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose Directory"),
                                                ui->project_path->displayText(),
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);
    QDir path(dir);
    if(path.exists())
    {
        ui->project_path->setText(dir);
    }

}

void DialogNewProject::on_buttonBox_accepted()
{
    if(ui->project_path->text().isEmpty())
    {
        QErrorMessage error(this);
        error.showMessage("Please set up name","Type??");
        return;
    }

    if(ui->folder_check->checkState()==Qt::Checked)
        session->createProject(ui->project_path->text(),ui->project_name->text()
                               ,ui->map_width->value(),ui->map_height->value(),ui->map_depth->value());
    else
    {
        QDir dir(ui->project_path->text());
        session->createProject(dir.absoluteFilePath(ui->project_name->text()),ui->project_name->text()
                               ,ui->map_width->value(),ui->map_height->value(),ui->map_depth->value());
    }

    close();
}

void DialogNewProject::on_buttonBox_rejected()
{
    close();
}
