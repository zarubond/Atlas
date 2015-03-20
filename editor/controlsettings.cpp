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

#include "controlsettings.h"

//keyboard
int ControlSettings::key_forward=Qt::Key_W;
int ControlSettings::key_backward=Qt::Key_S;
int ControlSettings::key_right=Qt::Key_D;
int ControlSettings::key_left=Qt::Key_A;
int ControlSettings::key_up=Qt::Key_F;
int ControlSettings::key_down=Qt::Key_C;
int ControlSettings::key_jump=Qt::Key_Space;
// mouse
float ControlSettings::mouse_sensitivity=1.0;
bool ControlSettings::mouse_inverse=false;
float ControlSettings::mouse_freq=5;
//virtual gamepad
bool ControlSettings::gamepad_swap=false;
float ControlSettings::gamepad_sensitivity=1.0;
float ControlSettings::gamepad_size=30;
//hw gamepad
