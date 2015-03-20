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

#include "parsenumber.h"

ParseNumber::ParseNumber()
{
}

char *ParseNumber::parse(float &value, const char *str)
{
    setlocale(LC_NUMERIC, "C");
    char * endptr;
    value=strtof(str,&endptr);
    setlocale(LC_NUMERIC, "");
    return endptr;
}

char *ParseNumber::parse(int &value, const char *str)
{
    setlocale(LC_NUMERIC, "C");
    char * endptr;
    value=strtol(str,&endptr,10);
    setlocale(LC_NUMERIC, "");
    return endptr;
}

char *ParseNumber::parse(char &value, const char *str)
{
    setlocale(LC_NUMERIC, "C");
    char * endptr;
    value=std::strtol(str,&endptr,10);
    setlocale(LC_NUMERIC, "");
    return endptr;
}

char *ParseNumber::parse(long &value, const char *str)
{
    setlocale(LC_NUMERIC, "C");
    char * endptr;
    value=std::strtol(str,&endptr,10);
    setlocale(LC_NUMERIC, "");
    return endptr;
}

char *ParseNumber::parse(short &value, const char *str)
{
    setlocale(LC_NUMERIC, "C");
    char * endptr;
    value=std::strtol(str,&endptr,10);
    setlocale(LC_NUMERIC, "");
    return endptr;
}

char *ParseNumber::parse(double &value, const char *str)
{
    setlocale(LC_NUMERIC, "C");
    char * endptr;
    value=std::strtod(str,&endptr);
    setlocale(LC_NUMERIC, "");
    return endptr;
}
