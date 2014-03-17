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
#ifndef DIALOGPROCEDURAL_H
#define DIALOGPROCEDURAL_H

#include <QDialog>
#include "../../../project/project.h"

#include "../../../tools/simplexnoise.h"
#include "../../../tools/perlinnoise.h"

#include "../../../tools/diamondsquare.h"

namespace Ui {
class DialogTerrainProcedural;
}

class TerrainTool;

class DialogTerrainProcedural : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogTerrainProcedural(TerrainIO * terrain_io,TerrainTool *parent = 0);
    ~DialogTerrainProcedural();
    
private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_combo_texture_currentIndexChanged(int index);
    void on_pushErode_clicked();
    void on_pushNoise_clicked();
    void on_pushSmooth_clicked();
    void on_combo_texture2_currentIndexChanged(int index);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    void updateImage();
    void disable();
    void enable();
    void perturb(float f, float d);

    Ui::DialogTerrainProcedural *ui;
    TerrainTool * tool;
    unsigned char texture_vertical,texture_horizontal;
    double * data;

    int width,depth,min_height,max_height;

    float scale,persistence;
    int power;
};

#endif // DIALOGPROCEDURAL_H
