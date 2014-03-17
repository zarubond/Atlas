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
#include "mesh3ds.h"
#include "mesh.h"

Mesh3ds::Mesh3ds()
{
}

bool Mesh3ds::load(const char *file_name, Mesh *mesh)
{
    file=fopen(file_name,"rb");
    if(file==NULL)
        return false;

    if(readInt()==0x4D4D)
    {
        fseek (file,28L,SEEK_SET);
        unsigned char version=readChar();
        if (version<3)
        {
            fclose(file);
            return false;
        }

        fseek (file, 16, SEEK_SET);
        unsigned long len;
        unsigned int id;
        bool done = false;

        while(feof(file) || done == true)
        {
            id=readInt();
            if(feof(file))
            {
                done = true;
                break;
            }
            len=readLong();
            switch(id)
            {
            case 0xFFFF: done = true; break;
            case 0x3D3D: readChunk(len); break;
            default: // Unknown
                {
                    unsigned long pos;
                    pos = ftell(file);
                    pos += (len - 6);
                    if(fseek(file, pos, SEEK_SET))
                        done = true;
                }
                break;
            }
        }
    }

    fclose(file);

    return true;
}


void Mesh3ds::objectBlock(FILE *file)
{
    int i=0;
    char t_char;
    do
    {
        fread (&t_char, sizeof(char), 1, file);
        //p_object->name[i]=l_char;
        //name+=t_char;
        i++;
    }
    while(t_char != '\0' && i<20);
    Mesh::Part * use_mesh=new Mesh::Part;
}

void Mesh3ds::materialBlock(FILE *file)
{

}

unsigned int Mesh3ds::readInt()
{
    unsigned int tmp;
    fread (&tmp, sizeof(tmp), 1, file);
    return tmp;
}

unsigned long Mesh3ds::readLong()
{
    unsigned long tmp;
    fread (&tmp, sizeof(tmp), 1, file);
    return tmp;
}

unsigned char Mesh3ds::readChar()
{
    unsigned char tmp;
    fread (&tmp, sizeof(tmp), 1, file);
    return tmp;
}

std::vector<std::string> Mesh3ds::meshFiles(const char * file_name)
{
    FILE * file=fopen(file_name,"rb");

    std::vector<std::string> list;

    if(file==NULL)
    {
        std::cerr<<"Unable to open mesh:"<<file_name<<std::endl;
        return list;
    }
    list.push_back(file_name);

    return list;
}

void Mesh3ds::readChunk(int len)
{
    unsigned long count=ftell(file) + (len - 6);
    int id;
    unsigned long llen;
    bool done = false;
/*
    while(!done)
    {
        id = ReadInt();
        if(feof(bin3ds)) { break; done = true; }
        llen = ReadLong(); // length of chunk
        switch(id)
        {
        case 0x4000:
        {
            while((ch = fgetc(bin3ds)) != 0)
            {
                tail->name[u] = ch;
                u++;
            }
            tail->name[u] = '\0';
            objectBlock(llen);
        }break;
        case 0xAFFF: materialBlock(llen); break;  // Read materials
        default: // Unknown
        {
            unsigned long pos;
            pos = ftell(bin3ds);
            if((pos - 6) >= count)
            {
                fseek(bin3ds, -6, SEEK_CUR);
                done = 1;
                break;
            }
            pos += (llen - 6);
            if(fseek(bin3ds, pos, SEEK_SET)) done = 1;
        }
    }*/
}
