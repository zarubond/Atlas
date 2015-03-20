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

#ifndef SOUNDSYSTEM_H
#define SOUNDSYSTEM_H

#include <QAudioFormat>
#include <QAudioOutput>
#include <QThread>
#include <QMutex>
#include <QIODevice>

#include <limits>
#include <stdint.h>
//http://fledisplace.com/

class Sound;

class SoundSystem: public QThread
{
    Q_OBJECT
public:
    SoundSystem(int max_sound=32);
    ~SoundSystem();
    void init();

    void update();
    void run();
    void stop();

    static void play(Sound * sound);

    float getVolume() const;
    void setVolume(float value);

private:
    struct SoundPlayer
    {
        Sound * sound;
        int offset;
    };

    void add(Sound * sound);
    void remove(int id);
    short mixSample(short sample1, short sample2);

    QAudioOutput* audio; // class member.
    float volume;
    int TimerStepSize;
    int WriteSize;
    QIODevice *sound_buffer;

    SoundPlayer stack[32];
    int stack_size;
    const int max_sound;

    short * stream_buffer;
    QMutex mutex;
    bool running;

    static SoundSystem *self_ptr;
};

#endif // SOUNDSYSTEM_H
