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

#include <QtCore>

#if (defined(Q_OS_LINUX) || defined(Q_OS_WINDOWS)) && !(defined(Q_OS_ANDROID) || defined(Q_OS_SAILFISH))
    #include <QApplication>
    #include <QStyleFactory>
#else
    #include <QGuiApplication>
#endif

#include <QPalette>

#include "atlas.h"

//http://qt-project.org/wiki/How_to_do_dynamic_translation_in_QML

/**
 * @brief main
 * @param argc
 * @param argv
 * @copyright Copyright 2012-2014 Ondrej Zaruba. All rights reserved.
 * @return
 */

int main(int argc, char *argv[])
{

#if (defined(Q_OS_LINUX) || defined(Q_OS_WINDOWS)) && !(defined(Q_OS_ANDROID) || defined(Q_OS_SAILFISH))

    QApplication app(argc, argv);
    QPalette palette=app.palette();
    palette.setColor(QPalette::Window,QColor(89,89,89));
    palette.setColor(QPalette::WindowText,Qt::white);
    palette.setColor(QPalette::Base,QColor(107,107,107));
    palette.setColor(QPalette::AlternateBase,QColor(100,100,100));
    palette.setColor(QPalette::ToolTipBase,Qt::white);
    palette.setColor(QPalette::ToolTipText,Qt::white);
    palette.setColor(QPalette::Text,Qt::white);
    palette.setColor(QPalette::Button,QColor(68,68,68));
    palette.setColor(QPalette::ButtonText,Qt::white);
    palette.setColor(QPalette::BrightText,Qt::white);
    //other
    /*
    palette.setColor(QPalette::Light,Qt::green);
    palette.setColor(QPalette::Midlight,Qt::green);
    palette.setColor(QPalette::Dark,Qt::green);
    palette.setColor(QPalette::Mid,Qt::green);
    palette.setColor(QPalette::Shadow,Qt::green);
*/
    palette.setColor(QPalette::Highlight,Qt::darkBlue);
    palette.setColor(QPalette::HighlightedText,Qt::white);
    palette.setColor(QPalette::Link,Qt::blue);
    palette.setColor(QPalette::LinkVisited,Qt::magenta);

    app.setPalette(palette);
#else
    QGuiApplication app(argc, argv);
#endif

    app.setOrganizationName("Zarubond");
    app.setApplicationName("Atlas");
    app.setOrganizationDomain("zarubond.com");
    app.setApplicationVersion("0.5.2");

    Atlas starter;
    starter.start();

    return app.exec();
}
