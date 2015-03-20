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

#include "grassregion.h"

#include "grassgraphics.h"

GrassRegion::GrassRegion(GrassGraphics *parent,int x,int y,int z,int size):
    parent(parent),pos_x(x),pos_y(y),pos_z(z),size(size)
{
    tuft_list=new Vector3f[1024];
    tuft_count=0;
}

GrassRegion::~GrassRegion()
{
    delete [] tuft_list;
}

void GrassRegion::addTuft(const Vector3f & tuft)
{
    for(int i=0;i<tuft_count;i++)
    {
       if(int(tuft_list[i][0])==int(tuft[0]) && int(tuft_list[i][2])==int(tuft[2]))
            return;
    }
    tuft_list[tuft_count++]=tuft;//Vertex3f(int(tuft[0]),int(tuft[1]),int(tuft[2]));
}

bool GrassRegion::removeTuft(const Vector3f & tuft)
{
    for(int i=0;i<tuft_count;i++)
    {
        if(int(tuft_list[i][0])==int(tuft[0]) && int(tuft_list[i][2])==int(tuft[2]))
        {
            for(int j=i;j<tuft_count-1;j++)
                tuft_list[i]=tuft_list[i+1];
            tuft_count--;
            return true;
        }
    }
    return false;
}

bool GrassRegion::removeTuft(int x, int z, float min_y, float max_y)
{
    for(int i=0;i<tuft_count;i++)
    {
        if(int(tuft_list[i][0])==int(x) && int(tuft_list[i][2])==int(z) && tuft_list[i][1]>=min_y && tuft_list[i][1]<=max_y)
        {
            for(int j=i;j<tuft_count-1;j++)
                tuft_list[i]=tuft_list[i+1];
            tuft_count--;
            return true;
        }
    }
    return false;
}

bool GrassRegion::removeTuft(int id)
{
    if(id >= tuft_count) return false;

    for(int i=id; i < tuft_count-1; i++)
        tuft_list[i]=tuft_list[i+1];

    tuft_count--;

    return true;
}

bool GrassRegion::inBox(int x, int y, int z, int w) const
{
    if(pos_x>=x && pos_x+size<=x+w && pos_y>=y && pos_y+size<=y+w && pos_z>=z && pos_z+size<=z+w)
        return true;
    return false;
}

Vector3f GrassRegion::getTuft(unsigned id) const
{
    return tuft_list[id];
}

const Vector3f *GrassRegion::tuftList() const
{
    return tuft_list;
}

int GrassRegion::tuftCount() const
{
    return tuft_count;
}

void GrassRegion::selectDraw(const Camera &)
{
    parent->pushRender(this);
}

void GrassRegion::selectDraw()
{
    parent->pushRender(this);
}

int GrassRegion::type()
{
    return 0;
}
