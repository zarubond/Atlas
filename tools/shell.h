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
#ifndef SHELL_H
#define SHELL_H


#include <iostream>
#include <fstream>
#include <streambuf>
#include <string>

#include <QDebug>
#include <QFile>
#include "./opengl.h"

class Shell:public std::basic_streambuf<char>
{
private:
    enum Color{
        RED=0,GREEN,BLUE,DEFAULT
    };
    void print(const char * text,Color color=DEFAULT);
public:
    Shell() : m_stream(std::cout)
    {
        this->shell=this;
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

    ~Shell()
    {
#if defined(Q_WS_WIN) && defined(QT_NO_DEBUG)
        stream_out.close();
#endif
    }
    /**
     * @brief log Print log to shell
     * @param str
     */
    static void log(const char * str);
    /**
     * @brief err Print error log to shell
     * @param str
     */
    static void err(const char * str);
    /**
     * @brief out Print some text to shell
     * @param str
     */
    static void out(const char * str);

protected:
    virtual int_type overflow(int_type v)
    {
        if (v == '\n')
        {
            print(m_string.c_str());
            m_string.erase(m_string.begin(), m_string.end());
        }
        else
            m_string += v;

        return v;
    }

    virtual std::streamsize xsputn(const char *p, std::streamsize n)
    {
        m_string.append(p, p + n);

        std::string::size_type pos = 0;
        while (pos != std::string::npos)
        {
            pos = m_string.find('\n');
            if (pos != std::string::npos)
            {
                std::string tmp(m_string.begin(), m_string.begin() + pos);
                print(tmp.c_str());
                m_string.erase(m_string.begin(), m_string.begin() + pos + 1);
            }
        }

        return n;
    }
#if defined(Q_WS_WIN) && defined(QT_NO_DEBUG)
    static void customMessageHandler(QtMsgType type, const char *msg);
#endif

private:
    std::ostream &m_stream;
    std::streambuf *m_old_buf;
    std::string m_string;
    static Shell * shell;

#if defined(Q_WS_WIN) && defined(QT_NO_DEBUG)
     //fucking wydle
     std::ofstream stream_out;
#endif

    };

class out: public std::ostream
{
public:
  //  friend out& operator<<(out &output, const QString & var);
protected:
};
/*
out& operator<<(out &output, const QString & var)
{
    //out<<qPrintable(var);

    return output;
}*/

class log: public std::ostream
{

};

class err: public std::ostream
{

};

class in: public std::istream
{

};

/*
Shell& operator<< (Shell &out, const char * str)
{
    // Since operator<< is a friend of the Point class, we can access
    // Point's members directly.
    /*out << "(" << cPoint.m_dX << ", " <<
        cPoint.m_dY << ", " <<
        cPoint.m_dZ << ")";*
    return out;
}*/

#endif // SHELL_H
