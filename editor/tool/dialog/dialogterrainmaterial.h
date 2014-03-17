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
#ifndef DIALOGTERRAINTEXTURE_H
#define DIALOGTERRAINTEXTURE_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QButtonGroup>
#include <QImageReader>

#include "../../../project/project.h"
#include "../../../assets/texturedialog.h"

namespace Ui {
class DialogTerrainTexture;
}

class DialogTerrainMaterial : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogTerrainMaterial(TerrainGraphics *terrain, TerrainIO *terrain_io,Assets * assets, QWidget *parent = 0);
    ~DialogTerrainMaterial();
private slots:
    void importTexture(int id);
    void importNormalMap(int id);
    void importAsset(int id);
    
    void on_buttonBox_accepted();

private:
    Ui::DialogTerrainTexture *ui;
    TerrainGraphics * terrain;
    TerrainIO * terrain_io;
    Assets * assets;

    QButtonGroup texture_group;
    QButtonGroup normalmap_buttons;
    QButtonGroup assets_buttons;
    QLabel ** labels_texture, ** labels_normal;
    QString * texture_path,*normalmap_path;
    bool changed;
};

#endif // DIALOGTERRAINTEXTURE_H
