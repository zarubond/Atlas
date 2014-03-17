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
#ifndef DIALOGNEWPROJECT_H
#define DIALOGNEWPROJECT_H

#include <QDialog>
#include <QFileDialog>
#include <QErrorMessage>

#include "../session.h"

namespace Ui {
class DialogNewProject;
}
/**
 * @brief The DialogNewProject class
 */
class DialogNewProject : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogNewProject(QWidget *parent,Session * session);
    ~DialogNewProject();
    
private slots:
    void on_pushButton_clicked();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::DialogNewProject *ui;
    Session * session;
};

#endif // DIALOGNEWPROJECT_H
