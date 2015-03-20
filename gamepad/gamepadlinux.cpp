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

#include "gamepadlinux.h"

GamepadLinux::GamepadLinux()
{
    init();
}

GamepadLinux::~GamepadLinux()
{
    for(std::vector<GamepadDevice*>::iterator it = devices.begin(); it != devices.end(); ++it)
    {
        delete (*it);
    }

    udev_unref(m_udev);
    udev_monitor_unref(m_udevMonitor);
}

bool GamepadLinux::init()
{
    m_udev = udev_new();
    if (!m_udev)
    {
        cout<<"Failed to get udev library context."<<endl;
        return false;
    }

    m_udevMonitor = udev_monitor_new_from_netlink(m_udev, "udev");

    if (!m_udevMonitor) {
        cerr<<"Unable to create an Udev monitor. No devices can be detected."<<endl;
        return false;
    }

    udev_monitor_enable_receiving(m_udevMonitor);
    udev_monitor_filter_add_match_subsystem_devtype(m_udevMonitor, "input", 0);

    m_udevMonitorFileDescriptor = udev_monitor_get_fd(m_udevMonitor);
    m_udevSocketNotifier = new QSocketNotifier(m_udevMonitorFileDescriptor, QSocketNotifier::Read, this);
    connect(m_udevSocketNotifier, SIGNAL(activated(int)), this, SLOT(onGamePadChange()));

    listDevices();

    return true;
}

void GamepadLinux::onGamePadChange()
{
    struct udev_device* device = udev_monitor_receive_device(m_udevMonitor);
    if (!isGamepadDevice(device))
        return;
    QByteArray action(udev_device_get_action(device));
    if (action == "add")
        registerDevice(udev_device_get_devnode(device));
    else if (action == "remove")
        unregisterDevice(udev_device_get_devnode(device));
}

void GamepadLinux::newEvent(GamepadEvent *event)
{
    emit gamepadEvent(event);
}

void GamepadLinux::listDevices()
{
    struct udev_enumerate* enumerate = udev_enumerate_new(m_udev);
    udev_enumerate_add_match_subsystem(enumerate, "input");
    udev_enumerate_add_match_property(enumerate, "ID_INPUT_JOYSTICK", "1");
    udev_enumerate_scan_devices(enumerate);
    struct udev_list_entry* cur;
    struct udev_list_entry* devs = udev_enumerate_get_list_entry(enumerate);
    for (cur = devs; cur != NULL; cur = udev_list_entry_get_next(cur)) {
        const char* devname = udev_list_entry_get_name(cur);
        struct udev_device* device = udev_device_new_from_syspath(m_udev, devname);
        if (isGamepadDevice(device))
            registerDevice(udev_device_get_devnode(device));
        udev_device_unref(device);
    }
    udev_enumerate_unref(enumerate);
}

bool GamepadLinux::isGamepadDevice(struct udev_device* device)
{
    const char* deviceFile = udev_device_get_devnode(device);
    const char* sysfsPath = udev_device_get_syspath(device);
    if (!deviceFile || !sysfsPath)
        return false;
    if (!udev_device_get_property_value(device, "ID_INPUT") || !udev_device_get_property_value(device, "ID_INPUT_JOYSTICK"))
        return false;
    return QByteArray(deviceFile).startsWith("/dev/input/js");
   // return QByteArray(deviceFile).startsWith("/dev/input/event");
}

void GamepadLinux::registerDevice(const std::string& device_file)
{
    GamepadDevice * device=new GamepadDevice(device_file);
    if(device->isOpen())
    {
        connect(device,SIGNAL(newEvent(GamepadEvent*)),this,SLOT(newEvent(GamepadEvent*)));
        devices.push_back(device);
    }
}

void GamepadLinux::unregisterDevice(const std::string& device_file)
{
    for(std::vector<GamepadDevice*>::iterator it = devices.begin(); it != devices.end(); ++it) {
        if((*it)->deviceFile()==device_file)
        {
            GamepadDevice * tmp=(*it);
            devices.erase(it);
            delete tmp;
            break;
        }
    }
}
