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

#include "terraincompress.h"
#include "terrainbase.h"

TerrainCompress::TerrainCompress()
{
}

int TerrainCompress::encode(TerrainCompress::Type type, const TerrainRegion *region, signed char *&voxel_data)
{
    if(type == TerrainCompress::EXT_RLE_OLD)
    {
        unsigned char * t=(uint8_t *)voxel_data;
        return compressExtRLEOld(region,t);
    }
    else if(type == TerrainCompress::EXT_RLE)
    {
        unsigned char * t=(uint8_t *)voxel_data;
        return compressExtRLE(region,t);
    }
    else if(type == TerrainCompress::SPLIT_RLE)
    {
        unsigned char * t=(uint8_t *)voxel_data;
        return compressSplitRLE(region,t);
    }
    else
    {
        return compressNone(region,voxel_data);
    }

    return 0;
}

void TerrainCompress::decode(TerrainCompress::Type type, TerrainRegion *region,const signed char *voxel_data)
{
    int node_size=region->size;
    //unsigned int s=node_size,s_2=node_size*node_size;
    const int size=region->comressed_size;

    if(size==0)
    {
        memset(region->voxel_data,0,node_size*node_size*node_size);
        int s_3=node_size*node_size*node_size;
        if(region->regionType()==TerrainRegion::UNDERGROUND)
            for(int i=0;i<s_3;i++)
                region->voxel_data[i].value=-127;
        else
            for(int i=0;i<s_3;i++)
                region->voxel_data[i].value=127;
    }
    else if(type == TerrainCompress::EXT_RLE_OLD)
    {
        TerrainVoxel voxel;
        int count;
        int pos=0;
        int i=0;

        while(i<size)
        {
            count=reinterpret_cast<const unsigned char&>(voxel_data[i++]);
            voxel.value       = reinterpret_cast<const signed char&>(voxel_data[i++]);
            voxel.material[0] = reinterpret_cast<const unsigned char&>(voxel_data[i++]);
            voxel.material[1] = reinterpret_cast<const unsigned char&>(voxel_data[i++]);
            voxel.material[2] = reinterpret_cast<const unsigned char&>(voxel_data[i++]);

            for(int j=0; j<count; j++)
            {
                region->voxel_data[pos] =voxel;
                pos++;
            }
        }
    }
    else if(type==TerrainCompress::EXT_RLE)
    {
        decompressExtRLE(region, voxel_data, size);
    }
    else if(type==TerrainCompress::SPLIT_RLE)
    {
        decompressSplitRLE(region,voxel_data,size);
    }
    else
    {
        cerr<<"Unknown terrain compression format"<<endl;
    }
}

int TerrainCompress::compressExtRLEOld(const TerrainRegion *region, unsigned char *&voxel_data)
{
    TerrainVoxel * tmp,*last=NULL;

    int node_size=region->size;
    int s_3=node_size*node_size*node_size;

    unsigned char count=0;
    int size=0;

    last=&region->voxel_data[0];
    count=1;

    for(int i=1;i<s_3;i++)
    {
        tmp=&region->voxel_data[i];

        if(last->material[0]!=tmp->material[0] || last->material[1]!=tmp->material[1] ||
                last->material[2]!=tmp->material[2] || last->material[3]!=tmp->material[3] ||
                last->value!=tmp->value || count==255)
        {
            voxel_data[size++]=count;
            voxel_data[size++]=reinterpret_cast<unsigned char&>(last->value);
            voxel_data[size++]=last->material[0];
            voxel_data[size++]=last->material[1];
            voxel_data[size++]=last->material[2];
            count=0;
            last=tmp;
        }
        count++;
    }

    voxel_data[size++]=count;
    voxel_data[size++]=reinterpret_cast<unsigned char&>(last->value);
    voxel_data[size++]=last->material[0];
    voxel_data[size++]=last->material[1];
    voxel_data[size++]=last->material[2];

    return size;
}

int TerrainCompress::compressExtRLE(const TerrainRegion *region, unsigned char *&voxel_data)
{
    TerrainVoxel * tmp,*last=NULL;

    int node_size=region->size;
    int s_3=node_size*node_size*node_size;

    unsigned char count=0;
    int size=0;

    last=&region->voxel_data[0];
    count=1;

    for(int i=1;i<s_3;i++)
    {
        tmp=&region->voxel_data[i];

        if(last->material[0]!=tmp->material[0] || last->material[1]!=tmp->material[1] ||
                last->material[2]!=tmp->material[2] || last->material[3]!=tmp->material[3] ||
                last->value!=tmp->value || count==255)
        {
            voxel_data[size++]=count;/*
            voxel_data[size++]=reinterpret_cast<unsigned char&>(last->value);
            memcpy(&voxel_data[size],last->material,sizeof(last->material));
            size+=sizeof(last->material);*/
            memcpy(&voxel_data[size],last,sizeof(*last));
            size+=sizeof(*last);
            count=0;
            last=tmp;
        }
        count++;
    }

    voxel_data[size++]=count;
    voxel_data[size++]=reinterpret_cast<unsigned char&>(last->value);
    memcpy(&voxel_data[size],last->material,sizeof(last->material));
    size+=sizeof(last->material);

   // int sx=compressSeparateRLE(region, voxel_data);
   // cout<<size-sx<<endl;

    return size;
}

void TerrainCompress::decompressExtRLE(TerrainRegion *region,const signed char *voxel_data, int size) const
{
    TerrainVoxel voxel;
    int count;
    int pos=0;
    int i=0;

    while(i<size)
    {
        count             = reinterpret_cast<const unsigned char&>(voxel_data[i++]);
        voxel.value       = reinterpret_cast<const signed   char&>(voxel_data[i++]);
        memcpy(voxel.material,&voxel_data[i],sizeof(voxel.material));
        i+=sizeof(voxel.material);
        for(int j=0;j<count;j++)
        {
            memcpy(&(region->voxel_data[pos++]),&voxel,sizeof(TerrainVoxel));
        }
       // std::fill_n(&(region->data_grid[pos]),count,voxel);
       // pos+=count;
    }
}

int TerrainCompress::compressSplitRLE(const TerrainRegion *region, unsigned char *&voxel_data)
{
    TerrainVoxel * tmp;
    int s_3=region->size*region->size*region->size;
    int size=0;

    TerrainVoxel *last=&region->voxel_data[0];
    unsigned char count=1;

    for(int i=1;i<s_3;i++)
    {
        tmp=&region->voxel_data[i];

        if(last->material[0]!=tmp->material[0] || last->material[1]!=tmp->material[1] ||
                last->material[2]!=tmp->material[2] || last->material[3]!=tmp->material[3] || count==UCHAR_MAX)
        {
            voxel_data[size++]=count;
            memcpy(&voxel_data[size],last->material,sizeof(tmp->material));
            size+=sizeof(tmp->material);

            count=0;
            last=tmp;
        }
        count++;
    }

    voxel_data[size++]=count;
    memcpy(&voxel_data[size],last->material,sizeof(last->material));
    size+=sizeof(last->material);

    last=&region->voxel_data[0];
    count=1;
    for(int i=1;i<s_3;i++)
    {
        tmp=&region->voxel_data[i];
        if(last->value!=tmp->value || count==UCHAR_MAX)
        {
            voxel_data[size++]=count;
            voxel_data[size++]=reinterpret_cast<unsigned char&>(last->value);
            count=0;
            last=tmp;
        }
        count++;
    }
    voxel_data[size++]=count;
    voxel_data[size++]=reinterpret_cast<unsigned char&>(last->value);

    return size;
}

void TerrainCompress::decompressSplitRLE(TerrainRegion *region, const signed char *voxel_data, int size) const
{
    TerrainVoxel voxel;
    int count;
    int pos=0;
    int s_3=region->size*region->size*region->size;

    for(int i=0;i<s_3;)
    {
        count = reinterpret_cast<const unsigned char&>(voxel_data[pos++]);
        memcpy(voxel.material,&voxel_data[pos],sizeof(voxel.material));
        pos+=sizeof(voxel.material);

        for(int j=0;j<count;j++)
            memcpy(&(region->voxel_data[i++]),&voxel,sizeof(TerrainVoxel));
    }

    for(int i=0;i<s_3;)
    {
        count       = reinterpret_cast<const unsigned char&>(voxel_data[pos++]);
        voxel.value = reinterpret_cast<const signed   char&>(voxel_data[pos++]);

        for(int j=0;j<count;j++)
            region->voxel_data[i++].value=voxel.value;
    }
    if(pos!=size)
    {
        cout<<pos<<" "<<size<<endl;
    }
}

int TerrainCompress::compressNone(const TerrainRegion *region, signed char *&voxel_data)
{
    return 0;
}

void TerrainCompress::decodeNone(TerrainBase *base, const signed char *voxel_data, unsigned int node_size, unsigned int size)
{

}
