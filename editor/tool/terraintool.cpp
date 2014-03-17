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
#include "terraintool.h"
#include "ui_terraintool.h"

TerrainTool::TerrainTool(QWidget *parent) :
    EditorTool(parent),
    ui(new Ui::TerrainTool)
{
    ui->setupUi(this);

    terrain=NULL;
    texture_vertical=0;
    texture_horizontal=0;

    for(int i=0;i<10;i++)
        tools[i]=NULL;

    tools[SMOOTH]   = new TerrainActionSmooth(this);
    tools[SCULPTOR] = new TerrainActionSculptor(this);
    tools[ERASER]   = new TerrainActionEraser(this);
    tools[INCREASE] = new TerrainActionIncrease(this);
    tools[BRUSH]    = new TerrainActionBrush(this);
    tools[DECREASE] = new TerrainActionDecrease(this);

    tool=tools[SCULPTOR];

    left_press=false;

    undo_stack.setUndoLimit(15);
}

TerrainTool::~TerrainTool()
{
    delete ui;
    for(int i=0;i<10;i++)
        delete tools[i];
}

void TerrainTool::setup(Canvas *space, Navigator *navig, Assets *assets)
{
    this->canvas=space;
    this->navig=navig;
    this->assets=assets;
}

void TerrainTool::undo()
{
    undo_stack.undo();
    terrain->rebuid();
}

void TerrainTool::redo()
{
    undo_stack.redo();
}

void TerrainTool::setTool(TerrainTool::Tool tool)
{
    if(tools[tool]!=NULL)
        this->tool=tools[tool];
}

TerrainGraphics *TerrainTool::getTerrain() const
{
    return terrain;
}

void TerrainTool::fillHeight(float height)
{
    for(int x=0;x<terrain->regionSize()*terrain->regionsX();x++)
        for(int y=0;y<ceil(height);y++)
            for(int z=0;z<terrain->regionsZ()*terrain->regionSize();z++)
            {
              //  terrain->setVoxel(x,y,z,value,0);
            }
    terrain->rebuid();
}

void TerrainTool::stroke(const Shape *shape)
{
    if(terrain!=NULL)
    {
        TerrainUndo * und=new TerrainUndo(terrain);
        tool->stroke(und,shape);
        undo_stack.push(und);
        terrain->rebuid();
    }
}

void TerrainTool::mousePress(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        Shape shape;
        shape.setPosition(navig->getX(),navig->getY(),navig->getZ());
        shape.setRadius(navig->getRadius());
        if(left_press==false)
        {
            stroke(navig);
            left_press=true;
            last_time+=100;
        }
    }
}

void TerrainTool::mouseRelease(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        left_press=false;
    }
}

void TerrainTool::mouseMove(QMouseEvent *event)
{
}

void TerrainTool::keyPress(QKeyEvent *event)
{
}

void TerrainTool::update(int elapsed_time)
{
    if(left_press)
    {
        if(elapsed_time-last_time>100)
        {
            last_time=elapsed_time;
            stroke(navig);
        }
    }
    else
        last_time=elapsed_time;
}

void TerrainTool::wheel(QWheelEvent *event)
{
}

void TerrainTool::onEnter()
{
    navig->show();
   // navig->setRadius(1);
    navig->setMode(Navigator::POINTER);
}

void TerrainTool::onLeave()
{
}

void TerrainTool::loadProject(Project *project)
{
    terrain=&project->terrain;
    terrain_io=&project->terrain_io;
    map=&project->map;
    QFileInfo info;

    ui->comboHorizontal->setIconSize(QSize(50,50));
    ui->comboVertical->setIconSize(QSize(50,50));
    for(int i=0;i<8;i++)
    {
        if(terrain_io->getTexturePath(i)!=NULL && !terrain_io->getTexturePath(i)->isEmpty())
        {
            info.setFile(*terrain_io->getTexturePath(i));
            ui->comboHorizontal->addItem(QIcon(QPixmap::fromImage(QImage(*terrain_io->getTexturePath(i)))),info.baseName(),QVariant(i+1));
            ui->comboVertical->addItem(QIcon(QPixmap::fromImage(QImage(*terrain_io->getTexturePath(i)))),info.baseName(),QVariant(i+1));
        }
    }
    ui->comboHorizontal->addItem("Blank",QVariant(0));
    ui->comboVertical->addItem("Blank",QVariant(0));

    connect(terrain_io,SIGNAL(newMaterial(int,QImage&)),this,SLOT(newMaterial(int,QImage&)));
}

void TerrainTool::unloadProject()
{
    ui->comboHorizontal->clear();
    ui->comboVertical->clear();
    map=NULL;
    terrain=NULL;
    undo_stack.clear();
}

QMenu *TerrainTool::createMenu()
{
    QMenu * menu=new QMenu(getName(),this);
    QAction * action;
    action=new QAction(tr("Materials"),menu);
    connect(action,SIGNAL(triggered()),this,SLOT(openMaterialDialog()));
    menu->addAction(action);

    action=new QAction(tr("Procedural"),menu);
    connect(action,SIGNAL(triggered()),this,SLOT(openProceduralDialog()));
    menu->addAction(action);

    action=new QAction(tr("Fill Ground"),menu);
    connect(action,SIGNAL(triggered()),this,SLOT(action_FillGround()));
    menu->addAction(action);

    action=new QAction(tr("Import Terrain"),menu);
    connect(action,SIGNAL(triggered()),this,SLOT(action_Import()));
    menu->addAction(action);

    action=new QAction(tr("Clear Terrain"),menu);
    connect(action,SIGNAL(triggered()),this,SLOT(action_ClearAll()));
    menu->addAction(action);

    action=new QAction(tr("Rebuild"),menu);
    connect(action,SIGNAL(triggered()),this,SLOT(action_Rebuild()));
    menu->addAction(action);

    return menu;
}

QString TerrainTool::getName()
{
    return tr("Terrain");
}

void TerrainTool::setTexture(int id, QImage &texture)
{
    if(id>0 && id<=terrain->maxTextures())
    {
        ui->comboHorizontal->insertItem(id-1,QIcon(QPixmap::fromImage(texture)),"??",QVariant(id));
        ui->comboVertical->insertItem(id-1,QIcon(QPixmap::fromImage(texture)),"??",QVariant(id));
    }
}

unsigned char TerrainTool::currentMaterial() const
{
    return terrain->materialId(texture_vertical,texture_horizontal);
}

void TerrainTool::openMaterialDialog()
{
    DialogTerrainMaterial dialog(terrain,terrain_io,assets,this);
    dialog.exec();
}

void TerrainTool::openProceduralDialog()
{
    DialogTerrainProcedural dialog(terrain_io,this);
    dialog.exec();
}

void TerrainTool::action_FillGround()
{
    //hact 100
    bool ok;
    float height = QInputDialog::getDouble(this, tr("Fill Ground"),
                                 tr("Height:"), 1, 1, 100, 1, &ok);
    if (ok)
    {
        //editor.terrain.fillHeight(height);
    }
}

void TerrainTool::action_Import()
{
    DialogTerrainImport dialog(this);
    dialog.exec();
}

void TerrainTool::action_ClearAll()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Clear terrain"));
    msgBox.setText(tr("Do you realy want to clear all terrain?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret=msgBox.exec();
    if(ret==QMessageBox::Yes && terrain!=NULL)
    {
        terrain->clear();
    }
}

void TerrainTool::action_Rebuild()
{
    if(terrain!=NULL)
        terrain->rebuid(true);
}

void TerrainTool::newMaterial(int id, QImage &img)
{
    setTexture(id,img);
}

void TerrainTool::on_toolSculptor_clicked()
{
    setTool(TerrainTool::SCULPTOR);
}

void TerrainTool::on_toolEraces_clicked()
{
    setTool(TerrainTool::ERASER);
}

void TerrainTool::on_toolBrush_clicked()
{
    setTool(TerrainTool::BRUSH);
}

void TerrainTool::on_toolSmooth_clicked()
{
    setTool(TerrainTool::SMOOTH);
}

void TerrainTool::on_toolIncreser_clicked()
{
    setTool(TerrainTool::INCREASE);
}

void TerrainTool::on_toolDecreser_clicked()
{
    setTool(TerrainTool::DECREASE);
}

void TerrainTool::on_handTypeMagnet_clicked()
{
    navig->setMode(Navigator::MAGNET);
}

void TerrainTool::on_handTypePointer_clicked()
{
    navig->setMode(Navigator::POINTER);
}

void TerrainTool::on_lockX_clicked()
{
    navig->lockX();
}

void TerrainTool::on_lockY_clicked()
{
    navig->lockY();
}

void TerrainTool::on_lockZ_clicked()
{
    navig->lockZ();
}

void TerrainTool::on_handShapeSphere_clicked()
{
   navig->setForm(Shape::SPHERE);
}

void TerrainTool::on_handShapeBox_clicked()
{
    navig->setForm(Shape::BOX);
}

void TerrainTool::on_comboVertical_currentIndexChanged(int index)
{
    QVariant var=ui->comboVertical->itemData(index);
    texture_vertical=var.toInt();
}

void TerrainTool::on_comboHorizontal_currentIndexChanged(int index)
{
    QVariant var=ui->comboHorizontal->itemData(index);
    texture_horizontal=var.toInt();
}
