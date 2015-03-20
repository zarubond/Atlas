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
#ifndef GAMEPADLINUX_H
#define GAMEPADLINUX_H

#include <libudev.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/joystick.h>
#include <climits>

#include <QSocketNotifier>
#include <QObject>
#include <QDebug>

#include "gamepadevent.h"
#include "gamepaddevice.h"

#include "../lib/tools/shell.h"
#include "gamepadprototype.h"

class GamepadLinux: public GamepadPrototype
{
    Q_OBJECT
public:
    GamepadLinux();
    ~GamepadLinux();
    bool init();
private slots:
    void onGamePadChange();
    void newEvent(GamepadEvent * event);
private:
    bool isGamepadDevice(udev_device *device);
    void listDevices();
    int m_udevMonitorFileDescriptor;
    QSocketNotifier *m_udevSocketNotifier;
    struct udev_monitor *m_udevMonitor;
    struct udev * m_udev;

    std::vector<GamepadDevice*> devices;

    void registerDevice(const std::string &device_file);
    void unregisterDevice(const std::string &device_file);
};

#endif // GAMEPADLINUX_H
