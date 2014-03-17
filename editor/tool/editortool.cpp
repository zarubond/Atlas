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
#include "editortool.h"

EditorTool::EditorTool(QWidget *parent) :
    QWidget(parent)
{
}

QMenu *EditorTool::createMenu()
{
    return NULL;
}

void EditorTool::undo()
{
}

void EditorTool::redo()
{
}

void EditorTool::mousePress(QMouseEvent * event)
{
}

void EditorTool::mouseRelease(QMouseEvent *event)
{ 
}

void EditorTool::mouseMove(QMouseEvent *event)
{
}

void EditorTool::keyPress(QKeyEvent *event)
{
}

void EditorTool::keyRelease(QKeyEvent *event)
{
}

void EditorTool::wheel(QWheelEvent *event)
{
}

void EditorTool::onEnter()
{
}

void EditorTool::onLeave()
{
}

void EditorTool::loadProject(Project *project)
{
}

void EditorTool::unloadProject()
{
}

void EditorTool::update(int elapsed_time)
{
}
