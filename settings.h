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
#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QStringList>
#include <iostream>

class Settings
{
public:
    Settings();

    static int getInt(const QString & key);
    static bool getBool(const QString & key);
    static float getFloat(const QString & key);
    static QString getString(const QString & key);
    static QStringList getStringList(const QString & key);

    static void setValue(const QString & key, int value);
    static void setValue(const QString & key, bool value);
    static void setValue(const QString & key, float value);
    static void setValue(const QString & key, const QString & value);
    static void setValue(const QString &key, const QStringList & value);

    static bool contains(const QString & key);

private:
    QSettings settings;
    static Settings * self_ptr;
};


#endif // SETTINGS_H
