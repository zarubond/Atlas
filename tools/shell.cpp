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
#include "shell.h"

void Shell::print(const char *text, Shell::Color color)
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

void Shell::log(const char *str)
{
    shell->print(str,BLUE);
}

void Shell::err(const char *str)
{
    shell->print(str,RED);
}

void Shell::out(const char *str)
{
    shell->print(str,DEFAULT);
}

#if defined(Q_WS_WIN) && defined(QT_NO_DEBUG)
void Shell::customMessageHandler(QtMsgType type, const char *msg)
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
    }
}
#endif


Shell * Shell::shell=NULL;
