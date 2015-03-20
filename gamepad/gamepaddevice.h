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
#ifndef GAMEPADDEVICE_H
#define GAMEPADDEVICE_H

#include <QObject>
#include <QSocketNotifier>
#include <QDebug>

#include <unistd.h>
#include <fcntl.h>
#include <linux/joystick.h>
#include <climits>

#include "gamepadevent.h"

#include "../lib/tools/shell.h"

class GamepadDevice: public QObject
{
    Q_OBJECT
public:
    GamepadDevice(const std::string & device_file);

    ~GamepadDevice();

    bool isOpen();
    const std::string &deviceFile();

signals:
    void newEvent(GamepadEvent* event);
private slots:
    void readCallback();


private:
    void updateForEvent(const js_event &event);

    std::string device_file;
    int file_descriptor;
    QSocketNotifier* m_notifier;
    GamepadEvent m_event;
    bool is_open;
};

#endif // GAMEPADDEVICE_H
