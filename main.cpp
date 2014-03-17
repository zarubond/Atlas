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
#include <QApplication>
#include <QStyleFactory>
#include "atlas.h"

bool expiration()
{
    QDate date=QLocale(QLocale::C).toDate(QString(__DATE__).simplified(), QLatin1String("MMM d yyyy"));;
    QDate today=QDate::currentDate ();

    if(date.daysTo(today) > 3)
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Your trial period has expired! Please download newer version.");
        return false;
    }

    return true;
}
/**
 * @brief main
 * @param argc
 * @param argv
 * @copyright Copyright 2012-2013 Ondrej Zaruba. All rights reserved.
 * @license This project is released under the GNU Public License.
 * @return
 */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("Zarubond");
    app.setApplicationName("Atlas");
/*
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    QPalette p;
    p = app.palette();
    p.setColor(QPalette::Window, QColor(53,53,53));
    p.setColor(QPalette::Button, QColor(53,53,53));
    p.setColor(QPalette::Highlight, QColor(142,45,197));
    p.setColor(QPalette::ButtonText, QColor(255,255,255));
    app.setPalette(p);
*/
    Atlas w;
   // if(expiration())
    {
        w.show();
        w.activateWindow();
        return app.exec();
    }

    return 0;
}
