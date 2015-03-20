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

#include "newprojectdialog.h"

#include "../atlas.h"

NewProjectDialog::NewProjectDialog(QObject *parent) :
    Dialog(parent)
{
    m_path=QDir::homePath();
    m_url=QUrl::fromLocalFile(QDir::homePath());

#if defined(Q_OS_ANDROID)
    m_path=QStandardPaths::writableLocation(QStandardPaths::DataLocation);
#elif defined(Q_OS_LINUX)
    m_path=QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
#elif defined(Q_OS_WIN)
    m_path=QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
#endif
    m_url=QUrl::fromLocalFile(m_path);

    QString name = qgetenv("USER"); // get the user name in Linux
    if(name.isEmpty()) {
        name = qgetenv("USERNAME"); // get the name in Windows
    }
    m_author=name;
    m_atlasWindow=NULL;
    m_width=32;
    m_height=32;
    m_depth=32;
    m_step=32;
}

QString NewProjectDialog::name() const
{
    return m_name;
}

QString NewProjectDialog::author() const
{
    return m_author;
}

QString NewProjectDialog::path() const
{
    return m_path;
}

int NewProjectDialog::width() const
{
    return m_width;
}

int NewProjectDialog::height() const
{
    return m_height;
}

int NewProjectDialog::depth() const
{
    return m_depth;
}

Window *NewProjectDialog::atlasWindow() const
{
    return m_atlasWindow;
}

void NewProjectDialog::opened()
{
    setWidth(32);
    setHeight(32);
    setDepth(32);
}

QUrl NewProjectDialog::url() const
{
    return m_url;
}

int NewProjectDialog::step() const
{
    return m_step;
}

void NewProjectDialog::accept()
{
    QDir dir(m_path);
    close();
    m_atlasWindow->createProject(dir.absoluteFilePath(m_name),m_name,m_author,m_width,m_height,m_depth);
}

void NewProjectDialog::reject()
{
    close();
}

void NewProjectDialog::setName(const QString &arg)
{
    if (m_name != arg) {
        m_name = arg;
        emit nameChanged(arg);
    }
}

void NewProjectDialog::setAuthor(const QString& arg)
{
    if (m_author != arg) {
        m_author = arg;
        emit authorChanged(arg);
    }
}

void NewProjectDialog::setPath(const QString &arg)
{
    if (m_path != arg) {
        m_path = arg;
        emit pathChanged(arg);
    }
}

void NewProjectDialog::setWidth(int arg)
{
    if (m_width != arg) {
        m_width = arg;
        emit widthChanged(arg);
    }
}

void NewProjectDialog::setHeight(int arg)
{
    if (m_height != arg) {
        m_height = arg;
        emit heightChanged(arg);
    }
}

void NewProjectDialog::setDepth(int arg)
{
    if (m_depth != arg) {
        m_depth = arg;
        emit depthChanged(arg);
    }
}

void NewProjectDialog::setAtlasWindow(Window *arg)
{
    if (m_atlasWindow != arg) {
        m_atlasWindow = arg;
        emit atlasWindowChanged(arg);
    }
}

void NewProjectDialog::setUrl(const QUrl& arg)
{
    if (m_url != arg) {
        m_url = arg;
        setPath(m_url.toLocalFile());
        emit urlChanged(arg);
    }
}

void NewProjectDialog::setStep(int arg)
{
    if (m_step != arg) {
        m_step = arg;
        emit stepChanged(arg);
    }
}
