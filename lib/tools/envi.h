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

#ifndef ENVI_H
#define ENVI_H

//#include <QtCore>

#ifdef UNUSED
#elif defined(__GNUC__)
# define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
# define UNUSED(x) /*@unused@*/ x
#else
# define UNUSED(x) x
#endif
//void func(int UNUSED(whatsig))

#ifdef __unix__
    #if defined(__ANDROID__)
        #define OS_ANDROID
    #elif defined(Q_OS_SAILFISH)
        #define OS_SAILFISH
    #else
        #define OS_LINUX
        #define OS_UNIX
    #endif
#endif

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(__MINGW32__)
    #define OS_WINDOWS
#endif

#ifdef __OSX__
    #define OS_MACOSX
#endif

/*
#ifdef Q_OS_WINRT
    #define OS_WINRT
#elif defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(__MINGW32__)
    #define OS_WINDOWS
#endif*/


#define LIB_QT


#endif // ENVI_H
