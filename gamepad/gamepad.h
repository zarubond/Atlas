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
#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <QtCore>
#include "gamepadevent.h"
#include "gamepadprototype.h"

#ifdef Q_OS_ANDROID
    #include "gamepadandroid.h"
    class Gamepad: public GamepadAndroid
    {
        Q_OBJECT
    };
#elif defined(Q_OS_LINUX) && !defined(Q_OS_SAILFISH)
    #include "gamepadlinux.h"
    class Gamepad: public GamepadLinux
    {
        Q_OBJECT
    };
#else
    class Gamepad: public GamepadPrototype
    {
        Q_OBJECT
    };
#endif

#endif // GAMEPAD_H
