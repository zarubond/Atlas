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
#ifndef VEGETATIONTOOL_H
#define VEGETATIONTOOL_H

#include <QWidget>
#include <QMouseEvent>
#include "editortool.h"

#include "../../tools/simplexnoise.h"
#include "../navigator.h"
#include "../../world/map.h"
#include "../shape.h"

#include "./dialog/dialoggrasstexture.h"

namespace Ui {
class VegetationTool;
}

/**
 * @brief The VegetationTool class
 */

class VegetationTool : public EditorTool
{
    Q_OBJECT
public:
    enum Tool
    {
        PLANT=0,
        DIG
    };

    explicit VegetationTool(QWidget *parent = 0);
    ~VegetationTool();


    void setTool(VegetationTool::Tool tool);

    void setup(Canvas *,Navigator * navig,Assets * assets);
    void undo();
    void loadProject(Project *project);
    void unloadProject();
    void onEnter();
    void mousePress(QMouseEvent *event);
    QString getName();

    QMenu *createMenu();
private slots:
    void on_grassPlant_clicked();
    void on_grassDig_clicked();

    void grassTexture();
private:
    void stroke(const Shape *shape);
    void make(float x, float y, float z);
    void plant(const Shape *shape);
    void dig(const Shape *shape);

    Ui::VegetationTool *ui;

    SimplexNoise noise;
    Map * map;
    GrassGraphics * base;
    GrassIO * base_io;
    Assets * assets;

    Navigator * navig;
    Tool tool;
};

#endif // VEGETATIONTOOL_H
