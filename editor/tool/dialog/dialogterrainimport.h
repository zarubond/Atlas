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
#ifndef DIALOGTERRAINIMPORT_H
#define DIALOGTERRAINIMPORT_H

#include <QDialog>
#include <QFileDialog>

#include "../../../project/project.h"

namespace Ui {
class DialogTerrainImport;
}

class TerrainTool;

class DialogTerrainImport : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogTerrainImport(TerrainTool *parent = 0);
    ~DialogTerrainImport();
    
private slots:
    void on_pushButton_clicked();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_spinMinHeight_valueChanged(int arg1);
    void on_spinMaxHeight_valueChanged(int arg1);

    void on_combo_texture_currentIndexChanged(int index);

private:
    Ui::DialogTerrainImport *ui;
    unsigned char texture;

    QImage image;
    TerrainTool * tool;
    TerrainGraphics * terrain;
};

#endif // DIALOGTERRAINIMPORT_H
