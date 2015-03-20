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

#include "terrainregion.h"
#include "transvoxel.cpp"

#include "terrainbase.h"

TerrainRegion::TerrainRegion(TerrainBase *terrain, int x, int y, int z, int size):
    dirty(true), changed(false), pos(x,y,z),
    voxel_data(NULL),size(size),voxel_compressed(NULL),comressed_size(0),region_type(GROUND),terrain(terrain)
{}

TerrainRegion::~TerrainRegion()
{
    delete [] voxel_data;
    delete [] voxel_compressed;
}

//idea from
//http://www.angelfire.com/linux/myp/MCAdvanced/MCImproved.html
Vector3f TerrainRegion::getNormal(const Vector3i &vert0, const Vector3i &vert1, int step) const
{
    //step=1;
    float a,b;
    Vector3f normal0,normal1;
    if(vert0[0]+step<size && vert0[0]-step>=0 && vert0[1]+step<size &&
       vert0[1]-step>=0 && vert0[2]+step<size && vert0[2]-step>=0)
    {
        normal0[0] =(voxelRaw(vert0[0]-step,vert0[1]  ,vert0[2]  )->value
                   - voxelRaw(vert0[0]+step,vert0[1]  ,vert0[2]  )->value)/127.0;
        normal0[1] =(voxelRaw(vert0[0]  ,vert0[1]-step,vert0[2]  )->value
                   - voxelRaw(vert0[0]  ,vert0[1]+step,vert0[2]  )->value)/127.0;
        normal0[2] =(voxelRaw(vert0[0]  ,vert0[1]  ,vert0[2]-step)->value
                   - voxelRaw(vert0[0]  ,vert0[1]  ,vert0[2]+step)->value)/127.0;
        a=voxelRaw(vert0[0],vert0[1],vert0[2])->value;
    }
    else
    {
        normal0[0] =(voxel(vert0[0]-step,vert0[1]  ,vert0[2]  )->value
                   - voxel(vert0[0]+step,vert0[1]  ,vert0[2]  )->value)/127.0;
        normal0[1] =(voxel(vert0[0]  ,vert0[1]-step,vert0[2]  )->value
                   - voxel(vert0[0]  ,vert0[1]+step,vert0[2]  )->value)/127.0;
        normal0[2] =(voxel(vert0[0]  ,vert0[1]  ,vert0[2]-step)->value
                   - voxel(vert0[0]  ,vert0[1]  ,vert0[2]+step)->value)/127.0;
        a=voxel(vert0[0],vert0[1],vert0[2])->value;
    }

    if(vert1[0]+step<size && vert1[0]-step>=0 && vert1[1]+step<size &&
       vert1[1]-step>=0 && vert1[2]+step<size && vert1[2]-step>=0)
    {
        normal1[0] =(voxelRaw(vert1[0]-step,vert1[1]  ,vert1[2]  )->value
                   - voxelRaw(vert1[0]+step,vert1[1]  ,vert1[2]  )->value)/127.0;
        normal1[1] =(voxelRaw(vert1[0]  ,vert1[1]-step,vert1[2]  )->value
                   - voxelRaw(vert1[0]  ,vert1[1]+step,vert1[2]  )->value)/127.0;
        normal1[2] =(voxelRaw(vert1[0]  ,vert1[1]  ,vert1[2]-step)->value
                   - voxelRaw(vert1[0]  ,vert1[1]  ,vert1[2]+step)->value)/127.0;
        b=voxelRaw(vert1[0],vert1[1],vert1[2])->value;
    }
    else
    {
        normal1[0] =(voxel(vert1[0]-step,vert1[1]  ,vert1[2]  )->value
                   - voxel(vert1[0]+step,vert1[1]  ,vert1[2]  )->value)/127.0;
        normal1[1] =(voxel(vert1[0]  ,vert1[1]-step,vert1[2]  )->value
                   - voxel(vert1[0]  ,vert1[1]+step,vert1[2]  )->value)/127.0;
        normal1[2] =(voxel(vert1[0]  ,vert1[1]  ,vert1[2]-step)->value
                   - voxel(vert1[0]  ,vert1[1]  ,vert1[2]+step)->value)/127.0;
        b=voxel(vert1[0],vert1[1],vert1[2])->value;
    }

    normal0.normalize();
    normal1.normalize();

    float mu = 0;

    if(a-b!=0) mu=a / (a-b);
    else if(normal0.length()==0) mu=1;

    return Vector3f(normal0[0] + mu * (normal1[0] - normal0[0]),
            normal0[1] + mu * (normal1[1] - normal0[1]),
            normal0[2] + mu * (normal1[2] - normal0[2]));
}

Vector3f TerrainRegion::getVertex(int x1, int y1, int z1, int x2, int y2, int z2) const
{
    float v1,v2;

    if(x1<size && y1<size && z1<size)
        v1=voxelRaw(x1,y1,z1)->value;
    else
        v1=voxel(x1,y1,z1)->value;

    if(x2<size && y2<size && z2<size)
        v2=voxelRaw(x2,y2,z2)->value;
    else
        v2=voxel(x2,y2,z2)->value;

    float mu =0;
    if(v1 - v2!=0)  mu= v1 / (v1 - v2);

    return Vector3f(x1 + pos[0] + mu * (x2 - x1), y1 + pos[1] + mu * (y2 - y1), z1 + pos[2] + mu * (z2 - z1));
}

bool TerrainRegion::inBox(int x, int y, int z, int w) const
{
    if(pos[0]>=x && pos[0]+size<=x+w && pos[1]>=y && pos[1]+size<=y+w && pos[2]>=z && pos[2]+size<=z+w)
        return true;
    return false;
}

TerrainVoxel *TerrainRegion::voxelData()
{
    decompress();
    return voxel_data;
}

void TerrainRegion::setVoxelData(TerrainVoxel *data)
{
    this->voxel_data=data;
}

bool TerrainRegion::isFinite(float var) const
{
#ifndef Q_OS_ANDROID
    return std::isfinite(var);
#else
    //android's std is missing isfinite!!!
    return finite(var);
#endif
}

void TerrainRegion::voxelCollision(const Vector3i &vox, const Vector3f &a, Collision &b)
{
    int cubeindex=0;
    Vector3f vertex_list[12];

    //position of all corners from 0,0,0
    static const int corner[][3]={{0,0,0},{1,0,0},{0,0,1},{1,0,1},{0,1,0},{1,1,0},{0,1,1},{1,1,1}};
    Vector3i vert0,vert1;
    int i=vox[0],j=vox[1],k=vox[2];

    i%=size;
    j%=size;
    k%=size;

    //test voxel position
    if(((i+1==size) + (j+1==size) + (k+1==size))==0)
        for(int c=0;c<8;c++)
            cubeindex |= (-(voxelRaw(i+corner[c][0],j+corner[c][1],k+corner[c][2])->value>>7))<<c;
    else
        for(int c=0;c<8;c++)
            cubeindex |= (-(voxel(i+corner[c][0],j+corner[c][1],k+corner[c][2])->value>>7))<<c;


    if (cubeindex==0 || cubeindex==255)
        return;

    const RegularCellData *data = &regularCellData[regularCellClass[cubeindex]];

    //get all voxel edges
    for(int id=0;id < data->GetVertexCount();id++)
    {
        unsigned short edge_id=regularVertexData[cubeindex][id];
        unsigned short corner0 = edge_id & 0x0F;
        unsigned short corner1 = (edge_id & 0xF0) >> 4;

        vert0.set(i+corner[corner0][0],j+corner[corner0][1],k+corner[corner0][2]);
        vert1.set(i+corner[corner1][0],j+corner[corner1][1],k+corner[corner1][2]);

        vertex_list[id]=getVertex(vert0[0],vert0[1],vert0[2],vert1[0],vert1[1],vert1[2]);
    }

    Triangle triangle;

    //form triangles from edges
    for(int id=0;id < data->GetTriangleCount();id++)
    {
        triangle.set(vertex_list[data->vertexIndex[id*3]]
                    ,vertex_list[data->vertexIndex[id*3+1]]
                    ,vertex_list[data->vertexIndex[id*3+2]]);

        Vector3f tmp=triangle.collision(a,b.collision);
        if(tmp!=b.collision)
        {
            b.normal=triangle.normal();
            b.collision=tmp;
            b.object=this;
        }
    }
}

const TerrainVoxel *TerrainRegion::voxel(int x, int y, int z) const
{
    if(x<0 || y<0 || z<0 || x>=size || y>=size || z>=size)
    {
            return terrain->getVoxel(x+pos[0],y+pos[1],z+pos[2]);
    }
    return &voxel_data[x*size*size+y*size+z];
}

int TerrainRegion::type()
{
    return terrain->type();
}

bool TerrainRegion::isDirty() const
{
    return dirty;
}

void TerrainRegion::decompress()
{
    if(voxel_data==NULL)
        terrain->decompress(this);
}

void TerrainRegion::touchDecompress()
{
    if(voxel_data==NULL)
        terrain->decompress(this);
    else
        terrain->touchDecompress(this);
}

bool TerrainRegion::isCompressed() const
{
    return (voxel_data==NULL);
}

const Vector3i &TerrainRegion::position() const
{
    return pos;
}

TerrainRegion::RegionType TerrainRegion::regionType() const
{
    return region_type;
}

void TerrainRegion::setRegionType(TerrainRegion::RegionType type)
{
    region_type=type;
}

void TerrainRegion::setVoxel(int x, int y, int z, const TerrainVoxel *voxel)
{
    decompress();
    x-=pos[0];
    y-=pos[1];
    z-=pos[2];

    voxel_data[x*size*size+y*size+z]=*voxel;

    dirty=true;
    changed=true;
}

void TerrainRegion::setVoxelValue(int x, int y, int z, signed char value)
{
    decompress();
    x-=pos[0];
    y-=pos[1];
    z-=pos[2];

    voxel_data[x*size*size+y*size+z].value=value;

    dirty=true;
    changed=true;
}

void TerrainRegion::setVoxelMaterial(int x, int y, int z, const unsigned char *material)
{
    decompress();
    x-=pos[0];
    y-=pos[1];
    z-=pos[2];
    voxel_data[x*size*size+y*size+z].setMaterial(material);

    dirty=true;
    changed=true;
}

void TerrainRegion::selectDraw(const Camera & )
{
}

void TerrainRegion::selectDraw()
{
}

void TerrainRegion::collision(const Vector3f &a, Collision &b)
{
    touchDecompress();
    if(voxel_data==NULL)
        return;

    Vector3f start=a,end=b.collision;

    Vector3f v=end-start;
    if(start[0] < pos[0])
    {
        start[0]=pos[0];
        start[1]=a[1]+((pos[0]-a[0])*v[1])/v[0];
        start[2]=a[2]+((pos[0]-a[0])*v[2])/v[0];
    }

    if(start[0] > pos[0]+size)
    {
        start[0]=pos[0]+size;
        start[1]=a[1]+((pos[0]+size-a[0])*v[1])/v[0];
        start[2]=a[2]+((pos[0]+size-a[0])*v[2])/v[0];
    }

    if(start[1] < pos[1])
    {
        start[0]=a[0]+((pos[1]-a[1])*v[0])/v[1];
        start[1]=pos[1];
        start[2]=a[2]+((pos[1]-a[1])*v[2])/v[1];
    }

    if(start[1] > pos[1]+size)
    {
        start[0]=a[0]+((pos[1]+size-a[1])*v[0])/v[1];
        start[1]=pos[1]+size;
        start[2]=a[2]+((pos[1]+size-a[1])*v[2])/v[1];
    }

    if(start[2] < pos[2])
    {
        start[0]=a[0]+((pos[2]-a[2])*v[0])/v[2];
        start[1]=a[1]+((pos[2]-a[2])*v[1])/v[2];
        start[2]=pos[2];
    }

    if(start[2] > pos[2]+size)
    {
        start[0]=a[0]+((pos[2]+size-a[2])*v[0])/v[2];
        start[1]=a[1]+((pos[2]+size-a[2])*v[1])/v[2];
        start[2]=pos[2]+size;
    }


    //http://www.gamerendering.com/2009/07/20/grid-traversal/
    Vector3i st=start;
    Vector3d dir=end;
    dir-=start;

    float len=dir.length()+15;//10;
    int maxDepth=len;
    dir/=len;

    Vector3i step(sgn(dir[0]),sgn(dir[1]),sgn(dir[2]));

    Vector3d cellBoundary(
                st[0] + (step[0] > 0 ? 1 : 0),
                st[1] + (step[1] > 0 ? 1 : 0),
                st[2] + (step[2] > 0 ? 1 : 0));
    Vector3d tMax(
                (cellBoundary[0] - start[0]) / dir[0],
                (cellBoundary[1] - start[1]) / dir[1],
                (cellBoundary[2] - start[2]) / dir[2]);
    if (! isFinite(tMax[0])) tMax[0] = INFINITY;
    if (! isFinite(tMax[1])) tMax[1] = INFINITY;
    if (! isFinite(tMax[2])) tMax[2] = INFINITY;

    Vector3d tDelta(
                step[0] / dir[0],
                step[1] / dir[1],
                step[2] / dir[2]);

    if (!isFinite(tDelta[0])) tDelta[0] = INFINITY;
    if (!isFinite(tDelta[1])) tDelta[1] = INFINITY;
    if (!isFinite(tDelta[2])) tDelta[2] = INFINITY;


    for (int i = 0; i < maxDepth; i++)
    {
        if(st[0]<pos[0] || st[0]>pos[0]+size || st[1]<pos[1] || st[1]>pos[1]+size || st[2]<pos[2] || st[2]>pos[2]+size)
            break;

        voxelCollision(st,a,b);

        if (tMax[0] < tMax[1] && tMax[0] < tMax[2])
        {
            st[0] += step[0];
            tMax[0] += tDelta[0];
        }
        else if (tMax[1] < tMax[2])
        {
            st[1] += step[1];
            tMax[1] += tDelta[1];
        }
        else
        {
            st[2] += step[2];
            tMax[2] += tDelta[2];
        }
    }
}
