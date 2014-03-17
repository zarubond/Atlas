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
#ifndef WELCOME_H
#define WELCOME_H

#include <QWidget>
#include <QSettings>
#include <QButtonGroup>
#include <QPushButton>

namespace Ui {
class Welcome;
}
class Atlas;
class Welcome : public QWidget
{
    Q_OBJECT
    
public:
    explicit Welcome(QWidget *parent = 0);
    ~Welcome();
    /**
     * @brief load Load welcom screen.
     * @param atlas
     */
    void load(Atlas * atlas);
private slots:

    void on_push_OpenProject_clicked();
    void on_push_NewProject_clicked();
    void openRecentProject();

private:
    Ui::Welcome *ui;
    QPushButton * buttons[5];
    QStringList files;
    Atlas * atlas;
};

#endif // WELCOME_H
