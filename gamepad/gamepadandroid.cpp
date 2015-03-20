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

#include "gamepadandroid.h"

static void gamepad(JNIEnv *, jobject, jfloat x, jfloat y)
{
    GamepadEvent event;
}

GamepadAndroid::GamepadAndroid()
{
    self_ptr=this;
    init();
}

void GamepadAndroid::init()
{
    JNINativeMethod methods[] {
        {"gamepadAxis", "(IF)V", reinterpret_cast<void *>(GamepadAndroid::gamepadAxes)},
        {"gamepadButton", "(IZ)V", reinterpret_cast<void *>(GamepadAndroid::gamepadButton)}};

    if (QAndroidJniObject::isClassAvailable("com/zarubond/atlas/GamePad"))
    {
        QAndroidJniObject javaClass("com/zarubond/atlas/GamePad");
        if(javaClass.isValid())
        {
            QAndroidJniEnvironment env;
            jclass objectClass = env->GetObjectClass(javaClass.object<jobject>());
            env->RegisterNatives(objectClass, methods,  sizeof(methods) / sizeof(methods[0]));
            env->DeleteLocalRef(objectClass);
        }
    }
}

void GamepadAndroid::gamepadAxes(JNIEnv *, jobject, jint axes, jfloat value)
{
    self_ptr->event=GamepadEvent(GamepadEvent::AXIS, static_cast<GamepadEvent::PIVOT>(axes), value);
    emit self_ptr->gamepadEvent(&self_ptr->event);
}

void GamepadAndroid::gamepadButton(JNIEnv *, jobject, jint button, jboolean press)
{
    GamepadEvent::BUTTON but=GamepadEvent::BUTTON_UNKNOWN;
    switch(button)
    {
    case 191:// KEYCODE_BUTTON_4
    case 99: but=GamepadEvent::BUTTON_X; break;//KEYCODE_BUTTON_X
    case 188:// KEYCODE_BUTTON_1
    case 100: but=GamepadEvent::BUTTON_Y; break;//KEYCODE_BUTTON_Y
    case 189:// KEYCODE_BUTTON_2
    case 96: but=GamepadEvent::BUTTON_A; break;//KEYCODE_BUTTON_A
    case 190:// KEYCODE_BUTTON_3
    case 97: but=GamepadEvent::BUTTON_B; break;//KEYCODE_BUTTON_B
    case 193:// KEYCODE_BUTTON_6
    case 103: but=GamepadEvent::BUTTON_TR1; break;//KEYCODE_BUTTON_R1
    case 192:// KEYCODE_BUTTON_5
    case 102: but=GamepadEvent::BUTTON_TL1; break;//KEYCODE_BUTTON_L1

    case 195:// KEYCODE_BUTTON_8
    case 105: but=GamepadEvent::BUTTON_TR2; break;//KEYCODE_BUTTON_R2
    case 194:// KEYCODE_BUTTON_7
    case 104: but=GamepadEvent::BUTTON_TL2; break;//KEYCODE_BUTTON_L2

    case 198:// KEYCODE_BUTTON_11
    case 106: but=GamepadEvent::BUTTON_ThumbL; break;//KEYCODE_BUTTON_THUMBL
    case 199:// KEYCODE_BUTTON_12
    case 107: but=GamepadEvent::BUTTON_ThumbR; break;//KEYCODE_BUTTON_THUMBR
    case 196:// KEYCODE_BUTTON_9
    case 109: but=GamepadEvent::BUTTON_Select; break;//KEYCODE_BUTTON_SELECT
    case 197:// KEYCODE_BUTTON_10
    case 108: but=GamepadEvent::BUTTON_Start; break;//KEYCODE_BUTTON_START
    default: break;
    }

    if(but!=GamepadEvent::BUTTON_UNKNOWN)
    {
        self_ptr->event=GamepadEvent(press?GamepadEvent::BUTTON_PRESS:GamepadEvent::BUTTON_RELEASE, but);
        emit self_ptr->gamepadEvent(&self_ptr->event);
    }
}

GamepadAndroid * GamepadAndroid::self_ptr=NULL;
