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
#ifndef SESSION_H
#define SESSION_H

#include <QSettings>
#include <QFileInfo>
#include <QXmlStreamWriter>
#include <QMessageBox>

#include "project/project.h"
#include "project/projecthandler.h"
#include "canvas.h"
#include "./editor/editor.h"

class Session
{
public:
    Session();
    ~Session();
    /**
     * @brief setup Initialize session.
     * @param space
     * @param editor
     */
    void setup(Canvas * space,Editor * editor);
    /**
     * @brief openProject Open project in file.
     * @param file_name
     * @return
     */
    bool openProject(const QString & file_name);
    /**
     * @brief saveProject Save loaded project
     */
    void saveProject();
    /**
     * @brief closeProject Close loaded project
     * @return
     */
    bool closeProject();
    /**
     * @brief createProject Create new project
     * @param project_path
     * @param name
     * @param width
     * @param height
     * @param depth
     * @return
     */
    bool createProject(const QString &project_path, const QString &name, int width, int height, int depth);
    /**
     * @brief isLoaded
     * @return Is project loaded
     */
    bool isLoaded();
    /**
     * @brief project Pointer to loaded project
     */
    Project * project;
    QSettings * getSettings();

private:

    ProjectHandler handler;
    Canvas * space;
    Editor * editor;
    QSettings settings;
};

#endif // SESSION_H
