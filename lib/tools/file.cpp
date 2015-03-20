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

#include "file.h"

File::File(const std::string &name):
    file(name.c_str())
{
}

File::~File()
{
    close();
}

bool File::open(Mode mode)
{
    QIODevice::OpenMode mod=QIODevice::NotOpen;
    if(mode&READ     ) mod|=QIODevice::ReadOnly;
    if(mode&WRITE    ) mod|=QIODevice::WriteOnly;
    if(mode&READWRITE) mod|=QIODevice::ReadWrite;
    if(mode&APPEND   ) mod|=QIODevice::Append;
    if(mode&TEXT     ) mod|=QIODevice::Text;

#ifdef QT_VERSION
    return file.open(static_cast<QIODevice::OpenMode>(mode));
#else
    return fopen(file,mode);
#endif
}

size_t File::read(void * ptr, size_t size, size_t count)
{
#ifdef QT_VERSION
    return file.read((char*)ptr,size*count);
#else
    return fread(ptr,size,count,file);
#endif
}

void File::close()
{
#ifdef QT_VERSION
    file.close();
#else
    fclose(file);
#endif
}

char *File::gets(char *str, int num)
{
#ifdef QT_VERSION
    qint64 read=file.readLine(str,num);
    if(read==-1)
        return NULL;
    return &str[read];
#else
    return fgets(str,num,file);
#endif
}

bool File::isOpen() const
{
    return file.isOpen();
}

long File::tell()
{
#ifdef QT_VERSION
    return file.pos();
#else
    return ftell(file);
#endif
}

int File::seek(long offset, Origin origin)
{
    //file.seek(offset);
    cerr<<"File::seek TODO"<<endl;
    return 0;
}

long File::size() const
{
    return file.size();
}
