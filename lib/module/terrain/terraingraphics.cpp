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

#include "terraingraphics.h"

#include "terrainsurface.h"
#include "../../map.h"


TerrainGraphics::TerrainGraphics(int type): TerrainBase(type)
{
    render_count=0;
    rebuild_count=0;

    material_count=4;

    render_list=NULL;
    rebuild_list=NULL;
    maker=NULL;
    for(int i=0; i<8; i++)
        materials[i]=NULL;
}

TerrainGraphics::~TerrainGraphics()
{
    delete [] render_list;
    delete [] rebuild_list;
    for(int i=0; i<maxMaterials(); i++)
        delete materials[i];
    delete maker;
}

void TerrainGraphics::build(int nodes_x, int nodes_y, int nodes_z, int node_size)
{
    this->nodes_x=nodes_x;
    this->nodes_y=nodes_y;
    this->nodes_z=nodes_z;
    this->node_size=node_size;

    grid=new TerrainRegion *[nodes_x*nodes_y*nodes_z];

    for(int i=0;i<nodes_x*nodes_y*nodes_z;i++)
        grid[i]=NULL;

    render_count=0;
    render_list    = new TerrainSurface*[nodes_x*nodes_y*nodes_z];
    rebuild_list   = new TerrainSurface*[nodes_x*nodes_y*nodes_z];
    maker=new TerrainMaker(regionSize());

    texture.create(512,512,maxMaterials(),GL_LINEAR_MIPMAP_LINEAR,GL_REPEAT);
    normalmap.create(512,512,maxMaterials(),GL_LINEAR_MIPMAP_LINEAR,GL_REPEAT);

    for(int i=0; i<maxMaterials(); i++)
        materials[i]=new TerrainMaterial(i,&normalmap,&texture);

}

void TerrainGraphics::rebuid(bool force)
{
    TerrainSurface * tmp;
    mutex.lock();
    for(int i=0;i<nodes_x;i++)
        for(int j=0;j<nodes_y;j++)
            for(int k=0;k<nodes_z;k++)
            {
                tmp=(TerrainSurface*)grid[i*nodes_z*nodes_y +j*nodes_z+k];

                if(tmp!=NULL && (tmp->isDirty() || force))
                {
                    maker->rebuid(tmp);
                    if(tmp->regionType()==TerrainRegion::ABOVEGROUND)
                        removeRegion(tmp);

                    /* TBD
                    else if(tmp->regionType()==TerrainRegion::UNDERGROUND)
                    {
                        for (std::list<TerrainRegion*>::iterator it=region_cache.begin(); it!=region_cache.end(); ++it)
                        {
                            if((*it)==tmp)
                            {
                                region_cache.erase(it);
                                break;
                            }
                        }
                        delete tmp->data_compressed;
                        tmp->data_compressed=NULL;
                        delete tmp->data_grid;
                        tmp->data_grid=NULL;
                        tmp->comressed_size=0;
                    }*/
                }
            }
    mutex.unlock();
}

void TerrainGraphics::pushRender(TerrainSurface *surface)
{
    render_list[render_count++]=surface;
}

void TerrainGraphics::pushRebuild(TerrainSurface *surface)
{
    rebuild_list[rebuild_count++]=surface;
}

int TerrainGraphics::maxMaterials() const
{
    return 4;
}

TerrainSurface *TerrainGraphics::popRender()
{
    return render_list[--render_count];
}

TerrainSurface **TerrainGraphics::renderList(int &count)
{
    count=render_count;
    return render_list;
}

int TerrainGraphics::renderCount()
{
    return render_count;
}

void TerrainGraphics::clearRenderList()
{
    render_count=0;
}

TerrainRegion *TerrainGraphics::createRegion(int x, int y, int z)
{
    if(grid[x*nodes_z*nodes_y +y*nodes_z+z]==NULL)
    {
        grid[x*nodes_z*nodes_y +y*nodes_z+z]=new TerrainSurface(this,x*node_size,y*node_size,z*node_size,node_size);
        map->insertObject(grid[x*nodes_z*nodes_y +y*nodes_z+z]);
    }
    return grid[x*nodes_z*nodes_y +y*nodes_z+z];
}

TerrainMaterial *TerrainGraphics::getMaterial(int id)
{
    for(int i=0; i<maxMaterials(); i++)
    {
        if(materials[i]->id()==id) return materials[i];
    }
    return NULL;
}

bool TerrainGraphics::addMaterial(TerrainMaterial *material)
{
    //TODO
    return false;
}

int TerrainGraphics::materialCount() const
{
    return material_count;
}

TextureArray *TerrainGraphics::textures()
{
    return &texture;
}

TextureArray *TerrainGraphics::normalMaps()
{
    return &normalmap;
}
