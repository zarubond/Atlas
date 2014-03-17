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
#include "grassgarden.h"

#include "grassgraphics.h"

GrassGarden::GrassGarden(GrassGraphics *base): base(base)
{
    tuft_count=0;
}

void GrassGarden::addTuft(const Vertex3f &tuft)
{
    for(unsigned int i=0;i<tuft_count;i++)
    {
        if(int(tuft_list[i][0])==int(tuft[0]) && int(tuft_list[i][2])==int(tuft[2]))
            return;
    }
    tuft_list[tuft_count++]=tuft;
}
