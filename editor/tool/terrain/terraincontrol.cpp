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

#include "terraincontrol.h"

TerrainControl::TerrainControl(QQuickItem *parent) :
    ToolControl(parent),m_textureModel(new QStandardItemModel), terrain(NULL)
{
    for(int i=0;i<10;i++)
        tools[i]=NULL;

    tools[SMOOTH]   = new TerrainActionSmooth(this);
    tools[SCULPTOR] = new TerrainActionSculptor(this);
    tools[ERASER]   = new TerrainActionEraser(this);
    tools[INCREASE] = new TerrainActionIncrease(this);
    tools[BRUSH]    = new TerrainActionBrush(this);
    tools[DECREASE] = new TerrainActionDecrease(this);

    m_tool=SCULPTOR;
    instrument=tools[SCULPTOR];

    left_press=false;
    undo_stack.setUndoLimit(64);
    texture_v=0;
    texture_h=0;

    elapsed_time=0;
    last_stroke=0;

    QHash<int, QByteArray> roleNames;
    roleNames[TerrainControl::TitleRole] =  "title";
    roleNames[TerrainControl::PathRole] = "path";
    roleNames[TerrainControl::TextureIdRole] = "texture_id";
    m_textureModel->setItemRoleNames(roleNames);

    m_pointerShape=SPHERE;
    m_pointerType=MAGNET;
    m_pointerSize=0.0;
    m_pointerOffset=0.0;
    m_materialFade=1.0;
    m_materialOpacity=1.0;
    m_lockX=false;
    m_lockY=false;
    m_lockZ=false;
    m_verticalTexture=-1;
    m_horizontalTexture=-1;
    m_blankTexture=false;
}

TerrainControl::~TerrainControl()
{
    for(int i=0;i<10;i++)
        delete tools[i];
    delete m_textureModel;
}

void TerrainControl::setup(Instrument *navig)
{
    this->navig=navig;
}

void TerrainControl::undo()
{
    undo_stack.undo();
    terrain->rebuid();
}

void TerrainControl::redo()
{
    undo_stack.redo();
}

TerrainGraphics *TerrainControl::getTerrain() const
{
    return terrain;
}

void TerrainControl::fillHeight(float height)
{
    TerrainVoxel voxel;
    voxel.setTextureRawH(m_horizontalTexture,15);
    voxel.setTextureRawV(m_verticalTexture,15);
    for(int x=0;x<terrain->regionSize()*terrain->regionsX();x++)
    {
        for(int z=0;z<terrain->regionsZ()*terrain->regionSize();z++)
        {
            for(int y=0;y<ceil(height);y++)
            {
                voxel.value=-127;
                terrain->setVoxel(x,y,z,&voxel);
            }
            voxel.value=ceil(127);
            terrain->setVoxel(x,ceil(height),z,&voxel);
        }
    }
    terrain->rebuid();
}

void TerrainControl::stroke(Shape *shape)
{
    if(terrain!=NULL && elapsed_time-last_stroke>100)
    {
        if(instrument->makeShadowTerrain())
            shadow.swapTerrainData();
        TerrainUndo * und=new TerrainUndo(terrain);
        instrument->stroke(und,shape,&shadow,texture_v+1,texture_h+1);
        terrain->rebuid();
        if(instrument->makeShadowTerrain())
            shadow.swapTerrainData();
        undo_stack.push(und);
        last_stroke=elapsed_time;
    }
}

void TerrainControl::mousePress(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        stroke(navig);
        left_press=true;
    }
}

void TerrainControl::mouseRelease(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        left_press=false;
        if(instrument->makeShadowTerrain())
        {
            shadow.swapTerrainData();
            shadow.clear();
        }
    }
}

void TerrainControl::mouseMove(QMouseEvent *)
{
    if(left_press)
        stroke(navig);
}

void TerrainControl::keyPress(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_X:
        setLockX(!m_lockX);
        break;
    case Qt::Key_Y:
        setLockY(!m_lockY);
        break;
    case Qt::Key_Z:
        setLockZ(!m_lockZ);
        break;
    }
}

void TerrainControl::update(int elapsed_time, const Player &player)
{
    this->elapsed_time=elapsed_time;
    if(left_press && instrument->repeat())
        stroke(navig);
}

void TerrainControl::wheel(QWheelEvent *event)
{
}

void TerrainControl::onEnter()
{
    navig->show();
    if(navig->minRadius()>navig->maxRadius()*m_pointerSize)
        navig->setRadius(navig->minRadius());
    else
        navig->setRadius(navig->maxRadius()*m_pointerSize);

    if(m_pointerType==POINTER)
        navig->setMode(Instrument::POINTER);
    else if(m_pointerType==MAGNET)
        navig->setMode(Instrument::MAGNET);
}

void TerrainControl::onLeave()
{
    navig->hide();
}

void TerrainControl::loadProject(Project* project)
{
    terrain=&project->world.terrain;
    terrain_io=&project->io->terrain_io;
    map=&project->world.map;
    reloadMaterials();

    shadow.setTerrain(terrain);
}

void TerrainControl::unloadProject()
{
    if(terrain!=NULL)
    {
        setVerticalTexture(0);
        setHorizontalTexture(0);
        map=NULL;
        terrain=NULL;
        undo_stack.clear();
        m_textureModel->clear();
    }
}

float TerrainControl::materialFade() const
{
    return m_materialFade;
}

float TerrainControl::materialOpacity() const
{
    return m_materialOpacity;
}

QStandardItemModel *TerrainControl::textureModel() const
{
    return m_textureModel;
}

float TerrainControl::pointerSize() const
{
    return m_pointerSize;
}

float TerrainControl::pointerOffset() const
{
    return m_pointerOffset;
}

bool TerrainControl::lockX() const
{
    return m_lockX;
}

bool TerrainControl::lockY() const
{
    return m_lockY;
}

bool TerrainControl::lockZ() const
{
    return m_lockZ;
}

TerrainControl::PointerShape TerrainControl::pointerShape() const
{
    return m_pointerShape;
}

TerrainControl::PointerType TerrainControl::pointerType() const
{
    return m_pointerType;
}

int TerrainControl::verticalTexture() const
{
    return m_verticalTexture;
}

int TerrainControl::horizontalTexture() const
{
    return m_horizontalTexture;
}

TerrainControl::Tool TerrainControl::tool() const
{
    return m_tool;
}

bool TerrainControl::blankTexture() const
{
    return m_blankTexture;
}

void TerrainControl::reloadMaterials()
{
    QFileInfo info;
    QStandardItem * item;

    m_textureModel->clear();
    for(int i=0; i<terrain->materialCount();i++)
    {
        TerrainMaterial * material = terrain->getMaterial(i);
        if(material !=NULL && material->enabled())
        {
            info.setFile(material->texture_path.c_str());
            item=new QStandardItem;
            QUrl url(QUrl::fromLocalFile(info.absoluteFilePath()));
            item->setData(material->name.c_str(),TerrainControl::TitleRole);
            item->setData(url.url(),TerrainControl::PathRole);
            item->setData(material->id(),TerrainControl::TextureIdRole);
            m_textureModel->appendRow(item);
        }
    }
    /*
    item=m_textureModel->item(0);
    if(item!=NULL)
    {
        texture_v=item->data(TextureIdRole).toInt();
        texture_h=texture_v;
    }*/
    emit textureModelChanged();
}

void TerrainControl::fillGround(float height)
{
    if(terrain!=NULL)
    {
        fillHeight(height);
    }
}

void TerrainControl::clearTerrain()
{
    if(terrain!=NULL)
    {
        terrain->clear();
    }
}

void TerrainControl::rebuildTerrain()
{
    if(terrain!=NULL)
        terrain->rebuid(true);
}

void TerrainControl::updateMaterial(int id)
{
    TerrainMaterial * material=terrain->getMaterial(id);
    if(material!=NULL)
    {
        QFileInfo info(material->texture_path.c_str());
        QStandardItem * item=NULL;
        QUrl url(QUrl::fromLocalFile(info.absoluteFilePath()));

        for(int i=0;i<m_textureModel->rowCount();i++)
        {
            if(m_textureModel->item(i)->data(TerrainControl::TextureIdRole).toInt()==id)
            {
                item=m_textureModel->item(i);
                break;
            }
        }

        if(material->enabled())
        {
            if(item==NULL)
            {
                item=new QStandardItem;
                item->setData(material->name.c_str(),TerrainControl::TitleRole);
                item->setData(url.url(),TerrainControl::PathRole);
                item->setData(material->id(),TerrainControl::TextureIdRole);
                m_textureModel->appendRow(item);
            }
            else
            {
                item->setData(material->name.c_str(),TerrainControl::TitleRole);
                item->setData(url.url(),TerrainControl::PathRole);
            }
        }
        else
        {
            if(item!=NULL)  m_textureModel->removeRow(item->row());

        }
    }
    emit textureModelChanged();
}

void TerrainControl::setTextureModel(QStandardItemModel *arg)
{
    if (m_textureModel != arg) {
        m_textureModel = arg;
        emit textureModelChanged();
    }
}

void TerrainControl::setPointerSize(float arg)
{
    if (m_pointerSize != arg) {
        m_pointerSize = arg;

        if(navig->minRadius()>navig->maxRadius()*m_pointerSize)
            navig->setRadius(navig->minRadius());
        else
            navig->setRadius(navig->maxRadius()*m_pointerSize);

        emit pointerSizeChanged(arg);
    }
}

void TerrainControl::setPointerOffset(float arg)
{
    if (m_pointerOffset != arg) {
        m_pointerOffset = arg;
        float value=m_pointerOffset;
        value-=0.5;
        value*=2.0;
        navig->setOffset(value);

        emit pointerOffsetChanged(arg);
    }
}

void TerrainControl::setMaterialOpacity(float arg)
{
    if (m_materialOpacity != arg) {
        m_materialOpacity = arg;
        emit materialOpacityChanged(arg);
    }
}

void TerrainControl::setMaterialFade(float arg)
{
    if (m_materialFade != arg) {
        m_materialFade = arg;
        emit materialFadeChanged(arg);
    }
}

void TerrainControl::setLockX(bool arg)
{
    if (m_lockX != arg) {
        m_lockX = arg;
        navig->lockX(m_lockX);
        emit lockXChanged(arg);
    }
}

void TerrainControl::setLockY(bool arg)
{
    if (m_lockY != arg) {
        m_lockY = arg;
        navig->lockY(m_lockY);
        emit lockYChanged(arg);
    }
}

void TerrainControl::setLockZ(bool arg)
{
    if (m_lockZ != arg) {
        m_lockZ = arg;
        navig->lockZ(m_lockZ);
        emit lockZChanged(arg);
    }
}

void TerrainControl::setPointerShape(TerrainControl::PointerShape arg)
{
    if (m_pointerShape != arg) {
        m_pointerShape = arg;

        if(m_pointerShape==SPHERE)
            navig->setForm(Shape::SPHERE);
        else if(m_pointerShape==BOX)
            navig->setForm(Shape::BOX);

        emit pointerShapeChanged(arg);
    }
}

void TerrainControl::setPointerType(TerrainControl::PointerType arg)
{
    if (m_pointerType != arg) {
        m_pointerType = arg;

        if(m_pointerType==MAGNET)
            navig->setMode(Instrument::MAGNET);
        else if(m_pointerType==POINTER)
            navig->setMode(Instrument::POINTER);

        emit pointerTypeChanged(arg);
    }
}

void TerrainControl::setVerticalTexture(int arg)
{
    if (m_verticalTexture != arg) {
        m_verticalTexture = arg;

        QStandardItem * item=m_textureModel->item(m_verticalTexture);
        if(item!=NULL)
            texture_v=item->data(TerrainControl::TextureIdRole).toInt();

        emit verticalTextureChanged(arg);
    }
}

void TerrainControl::setHorizontalTexture(int arg)
{
    if (m_horizontalTexture != arg) {
        m_horizontalTexture = arg;

        QStandardItem * item=m_textureModel->item(m_horizontalTexture);
        if(item!=NULL)
            texture_h=item->data(TerrainControl::TextureIdRole).toInt();

        emit horizontalTextureChanged(arg);
    }
}

void TerrainControl::setTool(TerrainControl::Tool arg)
{
    if (m_tool != arg) {
        m_tool = arg;
        if(tools[m_tool]!=NULL)
            instrument=tools[m_tool];
        emit toolChanged(arg);
    }
}

void TerrainControl::setBlankTexture(bool arg)
{
    if (m_blankTexture != arg) {
        m_blankTexture = arg;
        emit blankTextureChanged(arg);
    }
}
