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
#ifndef GAMEPADEVENT_H
#define GAMEPADEVENT_H

class GamepadEvent
{
public:
    enum BUTTON{
        BUTTON_A = 0x130,
        BUTTON_B,
        BUTTON_C,
        BUTTON_X,
        BUTTON_Y,
        BUTTON_Z,
        BUTTON_TL1,
        BUTTON_TR1,
        BUTTON_TL2,
        BUTTON_TR2,
        BUTTON_Select,
        BUTTON_Start,
        BUTTON_Mode,
        BUTTON_ThumbL,
        BUTTON_ThumbR,
        BUTTON_Up1 = 0x255,
        BUTTON_Down1,
        BUTTON_Left1,
        BUTTON_Right1,
        BUTTON_Up2,
        BUTTON_Down2,
        BUTTON_Left2,
        BUTTON_Right2,
        BUTTON_Up3,
        BUTTON_Down3,
        BUTTON_Left3,
        BUTTON_Right3,
        BUTTON_UNKNOWN
    };

    enum PIVOT{
        AXIS0_X=0,
        AXIS0_Y,
        AXIS1_X,
        AXIS1_Y,
        AXIS2_X,
        AXIS2_Y,
        UNKNOWN
    };

    enum TYPE{
        BUTTON_PRESS,
        BUTTON_RELEASE,
        AXIS,
        INIT
    };

    GamepadEvent();
    GamepadEvent(TYPE type, GamepadEvent::BUTTON button);
    GamepadEvent(TYPE type, GamepadEvent::PIVOT pivot, float value);

    float axisValue() const;
    TYPE type() const;
    BUTTON button() const;
    PIVOT pivot() const;
    int sender() const;

private:
    BUTTON m_button;
    TYPE m_type;
    PIVOT m_pivot;
    float m_axis_value;
    int m_sender;
};

#endif // GAMEPADEVENT_H
