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

#include "gamepaddevice.h"

GamepadDevice::GamepadDevice(const std::string &device_file):
    is_open(false)
{
    this->device_file=device_file;
    file_descriptor = open(device_file.c_str(), O_RDONLY | O_NONBLOCK);
    if (file_descriptor == -1)
        return;

    is_open=true;

    m_notifier = new QSocketNotifier(file_descriptor, QSocketNotifier::Read, this);
    connect(m_notifier, SIGNAL(activated(int)), this, SLOT(readCallback()));
}

GamepadDevice::~GamepadDevice()
{
    if (file_descriptor != -1)
        close(file_descriptor);
}

bool GamepadDevice::isOpen()
{
    return is_open;
}

const std::string &GamepadDevice::deviceFile()
{
    return device_file;
}

void GamepadDevice::updateForEvent(const js_event & event)
{
    if (!(event.type & JS_EVENT_AXIS || event.type & JS_EVENT_BUTTON))
        return;

    if (event.type & JS_EVENT_AXIS)
    {
        GamepadEvent::PIVOT pivot=GamepadEvent::UNKNOWN;
        switch(event.number)
        {
        case 0: pivot=GamepadEvent::AXIS0_X; break;
        case 1: pivot=GamepadEvent::AXIS0_Y; break;
        case 2: pivot=GamepadEvent::UNKNOWN; break;
        case 3: pivot=GamepadEvent::AXIS1_X; break;
        case 4: pivot=GamepadEvent::AXIS1_Y; break;
        case 5: pivot=GamepadEvent::AXIS2_X; break;
        case 6: pivot=GamepadEvent::AXIS2_Y; break;
        }

        m_event=GamepadEvent(GamepadEvent::AXIS, pivot, float(event.value)/float(SHRT_MAX));
        emit newEvent(&m_event);
    }
    else if (event.type & JS_EVENT_BUTTON)
    {
        if(event.value==0)
            m_event=GamepadEvent(GamepadEvent::BUTTON_RELEASE, static_cast<GamepadEvent::BUTTON>(event.number));
        else if(event.value==1)
            m_event=GamepadEvent(GamepadEvent::BUTTON_PRESS, static_cast<GamepadEvent::BUTTON>(event.number));
        emit newEvent(&m_event);
    }
    else if(event.type & JS_EVENT_INIT)
    {
    }
}

void GamepadDevice::readCallback()
{
    struct js_event event;

    while (read(file_descriptor, &event, sizeof(event)) > 0) {
        updateForEvent(event);
    }
}

