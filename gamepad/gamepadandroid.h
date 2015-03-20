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
#ifndef GAMEPADANDROID_H
#define GAMEPADANDROID_H

#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#include <jni.h>

#include "gamepadevent.h"
#include "../lib/tools/shell.h"
#include "gamepadprototype.h"

class GamepadAndroid: public GamepadPrototype
{
    Q_OBJECT
public:
    GamepadAndroid();
private:
    void init();

    static void gamepadAxes(JNIEnv *, jobject, jint axes, jfloat value);
    static void gamepadButton(JNIEnv *, jobject, jint button, jboolean press);
    static void gamepadDevice(JNIEnv *, jobject, jstring name);
    static GamepadAndroid * self_ptr;
    GamepadEvent event;
};

#endif // GAMEPADANDROID_H
