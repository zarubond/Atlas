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

#ifndef FILE_H
#define FILE_H
#include <QFile>
#include "shell.h"

class File
{
public:
    File(const std::string &name);
    ~File();
    enum Mode{
        READ=0x0001,
        WRITE=0x0002,
        READWRITE=READ|WRITE,
        APPEND=0x0004,
        TEXT=0x0010
    };
    enum Origin{
        BEGIN=0,
        CURRENT=1,
        END=2
    };

    bool open(Mode mode);
    size_t read(void *ptr, size_t size, size_t count);
    void close();
    char *gets(char * str, int num);
    bool isOpen() const;
    long int tell();
    int seek(long int offset, Origin origin);
    long int size() const;
private:
#ifdef QT_VERSION
    QFile file;
#else
    FILE * file;
#endif
};

#endif // FILE_H
