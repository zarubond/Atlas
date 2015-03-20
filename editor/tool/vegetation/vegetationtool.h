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
#ifndef VEGETATIONTOOL_H
#define VEGETATIONTOOL_H

#include <QMouseEvent>
#include "../toolcontrol.h"

#include "../../instrument.h"
#include "../../../lib/map.h"
#include "../../shape.h"

#include "dialoggrasstexture.h"

/**
 * @brief The VegetationTool class
 */

class VegetationTool : public ToolControl
{
    Q_OBJECT
public:
    enum Tool
    {
        PLANT=0,
        DIG
    };

    explicit VegetationTool(QQuickItem *parent = 0);
    ~VegetationTool();

    void setup(Instrument * navig);
    const char *uiFile();
    void uiSetup(QQuickItem *ui);
    void undo();
    void redo();
    void loadProject(Project * project);
    void unloadProject();
    void mousePress(QMouseEvent *event);
    void mouseRelease(QMouseEvent *event);
    void mouseMove(QMouseEvent * event);
    void onEnter();
    void onLeave();

    void setTool(VegetationTool::Tool tool);
private slots:
    void grassPlant();
    void grassDig();
    void pointerSize(float value);

    void openTextureDialog();
private:
    void stroke(const Shape *shape);
    void plant(const Shape *shape);
    void dig(const Shape *shape);
    bool removeTufts(GrassRegion *region, const Shape *shape);

    Map * map;
    QQuickItem *ui;
    GrassGraphics * base;
    GrassIO * base_io;
    Assets * assets;
    float orig_offset;

    Instrument * navig;
    Tool tool;
    bool left_press;
};

#endif // VEGETATIONTOOL_H
