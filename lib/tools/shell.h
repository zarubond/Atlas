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

#ifndef SHELL_H
#define SHELL_H


#include <cstdio>

#include "./envi.h"
#include "../math/math.h"
#include <QString>


#ifdef OS_ANDROID
    #include <android/log.h>
//https://gist.github.com/dzhioev/6127982
#endif

#define FILEINFO "("<<__FILE__<<":"<<__LINE__<<")"

/*
class StreamBuffer: public std::stringbuf
{
public:
    StreamBuffer(std::ostream& str);
    ~StreamBuffer();

private:

    virtual std::streamsize xsputn(const char *p, std::streamsize n);

    enum Color{
        RED=0,GREEN,BLUE,DEFAULT
    };
    void print(const char * text,Color color=DEFAULT);
#if defined(Q_WS_WIN) && defined(QT_NO_DEBUG)
    static void customMessageHandler(QtMsgType type, const char *msg);
#endif

    std::streambuf *m_old_buf;
    std::ostream &out;
#if defined(Q_WS_WIN) && defined(QT_NO_DEBUG)
     //fucking wydle
     std::ofstream stream_out;
#endif
};*/

class Output
{
public:
    enum Priority{
        LOG_UNKNOWN=0,
        LOG_DEBUG,
        LOG_VERBOSE,
        LOG_INFO,
        LOG_ERROR
    };

    Output(Priority priority);
    friend Output& operator<<(Output &out, const char * var);
    friend Output& operator<<(Output &out, const std::string& var);
    friend Output& operator<<(Output &out, const QString& var);

    //custom variables
    friend Output& operator<<(Output &out, const Vector2f & var);
    friend Output& operator<<(Output &out, const Vector2i & var);
    friend Output& operator<<(Output &out, const Vector3f & var);
    friend Output& operator<<(Output &out, const Vector3c & var);
    friend Output& operator<<(Output &out, const Vector3d & var);
    friend Output& operator<<(Output &out, const Vector3i & var);
    friend Output& operator<<(Output &out, const Vector4f & var);
    friend Output& operator<<(Output &out, const Vector4i & var);
    template <typename Type>
    friend Output& operator<<(Output &out, const Matrix3<Type> & var);
    friend Output& operator<<(Output &out, const Matrix4f & var);
    friend Output& operator<<(Output &out, const Quaternion & var);

    //build in variables
    friend Output& operator<<(Output &out, bool var);
    friend Output& operator<<(Output &out, signed char var);
    friend Output& operator<<(Output &out, unsigned char var);

    friend Output& operator<<(Output &out, signed short var);
    friend Output& operator<<(Output &out, unsigned short var);

    friend Output& operator<<(Output &out, signed int var);
    friend Output& operator<<(Output &out, unsigned int var);

    friend Output& operator<<(Output &out, signed long var);
    friend Output& operator<<(Output &out, unsigned long var);

    friend Output& operator<<(Output &out, long long var);
    friend Output& operator<<(Output &out, unsigned long long var);

    friend Output& operator<<(Output &out, float var);
    friend Output& operator<<(Output &out, double var);
    friend Output& operator<<(Output &out, long double var);

    //other
    Output & operator<<(Output& (*func)(Output&));
    void flush();
    void putchar(char ch);

private:
    void print(const char * str);
    void printf(const char * format, ...);
    char buffer[1024];
    int buffer_pos;
    Priority priority;
#ifdef OS_ANDROID
    android_LogPriority android_priority;
#else
    FILE * output;
#endif
};

Output & endl(Output& out);
Output & ends(Output& out);
Output & flush(Output& out);


extern Output cout;
extern Output cerr;
extern Output cdbg;
extern Output clog;

//#define cerr cerr<<"("<<__FILE__<<":"<<__LINE__<<")"

#endif // SHELL_H
