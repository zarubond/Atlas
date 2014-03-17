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
#include "soundsystem.h"
/*
#include "sound.h"

SoundSystem::SoundSystem(int max_sound):max_sound(max_sound)
{
    TimerStepSize=20;
    audio=NULL;
    self_ptr=this;
    volume=0.1;
}

SoundSystem::~SoundSystem()
{
    audio->stop();
    delete audio;
}

void SoundSystem::load()
{
    QAudioFormat format;

    format.setSampleSize(16);
    format.setSampleRate(44100);
    format.setChannelCount(1);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    WriteSize=(TimerStepSize*format.sampleRate()*2)/1000;
    stream_buffer=new short[WriteSize/2+1];
    for(int i=0; i<WriteSize/2;i++)
        stream_buffer[i]=0;

    audio = new QAudioOutput(format);

    for(int i=0;i<max_sound;i++)
    {
        stack[i].sound=NULL;
        stack[i].offset=-1;
    }

    stack_size=0;

    start();
}

void SoundSystem::update()
{
    short * ptr;
    int c=0;
    for(int i=0;i<stack_size;i++)
    {
        if(stack[i].sound!=NULL)
        {
            ptr=(short*)(stack[i].sound->buffer.data());

            if(c==0)
            {
                for(int j=0;j<WriteSize/2;j++)
                {
                    stream_buffer[j]=ptr[stack[i].offset]*volume;
                    stack[i].offset++;
                    if(stack[i].offset==stack[i].sound->buffer.size()/2)
                    {
                        for(;j<WriteSize/2;j++)
                            stream_buffer[j]=0;
                        remove(i);
                        break;
                    }
                }
            }
            else
            {
                for(int j=0;j<WriteSize/2;j++)
                {
                    stream_buffer[j]=mixSample(stream_buffer[j],ptr[stack[i].offset]*volume);
                    stack[i].offset++;
                    if(stack[i].offset==stack[i].sound->buffer.size()/2)
                    {
                        remove(i);
                        break;
                    }
                }
            }
            c++;
        }
    }
    sound_buffer->write((char*)stream_buffer,WriteSize);
}

short int SoundSystem::mixSample(short int sample1, short int sample2)
{
    const int32_t result(static_cast<int32_t>(sample1) + static_cast<int32_t>(sample2));
    typedef std::numeric_limits<short int> Range;
    if (Range::max() < result)
        return Range::max();
    else if (Range::min() > result)
        return Range::min();
    else
        return result;
}

float SoundSystem::getVolume() const
{
    return volume;
}

void SoundSystem::setVolume(float value)
{
    volume = value;
}

void SoundSystem::run()
{
    sound_buffer = audio->start();
    running=true;
    while (running)
    {
        mutex.lock();
        update();
        mutex.unlock();
        msleep(19);
    }
}

void SoundSystem::stop()
{
    running=false;
}

void SoundSystem::play(Sound *sound)
{
    self_ptr->mutex.lock();
    self_ptr->add(sound);
    self_ptr->mutex.unlock();
}

void SoundSystem::add(Sound *sound)
{
    if(stack_size<max_sound)
    {
        stack[stack_size].sound=sound;
        stack[stack_size].offset=0;
        stack_size++;
    }
}

void SoundSystem::remove(int id)
{
    if(id<stack_size)
    {
        stack_size--;
        for(int i=id;i<stack_size;i++)
        {
            stack[i]=stack[i+1];
        }
        stack[stack_size].sound=NULL;
        stack[stack_size].offset=-1;
    }
}

SoundSystem *SoundSystem::self_ptr=NULL;
*/
