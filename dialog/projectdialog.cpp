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

#include "projectdialog.h"

ProjectDialog::ProjectDialog(QObject *parent) :
    Dialog(parent)
{
    m_project=NULL;
}

Project *ProjectDialog::project() const
{
    return m_project;
}

QString ProjectDialog::author() const
{
    return m_author;
}

QString ProjectDialog::info() const
{
    return m_info;
}

QUrl ProjectDialog::snapshot() const
{
    return m_snapshot;
}

QString ProjectDialog::name() const
{
    return m_name;
}

void ProjectDialog::accept()
{
    m_project->world.name=m_name.toUtf8().constData();
    m_project->world.author=m_author.toUtf8().constData();
    m_project->world.info=m_info.toUtf8().constData();
   // if(info.exists())
     //   project->world.snapshot;
    //TBD
    close();
}

void ProjectDialog::reject()
{
    close();
}

void ProjectDialog::setProject(Project *arg)
{
    if (m_project != arg) {
        m_project = arg;
        emit projectChanged(arg);
    }
}

void ProjectDialog::setAuthor(QString arg)
{
    if (m_author != arg) {
        m_author = arg;
        emit authorChanged(arg);
    }
}

void ProjectDialog::setInfo(QString arg)
{
    if (m_info != arg) {
        m_info = arg;
        emit infoChanged(arg);
    }
}

void ProjectDialog::setSnapshot(QUrl arg)
{
    if (m_snapshot != arg) {
        m_snapshot = arg;
        emit snapshotChanged(arg);
    }
}

void ProjectDialog::setName(QString arg)
{
    if (m_name != arg) {
        m_name = arg;
        emit nameChanged(arg);
    }
}

void ProjectDialog::opened()
{
    setName(m_project->world.name.c_str());
    setAuthor(m_project->world.author.c_str());
    setInfo(m_project->world.info.c_str());
    setSnapshot(QUrl(m_project->world.snapshot.c_str()));
}
