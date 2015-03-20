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
#ifndef TERRAINPANEL_H
#define TERRAINPANEL_H

#include <QMouseEvent>

#include "../toolcontrol.h"
#include "../../instrument.h"
#include "../../undostack.h"
#include "../../../canvas.h"
#include "../../../lib/module/terrain/terrainbase.h"
#include "../../../lib/worldio.h"

#include "action/terrainactionbrush.h"
#include "action/terrainactiondecrease.h"
#include "action/terrainactioneraser.h"
#include "action/terrainactionincrease.h"
#include "action/terrainactionsculptor.h"
#include "action/terrainactionsmooth.h"

#include "terrainmaterialdialog.h"
#include "terrainproceduraldialog.h"
#include "terrainimportdialog.h"

#include "terrainundo.h"
#include "terrainshadow.h"

/**
 * @brief The TerrainTool class
 */
class TerrainControl : public ToolControl
{
    Q_OBJECT
    Q_PROPERTY(QStandardItemModel* textureModel READ textureModel NOTIFY textureModelChanged)
    Q_PROPERTY(float pointerSize READ pointerSize WRITE setPointerSize NOTIFY pointerSizeChanged)
    Q_PROPERTY(float pointerOffset READ pointerOffset WRITE setPointerOffset NOTIFY pointerOffsetChanged)
    Q_PROPERTY(float materialOpacity READ materialOpacity WRITE setMaterialOpacity NOTIFY materialOpacityChanged)
    Q_PROPERTY(float materialFade READ materialFade WRITE setMaterialFade NOTIFY materialFadeChanged)
    Q_PROPERTY(bool lockX READ lockX WRITE setLockX NOTIFY lockXChanged)
    Q_PROPERTY(bool lockY READ lockY WRITE setLockY NOTIFY lockYChanged)
    Q_PROPERTY(bool lockZ READ lockZ WRITE setLockZ NOTIFY lockZChanged)
    Q_PROPERTY(bool blankTexture READ blankTexture WRITE setBlankTexture NOTIFY blankTextureChanged)
    Q_PROPERTY(int verticalTexture READ verticalTexture WRITE setVerticalTexture NOTIFY verticalTextureChanged)
    Q_PROPERTY(int horizontalTexture READ horizontalTexture WRITE setHorizontalTexture NOTIFY horizontalTextureChanged)
    Q_PROPERTY(PointerShape pointerShape READ pointerShape WRITE setPointerShape NOTIFY pointerShapeChanged)
    Q_PROPERTY(PointerType pointerType READ pointerType WRITE setPointerType NOTIFY pointerTypeChanged)
    Q_PROPERTY(Tool tool READ tool WRITE setTool NOTIFY toolChanged)

    Q_ENUMS(Tool)
    Q_ENUMS(PointerType)
    Q_ENUMS(PointerShape)

public:
    enum Tool
    {
        ERASER=0,
        SCULPTOR,
        SMOOTH,
        BRUSH,
        INCREASE,
        DECREASE
    };

    enum PointerType{
        POINTER=0,
        MAGNET
    };

    enum PointerShape{
        SPHERE=0,
        BOX
    };

    explicit TerrainControl(QQuickItem *parent = 0);
    ~TerrainControl();

    void setup(Instrument * navig);
    void undo();
    void redo();
    void mousePress(QMouseEvent *event);
    void mouseRelease(QMouseEvent *event);
    void mouseMove(QMouseEvent *event);
    void keyPress(QKeyEvent *event);
    void update(int elapsed_time, const Player & player);
    void wheel(QWheelEvent *event);
    void onEnter();
    void onLeave();
    void loadProject(Project *project);
    void unloadProject();

    TerrainGraphics *getTerrain() const;

    void reloadMaterials();
    float backupValue(unsigned x, unsigned y, unsigned z) const;

    float materialFade() const;
    float materialOpacity() const;
    unsigned char textureVertical() const;
    unsigned char textureHorizontal() const;

    QStandardItemModel* textureModel() const;
    float pointerSize() const;
    float pointerOffset() const;
    bool lockX() const;
    bool lockY() const;
    bool lockZ() const;

    PointerShape pointerShape() const;
    PointerType pointerType() const;
    int verticalTexture() const;
    int horizontalTexture() const;
    Tool tool() const;
    bool blankTexture() const;

public slots:
    void fillGround(float height);
    void clearTerrain();
    void rebuildTerrain();
    void updateMaterial(int id);

    void setTextureModel(QStandardItemModel* arg);
    void setPointerSize(float arg);
    void setPointerOffset(float arg);
    void setMaterialOpacity(float arg);
    void setMaterialFade(float arg);
    void setLockX(bool arg);
    void setLockY(bool arg);
    void setLockZ(bool arg);

    void setPointerShape(PointerShape arg);
    void setPointerType(PointerType arg);

    void setVerticalTexture(int arg);
    void setHorizontalTexture(int arg);

    void setTool(Tool arg);
    void setBlankTexture(bool arg);

signals:
    void textureModelChanged();
    void pointerSizeChanged(float arg);
    void pointerOffsetChanged(float arg);
    void materialFadeChanged(float value);
    void materialOpacityChanged(float value);

    void lockXChanged(bool arg);
    void lockYChanged(bool arg);
    void lockZChanged(bool arg);

    void pointerShapeChanged(PointerShape arg);
    void pointerTypeChanged(PointerType arg);
    void verticalTextureChanged(int arg);
    void horizontalTextureChanged(int arg);

    void toolChanged(Tool arg);
    void blankTextureChanged(bool arg);

private:

    void backup(int x, int y, int z, int size);
    void fillHeight(float height);
    void stroke(Shape *shape);

    int backup_count;
    bool left_press;
    int last_stroke;
    int elapsed_time;
    unsigned char texture_v, texture_h;

    UndoStack undo_stack;

    enum ModelRoles {
        TitleRole = Qt::UserRole + 1,
        PathRole,
        TextureIdRole
    };

    TerrainAction * tools[10];
    TerrainAction * instrument;
    Canvas * canvas;
    TerrainIO * terrain_io;
    TerrainCache shadow;
    TerrainGraphics * terrain;

    Map * map;
    Instrument * navig;

    QStandardItemModel* m_textureModel;
    float m_pointerSize;
    float m_pointerOffset;
    float m_materialOpacity;
    float m_materialFade;
    bool m_lockX;
    bool m_lockY;
    bool m_lockZ;
    PointerShape m_pointerShape;
    PointerType m_pointerType;
    int m_verticalTexture;
    int m_horizontalTexture;
    Tool m_tool;
    bool m_blankTexture;
};

#endif // TERRAINPANEL_H
