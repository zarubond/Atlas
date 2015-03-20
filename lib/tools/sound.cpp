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

#include "sound.h"

Sound::Sound()
{
}

bool Sound::open(const char *file_name)
{
    QFile inputFile; // class member.
    inputFile.setFileName(file_name);
    if(inputFile.open(QIODevice::ReadOnly))
    {
        inputFile.seek(44); // length of wav header
        buffer=inputFile.readAll();
        inputFile.close();
        return true;
    }
    return false;
}

void Sound::play()
{
    SoundSystem::play(this);
}

void Sound::stop()
{
}

void Sound::pause()
{
}

void Sound::repeat(bool status)
{
}

