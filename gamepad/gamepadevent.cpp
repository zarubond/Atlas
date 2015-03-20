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

#include "gamepadevent.h"

GamepadEvent::GamepadEvent()
{
}

GamepadEvent::GamepadEvent(GamepadEvent::TYPE type, GamepadEvent::BUTTON button)
{
    m_type=type;
    m_button=button;
}

GamepadEvent::GamepadEvent(GamepadEvent::TYPE type, GamepadEvent::PIVOT pivot, float value)
{
    m_type=type;
    m_pivot=pivot;
    m_axis_value=value;
}

float GamepadEvent::axisValue() const
{
    return m_axis_value;
}

GamepadEvent::TYPE GamepadEvent::type() const
{
    return m_type;
}

GamepadEvent::BUTTON GamepadEvent::button() const
{
    return m_button;
}

GamepadEvent::PIVOT GamepadEvent::pivot() const
{
    return m_pivot;
}

int GamepadEvent::sender() const
{
    return m_sender;
}
