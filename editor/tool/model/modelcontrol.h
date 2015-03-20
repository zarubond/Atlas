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
#ifndef MODELPANEL_H
#define MODELPANEL_H

#include "../toolcontrol.h"
#include "../../../project.h"
#include "../../../lib/map.h"
#include "../../../lib/tools/camera.h"
#include "../../../canvas.h"
#include "../../../assets/assetmeshdialog.h"

#include "meshdatamodel.h"
#include "modeldatamodel.h"
#include "modeleditdialog.h"

#include <QMouseEvent>
#include <QStandardItem>
#include <QStandardItemModel>

/**
 * @brief The ModelPanel class
 */
class ModelControl : public ToolControl
{
    Q_OBJECT
    Q_PROPERTY(QStandardItemModel* treeModel READ treeModel NOTIFY treeModelChanged)
    Q_PROPERTY(int mapWidth READ mapWidth WRITE setMapWidth NOTIFY mapWidthChanged)
    Q_PROPERTY(int mapHeight READ mapHeight WRITE setMapHeight NOTIFY mapHeightChanged)
    Q_PROPERTY(int mapDepth READ mapDepth WRITE setMapDepth NOTIFY mapDepthChanged)

    Q_PROPERTY(MeshDataModel* mesh READ mesh NOTIFY meshChanged)
    Q_PROPERTY(QModelIndex meshIndex READ meshIndex WRITE setMeshIndex NOTIFY meshIndexChanged)
    Q_PROPERTY(ModelDataModel* model READ model NOTIFY modelChanged)
    Q_PROPERTY(QModelIndex modelIndex READ modelIndex WRITE setModelIndex NOTIFY modelIndexChanged)
    Q_PROPERTY(ModelOperation modelOperation READ modelOperation WRITE setModelOperation NOTIFY modelOperationChanged)

public:

    enum ModelOperation{
        POSITION,
        SCALE,
        ROTATION
    };
    Q_ENUMS(ModelOperation)

    explicit ModelControl(QQuickItem *parent = 0);
    ~ModelControl();
    void setup(Instrument *navig);

    void mousePress(QMouseEvent *event);
    void mouseMove(QMouseEvent *event);
    void mouseRelease(QMouseEvent *event);
    void mouseDoubleClick(QMouseEvent *event);
    void loadProject(Project * project);
    void unloadProject();
    void onEnter();
    void onLeave();
    void update(int elapsed_time, const Player &player);

    QStandardItemModel* treeModel() const;

    int mapWidth() const;
    int mapHeight() const;
    int mapDepth() const;

    MeshDataModel *mesh();
    ModelDataModel *model();

    QModelIndex meshIndex() const;
    QModelIndex modelIndex() const;

    ModelOperation modelOperation() const;

public slots:
    void duplicateModel();

    void newModel();
    void importMesh(QUrl path);
    void modelChanged(QStandardItem* item);

    void deleteModel(const QModelIndex & index);
    void deleteMesh(const QModelIndex & index);

    void setMapWidth(int arg);
    void setMapHeight(int arg);
    void setMapDepth(int arg);

    void setMeshIndex(const QModelIndex & arg);
    void setModelIndex(const QModelIndex &arg);

    void updateMeshName(const QString & name);
    void updateModelName(const QString & name);

    void setModelOperation(ModelOperation arg);

signals:
    void treeModelChanged();
    void mapWidthChanged(int arg);
    void mapHeightChanged(int arg);
    void mapDepthChanged(int arg);

    void modelIndexChanged(QModelIndex arg);
    void modelPositionChanged(QVector3D arg);
    void modelRotationChanged(QVector3D arg);
    void modelNameChanged(QString arg);
    void modelScaleChanged(float arg);

    //hacky
    void selectModel(int meshIndex, int modelIndex);
    void selectMesh(int meshIndex);

    void meshChanged(MeshDataModel * arg);
    void modelChanged(ModelDataModel * arg);

    void meshIndexChanged(QModelIndex arg);
    void modelOperationChanged(ModelOperation arg);

private slots:
    void modelChanged();
private:
    void selectModel(Model * model);
    Model *findModel(int x, int y);
    /**
     * @brief deselectModel Deselect model if selected.
     */
    void deselectModel();
    void reloadTree();

    void addMesh(ModelMesh *mesh);

    ModelGraphics * base;
    ModelIO * model_io;
    EditorGraphics * editor;
    Map * map;
    Instrument * navig;
    Camera camera;
    float axis_dist;
    Vector3f model_center;
    Vector3f orig_scale;


    enum ModelRoles {
        TextRole = Qt::UserRole + 1,
        ElementRole
    };

    enum ItemType
    {
        MODEL=0,
        MESH
    };

    int mouse_x,mouse_y;
    bool left_press;
    int axis_select;
    Vector3f pos_dif;

    QStandardItemModel* m_treeModel;
    int m_mapWidth;
    int m_mapHeight;
    int m_mapDepth;

    MeshDataModel m_mesh;
    ModelDataModel m_model;
    QModelIndex m_meshIndex;
    QModelIndex m_modelIndex;
    ModelOperation m_modelOperation;
};

#endif // MODELPANEL_H
