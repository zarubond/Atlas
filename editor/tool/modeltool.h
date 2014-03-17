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
#ifndef MODELTOOL_H
#define MODELTOOL_H

#include "./editortool.h"
#include "../../project/project.h"
#include "../../world/map.h"
#include "../../tools/camera.h"
#include "../../canvas.h"
#include "../../assets/meshdialog.h"

#include <QMouseEvent>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QMenu>
#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>

namespace Ui {
class ModelTool;
}
/**
 * @brief The ModelTool class
 */
class ModelTool : public EditorTool
{
    Q_OBJECT
public:
    explicit ModelTool(QWidget *parent = 0);
    ~ModelTool();
    void setup(Canvas *canvas, Navigator *navig, Assets *assets);

    void mousePress(QMouseEvent *event);

    void mouseMove(QMouseEvent *event);
    void mouseRelease(QMouseEvent *event);
    void wheel(QWheelEvent *event);
    void loadProject(Project *project);
    void unloadProject();
    void onEnter();
    void onLeave();
    QString getName();
    QMenu * createMenu();

private slots:
    void on_position_x_valueChanged(double arg1);
    void on_position_y_valueChanged(double arg1);
    void on_position_z_valueChanged(double arg1);
    void on_rotate_x_valueChanged(int value);
    void on_rotate_y_valueChanged(int value);
    void on_rotate_z_valueChanged(int value);
    void on_angle_x_valueChanged(double arg1);
    void on_angle_y_valueChanged(double arg1);
    void on_angle_z_valueChanged(double arg1);
    void on_treeView_customContextMenuRequested(const QPoint &);
    void on_treeView_clicked(const QModelIndex &index);
    void on_scale_valueChanged(double arg1);
    void on_treeView_activated(const QModelIndex &index);

    void test_slot();
    void duplicateModel();
    void deleteModel();
    void deleteMesh();
    void newModel();
    void importMesh();
    void importAsset();
    void modelChanged(QStandardItem* item);

private:
    Model *findModel(int x, int y);
    void selectModel(Model * base);
    /**
     * @brief deselectModel Deselect model if selected.
     */
    void deselectModel();
    void reloadTree();

    void addMesh(ModelMesh *mesh);

    void rotateObject(float x, float y, float z);

    Ui::ModelTool *ui;
    ModelGraphics * base;
    ModelIO * model_io;
    Map * map;
    Navigator * navig;
    Assets * assets;

    Canvas * canvas;

    Model * select_model;
    Vertex3f pos_dif;
    QModelIndex select_index;
    int items;
    QStandardItemModel tree_model;

    enum ItemType
    {
        MODEL=0,
        MESH
    };

    int mouse_x,mouse_y;
    bool left_press;

    float o_x,o_y,o_z;
    float factor;
};

#endif // MODELTOOL_H
