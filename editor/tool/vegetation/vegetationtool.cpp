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

#include "vegetationtool.h"

VegetationTool::VegetationTool(QQuickItem *parent) :
    ToolControl(parent)
{
    tool=PLANT;
    left_press=false;
}

VegetationTool::~VegetationTool()
{
}

void VegetationTool::setup(Instrument *navig)
{
    this->navig=navig;
    navig->show();
    navig->setMode(Instrument::MAGNET);
}

const char *VegetationTool::uiFile()
{
    return "tools/VegetationTool.qml";
}

void VegetationTool::uiSetup(QQuickItem *ui)
{
    this->ui=ui;
    connect(ui,SIGNAL(grassPlant())   ,this,SLOT(grassPlant()));
    connect(ui,SIGNAL(grassDig())     ,this,SLOT(grassDig()));
    connect(ui,SIGNAL(openTextureDialog()),this,SLOT(openTextureDialog()));
    connect(ui,SIGNAL(pointerSize(float)),this,SLOT(pointerSize(float)));
}

void VegetationTool::onEnter()
{
    ui->setProperty("pointer_size",navig->getRadius()/navig->maxRadius());
    orig_offset=navig->getOffset();
    navig->setOffset(0);

    navig->show();
    navig->setMode(Instrument::MAGNET);
}

void VegetationTool::onLeave()
{
    navig->setOffset(orig_offset);
}

void VegetationTool::grassPlant()
{
    setTool(VegetationTool::PLANT);
}

void VegetationTool::grassDig()
{
    setTool(VegetationTool::DIG);
}

void VegetationTool::pointerSize(float value)
{
    float rad=navig->maxRadius()*value;
    navig->setRadius(rad);
}

void VegetationTool::openTextureDialog()
{
    DialogGrassTexture dialog(base,base_io,assets);
    //dialog.exec();
}

void VegetationTool::mousePress(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        /*Shape shape;
        shape.setPosition(navig->getX(),navig->getY(),navig->getZ());
        shape.setRadius(navig->getRadius());
        stroke(&shape);*/
        stroke(navig);
        left_press=true;
    }
}

void VegetationTool::mouseRelease(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        left_press=false;
    }
}

void VegetationTool::mouseMove(QMouseEvent *event)
{
    if(left_press)
    {
        stroke(navig);
    }
}

void VegetationTool::setTool(VegetationTool::Tool tool)
{
    this->tool=tool;
}

void VegetationTool::stroke(const Shape *shape)
{
    switch(tool)
    {
    case PLANT:
        plant(shape);
        break;
    case DIG:
        dig(shape);
        break;
    }
}

void VegetationTool::undo()
{
}

void VegetationTool::redo()
{

}

void VegetationTool::loadProject(Project *project)
{
    map=&project->world.map;
    base=&project->world.grass;
    //base_io=&project->io->grass_io;
}

void VegetationTool::unloadProject()
{
}

void VegetationTool::plant(const Shape *shape)
{
    Vector3f pos=shape->getPosition(),start,end;
    float radius=shape->getRadius();
    int y=pos[1];
    for(int x=shape->minX(); x < shape->maxX();x++)
        for(int z=shape->minZ(); z < shape->maxZ();z++)
        {
            if(shape->isovalue(x,y,z))
            {
                start.set(x,y+radius,z);
                end.set(x,y-radius,z);
                pos=map->collision(start,end).collision;
                if(shape->isovalue(pos[0],pos[1],pos[2])<0)
                {
                   /* float val=noise.octave2d(1,1,1,0,x,z);
                    val=(val+1)/2;
                    if(val>0.5)
                    {*/
                        base->addTuft(pos);
                    //}
                }
            }
        }
}

void VegetationTool::dig(const Shape *shape)
{
    GrassRegion * region;
    for(int x=shape->minX(); int(x/map->nodeSize())*map->nodeSize() < shape->maxX(); x+=map->nodeSize())
        for(int y=shape->minY(); int(y/map->nodeSize())*map->nodeSize() < shape->maxY(); y+=map->nodeSize())
            for(int z=shape->minZ(); int(z/map->nodeSize())*map->nodeSize() < shape->maxZ(); z+=map->nodeSize())
            {
                region=base->getRegion(x/map->nodeSize(),y/map->nodeSize(),z/map->nodeSize());
                if(region!=NULL)
                {
                    removeTufts(region,shape);
                }
            }
}

bool VegetationTool::removeTufts(GrassRegion *region, const Shape * shape)
{

    for(int i=0; i < region->tuftCount(); i++)
    {
        if(shape->isovalue(region->getTuft(i))<0)
        {
            if(region->removeTuft(i)) i--;
        }
    }

    return false;
}
