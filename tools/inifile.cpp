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
#include "inifile.h"

IniFile::IniFile()
{
}

bool IniFile::open(const char *file_name)
{
    file = fopen (file_name , "rw");
    if(file==NULL)
        return false;
    return true;
}

void IniFile::close()
{
    fclose(file);
}


void IniFile::setValue(const char *key, const char *value)
{
}

void IniFile::setValue(const char *key, const int value)
{
}

void IniFile::setValue(const char *key, const float value)
{
}

bool IniFile::value(const char *key, std::string &value)
{
    char * tmp=findKey(key);
    if(tmp!=NULL)
    {
        value=tmp;
        return true;
    }
    return false;
}

bool IniFile::value(const char *key, int &value)
{
    char * tmp=findKey(key);
    if(tmp!=NULL)
    {
        value=atoi(tmp);
        return true;
    }
    return false;
}

bool IniFile::value(const char *key, float &value)
{
    char * tmp=findKey(key);
    if(tmp!=NULL)
    {
        value=atof(tmp);
        return true;
    }
    return false;
}

bool IniFile::remove(const char *key)
{
    return false;
}

void IniFile::clear()
{
}

char *IniFile::findKey(const char *key)
{
    fseek(file, 0, SEEK_SET);
    int i,key_len=strlen(key);
    while ( feof(file)==0 )
    {
        i=0;
        fgets(line,512,file);

        while(line[i]!='\n' && i < key_len)
        {
            if(line[i]!=key[i])
                goto lineend;
            i++;
        }

        if(line[i]=='=')
        {
            int c=strlen(&line[i+1]);
            line[i+c]=0;
            return &line[i+1];
        }
        lineend:;
    }
    return NULL;
}
