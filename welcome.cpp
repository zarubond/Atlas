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

#include "welcome.h"

#include "atlas.h"

Welcome::Welcome(QQuickItem *parent) :
    QQuickItem(parent),m_projectModel(new QStandardItemModel)
{
    QHash<int, QByteArray> roleNames;
    roleNames[Welcome::TitleRole] =  "title";
    roleNames[Welcome::PathRole] = "path";
    m_projectModel->setItemRoleNames(roleNames);

    connect(this,SIGNAL(visibleChanged()),this,SLOT(reloadRecentProjects()));
}

Welcome::~Welcome()
{
    delete m_projectModel;
}

void Welcome::load(Window *atlas)
{
    this->atlas=atlas;
}

QStandardItemModel *Welcome::projectModel() const
{
    return m_projectModel;
}

void Welcome::setProjectModel(QStandardItemModel *arg)
{
    if (m_projectModel != arg) {
        m_projectModel = arg;
        emit projectModelChanged();
    }
}

void Welcome::reloadRecentProjects()
{
    QStringList files = GlobalSettings::instance()->recentProjects();

    m_projectModel->clear();
    QFileInfo info;
    for(int i=0;i<files.size();i++)
    {
        info.setFile(files[i]);
        if(info.exists())
        {
            QStandardItem * item=new QStandardItem;
            item->setData(info.baseName(),Welcome::TitleRole);
            item->setData(info.filePath(),Welcome::PathRole);
            m_projectModel->appendRow(item);
        }
    }
    emit projectModelChanged();
    files.clear();
}

void Welcome::openProject(int index)
{
    QString path = m_projectModel->item(index)->data(Welcome::PathRole).toString();
    atlas->openProject(path);
}
