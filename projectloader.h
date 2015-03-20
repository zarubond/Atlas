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
#ifndef LOADING_H
#define LOADING_H

#include <QQuickItem>
#include <QFileInfo>
#include <QOpenGLContext>
#include <QQuickWindow>
#include <QSettings>

#include "project.h"
#include "projectio.h"
#include "globalsettings.h"

class ProjectLoader : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(float loadingProgress READ loadingProgress WRITE setLoadingProgress NOTIFY loadingProgressChanged)
    Q_PROPERTY(QString loadingMessage READ loadingMessage WRITE setLoadingMessage NOTIFY loadingMessageChanged)

public:
    explicit ProjectLoader(QQuickItem *parent = 0);

    void openProject(const QString &file_path, QOpenGLContext *context);
    void createProject();
    float loadingProgress() const;
    QString loadingMessage() const;
    void saveProject();
    void saveProjectAs(const QUrl & save_folder);

    void createProject(const QString &path, const QString &name, const QString &author, int width, int height, int depth);
public slots:
    void setLoadingProgress(float arg);
    void setLoadingMessage(QString & arg);

signals:
    void projectLoaded(Project * project);
    void loadingProgressChanged(float arg);
    void loadingMessageChanged(QString arg);

private slots:
    void updateProgress(QString caption, int value);
    void loadingDone(bool status);
private:
    void addRecentProject(const QString &project_file);
    ProjectIO handler;
    Project * project;
    float m_loadingProgress;
    QString m_loadingMessage;
};

#endif // LOADING_H
