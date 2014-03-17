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
#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QListWidgetItem>
#include <QSettings>
#include <QFileDialog>

#include "../settings.h"

namespace Ui {
class ConfigDialog;
}

//http://grip.espace-win.org/doc/apps/qt4/html/dialogs-configdialog.html
/**
 * @brief The ConfigDialog class
 * @todo This class is under construction.
 */
class ConfigDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ConfigDialog(QWidget *parent = 0);
    ~ConfigDialog();
    
private slots:
    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void on_browseAsset_clicked();

    void on_buttonBox_accepted();

private:
    void loadAssets();
    void saveAssets();
    Ui::ConfigDialog *ui;

};

#endif // CONFIGDIALOG_H
