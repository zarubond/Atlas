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
#ifndef MESHDIALOG_H
#define MESHDIALOG_H

#include <QDialog>
#include <QStandardItem>
#include <QListWidgetItem>

#include "assets.h"

namespace Ui {
class MeshDialog;
}
/**
 * @brief The MeshDialog class
 */
class MeshDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MeshDialog(Assets *assets, QWidget *parent = 0);
    ~MeshDialog();
    /**
     * @brief isSelected
     * @return If the mesh was selected
     */
    bool isSelected();
    /**
     * @brief file Path file of selected mesh.
     * @return
     */
    QString file();
    /**
     * @brief preview Path to image with preview of selected mesh.
     * @return
     */
    QString preview();
    /**
     * @brief name Name of the selected mesh.
     * @return
     */
    QString name();

protected:
    void showEvent(QShowEvent *);
private slots:
    void on_buttonBox_accepted();

    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    class Item :public QListWidgetItem
    {
    public:
        QString file;
        QString preview;
        QString name;
    };

    void load();
    void readMesh(QXmlStreamReader &xml);

    Ui::MeshDialog *ui;
    Assets * assets;
    QDir dir;
    Item * selected;
};

#endif // MESHDIALOG_H
