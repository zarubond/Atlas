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
#ifndef CONTROLSETTINGS_H
#define CONTROLSETTINGS_H

#include <Qt>

//this thing and all thinks that it uses are one big HACK
struct ControlSettings
{
    //keyboard
    static int key_forward, key_backward, key_right, key_left;
    static int key_up, key_down, key_jump;
    // mouse
    static float mouse_sensitivity;
    static bool mouse_inverse;
    static float mouse_freq;
    //virtual gamepad
    static bool gamepad_swap;
    static float gamepad_sensitivity;
    static float gamepad_size;
    //hw gamepad
};

#endif // CONTROLSETTINGS_H
