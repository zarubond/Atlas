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
#ifndef TERRAINTOOL_H
#define TERRAINTOOL_H

#include <QWidget>
#include <QMouseEvent>
#include <QPushButton>
#include <QMenu>
#include <QInputDialog>
#include <QMessageBox>
#include <QUndoStack>

#include "./editortool.h"
#include "../navigator.h"
#include "../../canvas.h"
#include "../../world/module/terrain/terrainbase.h"

#include "action/terrainactionbrush.h"
#include "action/terrainactiondecrease.h"
#include "action/terrainactioneraser.h"
#include "action/terrainactionincrease.h"
#include "action/terrainactionsculptor.h"
#include "action/terrainactionsmooth.h"

#include "dialog/dialogterrainmaterial.h"
#include "dialog/dialogterrainprocedural.h"
#include "dialog/dialogterrainimport.h"

#include "terrainundo.h"

namespace Ui {
class TerrainTool;
}
/**
 * @brief The TerrainTool class
 */
class TerrainTool : public EditorTool
{
    Q_OBJECT
public:
    explicit TerrainTool(QWidget *parent = 0);
    ~TerrainTool();

    void setup(Canvas * canvas,Navigator * navig,Assets * assets);
//overloaded
    void undo();
    void redo();
    void stroke(const Shape *shape);
    void mousePress(QMouseEvent *event);
    void mouseRelease(QMouseEvent *event);
    void mouseMove(QMouseEvent *event);
    void keyPress(QKeyEvent *event);
    void update(int elapsed_time);
    void wheel(QWheelEvent *event);
    void onEnter();
    void onLeave();
    void loadProject(Project *project);
    void unloadProject();
    QMenu * createMenu();
    QString getName();

    TerrainGraphics *getTerrain() const;

    void fillHeight(float height);

    float backupValue(unsigned x, unsigned y, unsigned z) const;

    void setTexture(int id, QImage &texture);
    unsigned char currentMaterial() const;
public slots:
    void openMaterialDialog();
    void openProceduralDialog();
    void action_FillGround();
    void action_Import();
    void action_ClearAll();
    void action_Rebuild();
    void newMaterial(int id,QImage & img);

private slots:

    void on_toolSculptor_clicked();
    void on_toolEraces_clicked();
    void on_toolBrush_clicked();
    void on_toolSmooth_clicked();
    void on_toolIncreser_clicked();
    void on_toolDecreser_clicked();
    void on_handTypeMagnet_clicked();
    void on_handTypePointer_clicked();
    void on_lockX_clicked();
    void on_lockY_clicked();
    void on_lockZ_clicked();
    void on_handShapeSphere_clicked();
    void on_handShapeBox_clicked();
    void on_comboVertical_currentIndexChanged(int index);
    void on_comboHorizontal_currentIndexChanged(int index);

private:
    enum Tool
    {
        ERASER=0,
        SCULPTOR,
        SMOOTH,
        BRUSH,
        INCREASE,
        DECREASE
    };
    Assets * assets;

    Ui::TerrainTool *ui;

    void backup(int x, int y, int z, int size);
    void setTool(Tool tool);

    int backup_count;
    bool left_press;
    int last_time;

    int texture_vertical,texture_horizontal;
    QUndoStack undo_stack;

    TerrainAction * tools[10];
    TerrainAction * tool;
    Canvas * canvas;
    TerrainGraphics * terrain;
    TerrainIO * terrain_io;
    Map * map;
    Navigator * navig;
};

#endif // TERRAINTOOL_H
