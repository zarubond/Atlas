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
#include "editor.h"

Editor::Editor()
{
    tool=NULL;
    for(int i=0;i<10;i++)
        tools[i]=NULL;
    navig_dist=3.0;
}

void Editor::init(Canvas *canvas)
{
    this->canvas=canvas;
    canvas->setCamera(player.getCamera());
    navig.init();
    navig.setPosition(canvas->getCamera()->getPosition());
    canvas->setNavigator(&navig);

    tool_count=0;

    tools[tool_count++]=new TerrainTool;
    tools[tool_count++]=new ModelTool;
    tools[tool_count++]=new VegetationTool;

    for(int i=0;i<tool_count;i++)
        tools[i]->setup(canvas,&navig,&assets);

    selectTool(0);

    canvas->installEventFilter(this);
    if(Settings::contains("assets/file"))
    {
        QString file=Settings::getString("assets/file");
        if(file.isEmpty() || !assets.load(file))
        {
            std::cerr<<"Unable to load assets"<<std::endl;
        }
    }
    driver.setPlayer(&player);
}

void Editor::selectTool(int tool_id)
{
    if(tool_id>=0 && tool_id<tool_count)
    {
        if(this->tool!=NULL)
            this->tool->onLeave();

        this->tool=tools[tool_id];
        this->tool->onEnter();
    }
}

void Editor::setProject(Project * project)
{
    for(int i=0;i<10;i++)
    {
        if(tools[i]!=NULL)
            tools[i]->loadProject(project);
    }

    this->map=&project->map;

    driver.setMap(&project->map,canvas);
}

void Editor::unsetProject()
{
    for(int i=0;i<10;i++)
    {
        if(tools[i]!=NULL)
            tools[i]->unloadProject();
    }

    this->map=NULL;
}

int Editor::toolCount()
{
    return tool_count;
}

EditorTool * Editor::getTool(int tool_id)
{
    if(tool_id>=0 && tool_id<tool_count)
    {
        return tools[tool_id];
    }
    return NULL;
}

void Editor::undo()
{
    if(tool!=NULL)
        tool->undo();
}

void Editor::redo()
{
    if(tool!=NULL)
        tool->redo();
}

void Editor::update(int elapsed_time)
{
    if(tool!=NULL)
    {
        driver.update(elapsed_time);
        navig.updatePosition(canvas->getCamera(),map);
        tool->update(elapsed_time);
    }
}

void Editor::wheelEvent(QWheelEvent *event)
{
    Qt::KeyboardModifiers keyMod = QApplication::keyboardModifiers ();
   // bool isSHIFT = keyMod.testFlag(Qt::ShiftModifier);
    //bool isCTRL = keyMod.testFlag(Qt::ControlModifier);
    if(keyMod.testFlag(Qt::ControlModifier))
    {
        float tmp=navig.getRadius()+event->delta()*0.005;

        if(tmp<30 && tmp>0.5)
        {
            navig.setRadius(tmp);
        }
    }
    else
    {
        float tmp=navig_dist+event->delta()*0.01;
        if(tmp>1 && tmp<500)
        {
            navig_dist=tmp;
            navig.setDistance(navig_dist);
            //move=true;
        }
/*
        if(space->navig.getMode()==Navigator::MAGNET)
        {
            Vertex3f tmp_pos(camera->getPosition());
            forward(event->delta()*0.03,tmp_pos);
            camera->translate(tmp_pos);
        }*/
    }

    if(!driver.wheelEvent(event))
        tool->wheel(event);
}

void Editor::mouseMoveEvent(QMouseEvent *e)
{
    driver.mouseMoveEvent(e);
    navig.setMouse(e->x(),e->y());
    //navig.updatePosition(canvas->getCamera(),map,e->x(),e->y());
    tool->mouseMove(e);
}

void Editor::mousePressEvent(QMouseEvent *e)
{
    if(!driver.mousePressEvent(e) && tool!=NULL)
        tool->mousePress(e);
}

void Editor::mouseReleaseEvent(QMouseEvent * e)
{
    if(!driver.mouseReleaseEvent(e) && tool!=NULL)
        tool->mouseRelease(e);
}

bool Editor::eventFilter(QObject *obj, QEvent *event)
{
    switch(event->type())
    {
    case QEvent::MouseMove:
        mouseMoveEvent(static_cast<QMouseEvent *>(event));
        break;
    case QEvent::MouseButtonPress:
        mousePressEvent(static_cast<QMouseEvent *>(event));
        break;
    case QEvent::MouseButtonRelease:
        mouseReleaseEvent(static_cast<QMouseEvent *>(event));
        break;
    case QEvent::Wheel:
        wheelEvent(static_cast<QWheelEvent *>(event));
        break;
    default:
        return QObject::eventFilter(obj,event);
    }

    return true;
}

void Editor::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_X:
        navig.lockX();
        break;
    case Qt::Key_Y:
        navig.lockY();
        break;
    case Qt::Key_Z:
        navig.lockZ();
        break;
    case Qt::Key_Plus:
    {
        float tmp=navig_dist+0.3;
        if(tmp>1 && tmp<500)
        {
            navig.setDistance(tmp);
            navig_dist=tmp;
        }

        break;
    }
    case Qt::Key_Minus:
    {
        float tmp=navig_dist-0.3;
        if(tmp>1 && tmp<500)
        {
            navig.setDistance(tmp);
            navig_dist=tmp;
        }

        break;
    }
    default:
        break;
    }

    if(!driver.keyPressEvent(e) && tool!=NULL)
        tool->keyPress(e);
}

void Editor::keyReleaseEvent(QKeyEvent *e)
{
    if(!driver.keyReleaseEvent(e))
        if(tool!=NULL)
            tool->keyRelease(e);
}

