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
#ifndef DIALOGGRASSTEXTURE_H
#define DIALOGGRASSTEXTURE_H

#include <QDialog>
#include "../../../project/project.h"
#include "../../../assets/texturedialog.h"

#include <QButtonGroup>
#include <QLabel>
#include <QPushButton>
#include <QImageReader>
#include <QFileDialog>

namespace Ui {
class DialogGrassTexture;
}

class DialogGrassTexture : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogGrassTexture(GrassGraphics *grass, GrassIO *grass_io, Assets * assets, QWidget *parent = 0);
    ~DialogGrassTexture();
private slots:
    void importTexture(int id);
    void importAsset(int id);
    
    void on_buttonBox_accepted();

private:
    Ui::DialogGrassTexture *ui;
    Assets * assets;

    QButtonGroup texture_group;
    QButtonGroup assets_buttons;
    QLabel ** labels_texture;
    QString * texture_path;
    GrassIO * grass_io;
    bool changed;
};

#endif // DIALOGGRASSTEXTURE_H
