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
#include "grassbase.h"

GrassBase::GrassBase(int type):ModuleBase(type)
{
    tuft_count=0;
    tuft_list=new Vertex3f[10024];
}

GrassBase::~GrassBase()
{
    delete [] tuft_list;
}

void GrassBase::addTuft(const Vertex3f &position)
{
    for(unsigned int i=0;i<tuft_count;i++)
    {
        if(int(tuft_list[i][0])==int(position[0]) && int(tuft_list[i][2])==int(position[2]))
            return;
    }
    tuft_list[tuft_count++]=position;
}


bool GrassBase::removeTuft(const Vertex3f &position)
{
    for(unsigned int i=0;i<tuft_count;i++)
    {
        if(int(tuft_list[i][0])==int(position[0]) && int(tuft_list[i][2])==int(position[2]))
        {
            for(unsigned int j=i;j<tuft_count;j++)
            {
                tuft_list[j]=tuft_list[j+1];
            }
            tuft_count--;
            return true;
        }
    }
    return false;
}

bool GrassBase::removeTuft(unsigned id)
{
    for(unsigned int j=id;j<tuft_count-1;j++)
    {
        tuft_list[j]=tuft_list[j+1];
    }

    if(id < tuft_count)
    {
        tuft_count--;
        return true;
    }
    return false;
}

Vertex3f GrassBase::getTuft(unsigned id) const
{
    return tuft_list[id];
}

const Vertex3f *GrassBase::tuftList() const
{
    return tuft_list;
}

int GrassBase::tuftCount() const
{
    return tuft_count;
}

int GrassBase::getTuftCount() const
{
    return tuft_count;
}
