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

#include "shell.h"

#define ANSI_COLOR_RED     "\033[31m"
#define ANSI_COLOR_GREEN   "\033[32m"
#define ANSI_COLOR_YELLOW  "\033[33m"
#define ANSI_COLOR_BLUE    "\033[34m"
#define ANSI_COLOR_MAGENTA "\033[35m"
#define ANSI_COLOR_CYAN    "\033[36m"
#define ANSI_COLOR_RESET   "\033[0m"

/*
StreamBuffer::StreamBuffer(std::ostream& str)
   :out(str)
{
#if defined(Q_WS_WIN) && defined(QT_NO_DEBUG)
    stream_out.open("stdout.txt");
    std::cout.rdbuf(this); //redirect std::cout to out.txt!
    std::cerr.rdbuf(this);
    qInstallMsgHandler(Shell::customMessageHandler);
#elif defined(Q_WS_ANDROID)
    m_old_buf = std::cout.rdbuf();
    //std::cerr.rdbuf(this);
    std::cout.rdbuf(this);
    std::cerr.rdbuf(this);
#endif

}

StreamBuffer::~StreamBuffer()
{
#if defined(Q_WS_WIN) && defined(QT_NO_DEBUG)
        stream_out.close();
#endif
}

#if defined(Q_WS_WIN) && defined(QT_NO_DEBUG)
void StreamBuffer::customMessageHandler(QtMsgType type, const char *msg)
{
    QString txt;
    switch (type) {
    case QtDebugMsg:
        txt = QString("Debug: %1").arg(msg);
        shell->print(txt.toUtf8().constData(),DEFAULT);
    break;
    case QtWarningMsg:
        txt = QString("Warning: %1").arg(msg);
        shell->print(txt.toUtf8().constData(),RED);
    break;
    case QtCriticalMsg:
        txt = QString("Critical: %1").arg(msg);
        shell->print(txt.toUtf8().constData(),RED);
    break;
    case QtFatalMsg:
        txt = QString("Fatal: %1").arg(msg);
        shell->print(txt.toUtf8().constData(),RED);
        abort();
    break;
    }
}
#endif

void StreamBuffer::print(const char *text, StreamBuffer::Color color)
{
#if defined(Q_WS_WIN) && defined(QT_NO_DEBUG)
    stream_out<<text<<std::endl;
#elif defined(Q_WS_ANDROID)
    qDebug()<<text;
#else
    switch(color)
    {
    case RED:
        std::cout<<"\033[0;"<<31<<"m"<<text;
        break;
    case BLUE:
        std::cout<<"\033[0;"<<34<<"m"<<text;
        break;
    case GREEN:
        std::cout<<"\033[0;"<<32<<"m"<<text;
        break;
    default:
        std::cout<<"\033[0m"<<text;
        break;
    }

#endif
}

std::streamsize StreamBuffer::xsputn(const char *p, std::streamsize n)
{
    fwrite(p,sizeof(char),n,stdout);
    //printf(ANSI_COLOR_RED     "This text is RED!"     ANSI_COLOR_RESET "\n");
    //printf("\033[22;34mHello, world!\033[0m");
    //std::cerr<<"dsas"<<std::endl;
    //std::cout << "\033[01;31m" << "This is colourful text" << "\033[00m" << endl ;
    //out<<"Default \033[22;34m Green \x1B[0m" << std::endl ;


    return n;
}
*/
Output::Output(Priority priority) : buffer_pos(0)
{
#ifdef OS_ANDROID
    switch(priority)
    {
    case LOG_DEBUG:     android_priority=ANDROID_LOG_DEBUG;break;
    case LOG_VERBOSE:   android_priority=ANDROID_LOG_VERBOSE;break;
    case LOG_INFO:      android_priority=ANDROID_LOG_INFO;break;
    case LOG_ERROR:     android_priority=ANDROID_LOG_ERROR;break;
    default: android_priority=ANDROID_LOG_UNKNOWN;break;
    }
#else
    switch(priority)
    {
    case LOG_DEBUG:     output=stderr;break;
    case LOG_VERBOSE:   output=stdout;break;
    case LOG_INFO:      output=stdout;break;
    case LOG_ERROR:     output=stderr;break;
    default: output=NULL;break;
    }
#endif
}

Output& operator<<(Output &out, const char * var)
{
    out.print(var);
    return out;
}

Output& operator<<(Output &out, const std::string& var)
{
    out.print(var.c_str());
    return out;
}

Output& operator<<(Output &out, const QString & var)
{
    //const char * str=qPrintable(var);
    out.print(var.toUtf8().constData());
    return out;
}

Output& operator<<(Output &out,const Vector2f & var)
{
    out.printf("[v|%f %f]",var[0],var[1]);
    return out;
}

Output& operator<<(Output &out,const Vector2i & var)
{
    out.printf("[v|%f %f]",var[0],var[1]);
    return out;
}

Output& operator<<(Output &out,const Vector3f & var)
{
    out.printf("[v|%f %f %f]",var[0],var[1],var[2]);
    return out;
}

Output& operator<<(Output &out,const Vector3d & var)
{
    out.printf("[v|%f %f %f]",var[0],var[1],var[2]);
    return out;
}

Output& operator<<(Output &out,const Vector3c & var)
{
    int a=var[0],b=var[1],c=var[2];
    out.printf("[v|%d %d %d]",a,b,c);
    return out;
}

Output& operator<<(Output &out,const Vector3i & var)
{
    out.printf("[v|%d %d %d]",var[0],var[1],var[2]);
    return out;
}

Output& operator<<(Output &out,const Vector4f & var)
{
    out.printf("[v|%f %f %f %f]",var[0],var[1],var[2],var[3]);
    return out;
}

Output& operator<<(Output &out,const Vector4i & var)
{
    out.printf("[v|%d %d %d %d]",var[0],var[1],var[2],var[3]);
    return out;
}

template <typename Type>
Output& operator<<(Output &out,const Matrix3<Type> & var)
{
    out.printf("\n[m|%f %f %f]\n",var[0],var[1],var[2]);
    out.printf("[m|%f %f %f]\n",var[3],var[4],var[5]);
    out.printf("[m|%f %f %f]\n",var[6],var[7],var[8]);
    return out;
}

Output& operator<<(Output &out,const Matrix4f& var)
{
    out.printf("\n[m|%f %f %f %f]",var[0],var[1],var[2],var[3]);
    out.printf("[m|%f %f %f %f]",var[4],var[5],var[6],var[7]);
    out.printf("[m|%f %f %f %f]",var[8],var[9],var[10],var[11]);
    out.printf("[m|%f %f %f %f]",var[12],var[13],var[14],var[15]);
    return out;
}

Output& operator<<(Output &out,const Quaternion & var)
{
    out.printf("[q|%f %f %f %f]",var[0],var[1],var[2],var[3]);
    return out;
}

Output& operator<<(Output &out, bool var)
{
    if(var)
        out.printf("[true]");
    else
        out.printf("[false]");

    return out;
}

Output& operator<<(Output &out, signed char var)
{
    out.printf("%c",var);
    return out;
}

Output& operator<<(Output &out, unsigned char var)
{
    //HACK
    out.printf("%d",int(var));
    return out;
}

Output& operator<<(Output &out, signed short var)
{
    out.printf("%dh",var);
    return out;
}
Output& operator<<(Output &out, unsigned short var)
{
    out.printf("%uh",var);
    return out;
}

Output& operator<<(Output &out, signed int var)
{
    out.printf("%d",var);
    return out;
}
Output& operator<<(Output &out, unsigned int var)
{
    out.printf("%u",var);
    return out;
}

Output& operator<<(Output &out, signed long var)
{
    out.printf("%ld",var);
    return out;
}
Output& operator<<(Output &out, unsigned long var)
{
    out.printf("%lu",var);
    return out;
}

Output& operator<<(Output &out, long long var)
{
    out.printf("%lld",var);
    return out;
}
Output& operator<<(Output &out, unsigned long long var)
{
    out.printf("%llu",var);
    return out;
}

Output& operator<<(Output &out, float var)
{
    out.printf("%f",var);
    return out;
}
Output& operator<<(Output &out, double var)
{
    out.printf("%f",var);
    return out;
}
Output& operator<<(Output &out, long double var)
{
    out.printf("%Le",var);
    return out;
}

Output &Output::operator<<(Output &(*func)(Output &))
{
    return func(*this);
}

void Output::flush()
{
#ifdef OS_ANDROID
    __android_log_print(android_priority, "Atlas","%.*s",buffer_pos, buffer);
#else
    fwrite(buffer,sizeof(char),buffer_pos,output);
    fflush(output);
#endif
    buffer_pos=0;
}

void Output::putchar(char ch)
{
    buffer[buffer_pos]=ch;
    buffer_pos++;
    if(buffer_pos>768) flush();
}

void Output::print(const char *str)
{
    int c=0;
    while(str[c]!='\0')
    {
        buffer[buffer_pos++]=str[c++];
        if(buffer_pos>=768)
            flush();
    }
}

void Output::printf(const char *format, ...)
{
    va_list args;
    va_start (args, format);

    buffer_pos+=vsprintf(&buffer[buffer_pos],format,args);
    if(buffer_pos>768)
        flush();
    va_end (args);
}

Output & endl(Output& out)
{
    out.putchar('\n');
    out.flush();
    return out;
}

Output & ends(Output& out)
{
    out.putchar('\0');
    return out;
}

Output & flush(Output& out)
{
    out.flush();
    return out;
}

Output cout(Output::LOG_VERBOSE);
Output clog(Output::LOG_INFO);
Output cdbg(Output::LOG_DEBUG);
Output cerr(Output::LOG_DEBUG);
