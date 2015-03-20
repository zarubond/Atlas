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

#include "modelgraphics.h"

ModelGraphics::ModelGraphics(int type):ModelBase(type)
{
    render_count=0;
    hightlight=NULL;
}

void ModelGraphics::pushRender(Model *model)
{
    render_list[render_count++]=model;
}

Model *ModelGraphics::popRender()
{
    return render_list[--render_count];
}

Model **ModelGraphics::renderList(int &count)
{
    count=render_count;
    return render_list;
}

void ModelGraphics::clearRenderList()
{
    render_count=0;
}

int ModelGraphics::renderCount() const
{
    return render_count;
}

void ModelGraphics::highlightModel(Model *model)
{
    hightlight=model;
}

Model *ModelGraphics::hightlighted()
{
    return hightlight;
}
