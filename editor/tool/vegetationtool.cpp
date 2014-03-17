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
#include "vegetationtool.h"
#include "ui_vegetationtool.h"

VegetationTool::VegetationTool(QWidget *parent) :
    EditorTool(parent),
    ui(new Ui::VegetationTool)
{
    ui->setupUi(this);
    tool=PLANT;
}

VegetationTool::~VegetationTool()
{
    delete ui;
}

void VegetationTool::setup(Canvas *, Navigator *navig, Assets *assets)
{
    this->navig=navig;
    this->assets=assets;
    navig->show();
    navig->setMode(Navigator::MAGNET);
}

void VegetationTool::on_grassPlant_clicked()
{
    setTool(VegetationTool::PLANT);
}

void VegetationTool::on_grassDig_clicked()
{
    setTool(VegetationTool::DIG);
}

void VegetationTool::grassTexture()
{
    DialogGrassTexture dialog(base,base_io,assets,this);
    dialog.exec();
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
    }
}

QString VegetationTool::getName()
{
    return "Grass";
}

void VegetationTool::setTool(VegetationTool::Tool tool)
{
    this->tool=tool;
}

void VegetationTool::make(float x, float y, float z)
{
    Vertex3f start,end;
    Vertex3f pos;
    float radius=navig->getRadius();

    for(int i=-radius;i<=floor(radius);i++)
        for(int j=-radius;j<=floor(radius);j++)
            {
                float a=i;//+x-int(x);
                float b=j;//+z-int(z);
                float r=a*a+b*b;
                r=sqrt(r);

                if(r<radius)
                {
                    start.set(i+x,y+radius,j+z);
                    end.set(i+x,y-radius,j+z);
                    pos=map->collision(start,end).collision;
                    if(pos!=end)
                    {
                        if(tool==DIG)
                            base->removeTuft(pos);
                        else
                        {
                            float val=noise.octave2d(1,1,1,0,i,j);
                            val=(val+1)/2;
                            //std::cout<<val<<std::endl;
                            if(val>0.5)
                            {
                                base->addTuft(pos);
                            }
                        }
                    }
                }

        }
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

void VegetationTool::loadProject(Project *project)
{
    this->map=&project->map;
    this->base=&project->grass;
    this->base_io=&project->grass_io;
}

void VegetationTool::unloadProject()
{
}

void VegetationTool::onEnter()
{
    navig->show();
    navig->setRadius(3);
    navig->setMode(Navigator::MAGNET);
}

void VegetationTool::plant(const Shape *shape)
{
    Vertex3f pos=shape->getPosition(),start,end;
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
    for(int i=0;i<base->getTuftCount();i++)
    {
        if(shape->isovalue(base->getTuft(i))<0)
        {
            base->removeTuft(i);
            i--;//compensate delete shift
        }
    }
}

QMenu * VegetationTool::createMenu()
{
    QMenu * menu=new QMenu(getName(),this);
    QAction * action;
    action=new QAction(tr("Grass texture"),menu);
    connect(action,SIGNAL(triggered()),this,SLOT(grassTexture()));
    menu->addAction(action);

    return menu;
}
