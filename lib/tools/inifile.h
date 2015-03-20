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

#ifndef INIFILE_H
#define INIFILE_H

#include <fstream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

class IniFile
{
public:
    IniFile();
    /**
     * @brief open Open ini type file.
     * @param file_name Name of the file.
     * @return True on success.
     */
    bool open(const char * file_name);
    /**
     * @brief close Close file.
     */
    void close();
    /**
     * @brief setValue Set new value in file.
     * @param key Key name.
     * @param value Value of key.
     */
    void setValue(const char * key, const char * value);
    void setValue(const char *key, const int value);
    void setValue(const char *key, const float value);
    /**
     * @brief value Retrieves value of given key.
     * @param key   Key name.
     * @param value Reference for value.
     * @return True on success.
     */
    bool value(const char * key, std::string & value);
    bool value(const char *key, int &value);
    bool value(const char *key, float &value);
    /**
     * @brief remove Eraces key from file.
     * @param key Key name.
     * @return True on success.
     */
    bool remove(const char * key);
    /**
     * @brief clear Eracess whole file.
     */
    void clear();
private:
    char * findKey(const char * key);
    FILE * file;
    char line[512];//buffer
};

#endif // INIFILE_H
