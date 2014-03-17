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
#ifndef EDITOR_H
#define EDITOR_H

#include <QMouseEvent>
#include "tool/terraintool.h"
#include "tool/modeltool.h"
#include "tool/vegetationtool.h"

#include "navigator.h"
#include "driver.h"
#include "../assets/assets.h"
#include "../settings.h"

/**
 * @brief The Editor class
 */

class Editor : public QObject
{
     Q_OBJECT
public:
    Editor();
    void init(Canvas * canvas);

    void setProject(Project * project);
    void unsetProject();
    int toolCount();
    EditorTool *getTool(int tool_id);
    void selectTool(int tool_id);

    void undo();
    void redo();

    void wheelEvent(QWheelEvent * event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent * e);

    bool eventFilter(QObject *obj, QEvent *event);

    void update(int elapsed_time);

    Driver driver;
    Assets assets;
private:


    Navigator navig;
    float navig_dist;

    Canvas * canvas;
    Player player;
    Map * map;
    EditorTool * tools[10];
    EditorTool * tool;
    int last_time;
    int tool_count;

};

#endif // EDITOR_H
