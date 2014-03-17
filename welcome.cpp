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
#include "welcome.h"
#include "ui_welcome.h"

#include "atlas.h"

Welcome::Welcome(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Welcome)
{
    ui->setupUi(this);

    for(int i=0;i<5;i++)
    {
        buttons[i]=NULL;
    }
}

Welcome::~Welcome()
{
    delete ui;
}

void Welcome::load(Atlas *atlas)
{
    this->atlas=atlas;

    QVBoxLayout* layout = new QVBoxLayout();
    QSettings settings;

    files = settings.value("recentFileList").toStringList();
    for(int i=0;i<files.size() && i<5;i++)
    {
        QString text = tr("&%1. %2").arg(i + 1).arg(files[i]);
        QPushButton * label=new QPushButton(text);
        label->setFlat(true);
        label->setStyleSheet ("text-align: left");
        connect(label,SIGNAL(clicked()),this,SLOT(openRecentProject()));
        layout->addWidget(label);
        buttons[i]=label;
    }
    QSpacerItem * space=new QSpacerItem(10,10,QSizePolicy::Minimum,QSizePolicy::Expanding);
    layout->addItem(space);
    delete ui->project_frame->layout();
    ui->project_frame->setLayout(layout);
}


void Welcome::on_push_OpenProject_clicked()
{
    atlas->openProjectDialog(true);
}

void Welcome::on_push_NewProject_clicked()
{
    atlas->newProjectDialog(true);
}

void Welcome::openRecentProject()
{
    QPushButton *action = qobject_cast<QPushButton *>(sender());
    if (action!=NULL)
    {
        for(int i=0;i<5 && i<files.size();i++)
        {
            if(action==buttons[i])
            {
                atlas->openProject(files[i]);
                break;
            }
        }
    }
}
