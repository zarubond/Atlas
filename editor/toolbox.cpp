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

#include "toolbox.h"

Toolbox::Toolbox():
    map(NULL),tool(NULL),tool_count(0),mouse_x(0),mouse_y(0)
{
    for(int i=0;i<10;i++)
        tools[i]=NULL;
    navig_dist=3.0;
}

Toolbox::~Toolbox()
{
    for(int i=0;i<10;i++)
        delete tools[i];
}

void Toolbox::selectTool(int tool_id)
{
    if(tool_id>=0 && tool_id<tool_count)
    {
        if(this->tool!=NULL)
            this->tool->onLeave();

        this->tool=tools[tool_id];
        this->tool->onEnter();
    }
}

void Toolbox::setProject(Project * project)
{
    for(int i=0;i<10;i++)
    {
        if(tools[i]!=NULL)
            tools[i]->loadProject(project);
    }

    project->world.editor.navig=&intrument;

    this->map=&project->world.map;

    if(tools[0]!=NULL)
    {
        tool=tools[0];
        tool->onEnter();
    }
}

void Toolbox::unloadProject()
{
    for(int i=0;i<10;i++)
    {
        if(tools[i]!=NULL)
            tools[i]->unloadProject();
    }

    this->map=NULL;
}

ToolControl * Toolbox::getTool(int tool_id)
{
    if(tool_id>=0 && tool_id<tool_count)
    {
        return tools[tool_id];
    }
    return NULL;
}

void Toolbox::undo()
{
    if(tool!=NULL)
        tool->undo();
}

void Toolbox::redo()
{
    if(tool!=NULL)
        tool->redo();
}

void Toolbox::addTool(ToolControl *tool)
{
    tools[tool_count++]=tool;
    tool->setup(&intrument);
    if(this->tool==NULL)
        this->tool=tool;
}

void Toolbox::wheelEvent(QWheelEvent *event)
{
    Qt::KeyboardModifiers keyMod = QGuiApplication::keyboardModifiers ();
   // bool isSHIFT = keyMod.testFlag(Qt::ShiftModifier);
    //bool isCTRL = keyMod.testFlag(Qt::ControlModifier);
    if(keyMod.testFlag(Qt::ControlModifier))
    {
        float tmp=intrument.getRadius()+event->delta()*0.005;
        intrument.setRadius(tmp);
    }
    else
    {
        float tmp=navig_dist+event->delta()*0.01;
        if(tmp>1 && tmp<500)
        {
            navig_dist=tmp;
            intrument.setDistance(navig_dist);
            //move=true;
        }
    }

    tool->wheel(event);
}

void Toolbox::mouseMoveEvent(QMouseEvent *e)
{
    mouse_x=e->x();
    mouse_y=e->y();
    intrument.setMouse(mouse_x, mouse_y);
    if(tool!=NULL)
        tool->mouseMove(e);
}

void Toolbox::mousePressEvent(QMouseEvent *e)
{
    mouse_x=e->x();
    mouse_y=e->y();
    intrument.setMouse(mouse_x, mouse_y);
    if(tool!=NULL)
        tool->mousePress(e);
}

void Toolbox::mouseReleaseEvent(QMouseEvent * e)
{
    if(tool!=NULL)
        tool->mouseRelease(e);
}

void Toolbox::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(tool!=NULL)
        tool->mouseDoubleClick(event);
}

void Toolbox::update(int elapsed_time, Player & player)
{
    if(tool!=NULL && map!=NULL)
    {
        intrument.setMouse(mouse_x, mouse_y);
        intrument.updatePosition(player.getCamera(),map);
        tool->update(elapsed_time, player);
    }
}

void Toolbox::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_Plus:
    {
        float tmp=navig_dist+0.3;
        if(tmp>1 && tmp<500)
        {
            intrument.setDistance(tmp);
            navig_dist=tmp;
        }

        break;
    }
    case Qt::Key_Minus:
    {
        float tmp=navig_dist-0.3;
        if(tmp>1 && tmp<500)
        {
            intrument.setDistance(tmp);
            navig_dist=tmp;
        }

        break;
    }
    default:
        break;
    }

    if(tool!=NULL)
        tool->keyPress(e);
}

void Toolbox::keyReleaseEvent(QKeyEvent *e)
{
    if(tool!=NULL)
        tool->keyRelease(e);
}

void Toolbox::hoverMoveEvent(QHoverEvent *event)
{
    mouse_x=event->pos().x();
    mouse_y=event->pos().y();
    intrument.setMouse(event->pos().x(),event->pos().y());
}
