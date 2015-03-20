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

#include "toolcontrol.h"

ToolControl::ToolControl(QQuickItem *parent) :
    QQuickItem(parent),m_controlPanel(NULL)
{
}

void ToolControl::uiSetup(QQuickItem *UNUSED(ui))
{
}

void ToolControl::uiTabSetup(QQuickItem *UNUSED(tab))
{

}

void ToolControl::undo()
{
}

void ToolControl::redo()
{
}

void ToolControl::mousePress(QMouseEvent * UNUSED(event))
{
}

void ToolControl::mouseRelease(QMouseEvent *UNUSED(event))
{ 
}

void ToolControl::mouseMove(QMouseEvent *UNUSED(event))
{
}

void ToolControl::mouseDoubleClick(QMouseEvent *UNUSED(event))
{
}

void ToolControl::keyPress(QKeyEvent *UNUSED(event))
{
}

void ToolControl::keyRelease(QKeyEvent *UNUSED(event))
{
}

void ToolControl::wheel(QWheelEvent *UNUSED(event))
{
}

void ToolControl::onEnter()
{
}

void ToolControl::onLeave()
{
}

void ToolControl::loadProject(Project *project)
{
}

void ToolControl::unloadProject()
{
}

void ToolControl::update(int UNUSED(elapsed_time), const Player &UNUSED(player))
{
}

QString ToolControl::title() const
{
    return m_title;
}

QQmlComponent *ToolControl::controlPanel() const
{
    return m_controlPanel;
}


void ToolControl::setTitle(const QString& arg)
{
    if (m_title != arg) {
        m_title = arg;
        emit titleChanged(arg);
    }
}

void ToolControl::setControlPanel(QQmlComponent *arg)
{
    if (m_controlPanel != arg) {
        m_controlPanel = arg;
        emit controlPanelChanged(arg);
    }
}
