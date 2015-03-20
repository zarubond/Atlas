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

#include "undostack.h"

UndoStack::UndoStack()
{
    limit=0;
    position=0;
    commands=NULL;
}

UndoStack::~UndoStack()
{
    for(int i=0;i<limit;i++)
        delete commands[i];
    delete [] commands;
}

void UndoStack::setUndoLimit(int limit)
{
    this->limit=limit;
    UndoCommand** tmp =new UndoCommand*[limit];
    if(commands==NULL)
    {
        commands=tmp;
        for(int i=0;i<limit;i++)
            commands[i]=NULL;
    }
}

void UndoStack::push(UndoCommand *command)
{
    delete commands[position];
    commands[position++]=command;
    position=position%limit;
}

void UndoStack::undo()
{
    int p=position-1;//%limit, problem with c++ modulo implementation
    if(p<0) p=limit-1;
    UndoCommand * tmp=commands[p];
    if(tmp!=NULL)
    {
        tmp->undo();
        position=p;
    }
}

void UndoStack::redo()
{
    UndoCommand * tmp=commands[position];
    if(tmp!=NULL)
    {
        tmp->undo();
        position++;
        position=position%limit;
    }
}

void UndoStack::clear()
{
    for(int i=0;i<limit;i++)
    {
        delete commands[i];
        commands[i]=NULL;
    }
}
