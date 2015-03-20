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
#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <QMouseEvent>
#include "tool/terrain/terraincontrol.h"
#include "tool/model/modeltool.h"
#include "tool/vegetation/vegetationtool.h"
#include "tool/water/watertool.h"

#include "instrument.h"
#include "driver.h"
#include "../assets/assets.h"
#include "../globalsettings.h"

#include "tool/model/modeltool.h"

/**
 * @brief The Editor class
 */

class Toolbox : public QObject
{
     Q_OBJECT
public:
    Toolbox();
    ~Toolbox();

    void setProject(Project * project);
    void unloadProject();
    ToolControl *getTool(int tool_id);
    void selectTool(int tool_id);

    void undo();
    void redo();

    void wheelEvent(QWheelEvent * event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent * event);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent * e);
    void hoverMoveEvent(QHoverEvent * event);

    void update(int elapsed_time, Player &player);

    void addTool(ToolControl * tool);

private:

    float navig_dist;
    Map * map;
    Instrument intrument;
    ToolControl * tools[10];
    ToolControl * tool;

    ModelTool model;

    int last_time;
    int tool_count;
    int mouse_x, mouse_y;
};

#endif // TOOLBOX_H
