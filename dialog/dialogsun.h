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
#ifndef DIALOGSUN_H
#define DIALOGSUN_H

#include <QDialog>
#include <QColorDialog>
#include "../world/complement/lightmanager.h"

namespace Ui {
class DialogSun;
}


/**
 * @brief The DialogSun class
 */
class DialogSun : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogSun(LightManager *light, QWidget *parent = 0);
    ~DialogSun();
    
private slots:

    void diffuseColorChange(const QColor & color);
    void specularColorChange(const QColor & color);
    void ambientColorChange(const QColor & color);

    void on_pushChoose_Ambient_clicked();

    void on_pushChoose_Specular_clicked();

    void on_pushChoose_Diffuse_clicked();

    void on_sliderShiniess_valueChanged(int value);

    void on_dialX_valueChanged(int value);

    void on_dialY_valueChanged(int value);

private:
    Ui::DialogSun *ui;
    LightManager * const light;
};

#endif // DIALOGSUN_H
