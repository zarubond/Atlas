/**
 *  Atlas - Volumetric terrain editor
 *  Copyright (C) 2012-2013  Ondřej Záruba
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
#include "settings.h"

Settings::Settings()
{
    self_ptr=this;
}

int Settings::getInt(const QString &key)
{
    return self_ptr->settings.value(key).toInt();
}

bool Settings::getBool(const QString &key)
{
    return self_ptr->settings.value(key).toBool();
}

float Settings::getFloat(const QString &key)
{
    return self_ptr->settings.value(key).toFloat();
}

QString Settings::getString(const QString &key)
{
    return self_ptr->settings.value(key).toString();
}

QStringList Settings::getStringList(const QString &key)
{
    return self_ptr->settings.value(key).toStringList();
}

void Settings::setValue(const QString &key, int value)
{
    self_ptr->settings.setValue(key,value);
}

void Settings::setValue(const QString &key, bool value)
{
    self_ptr->settings.setValue(key,value);
}

void Settings::setValue(const QString &key, float value)
{
    self_ptr->settings.setValue(key,value);
}

void Settings::setValue(const QString &key, const QString &value)
{
    self_ptr->settings.setValue(key,value);
}

void Settings::setValue(const QString &key, const QStringList &value)
{
    self_ptr->settings.setValue(key,value);
}

bool Settings::contains(const QString &key)
{
    return self_ptr->settings.contains(key);
}

Settings * Settings::self_ptr=NULL;
