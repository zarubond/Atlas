/**
 *  Atlas - Volumetric terrain editor
 *  Copyright (C) 2012-2015  Ondřej Záruba
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

#include "../../../project.h"
#include "../../../lib/module/grass/grassio.h"
#include "../../../assets/assettexturedialog.h"

#include <QImageReader>

class DialogGrassTexture : public Dialog
{
    Q_OBJECT
    
public:
    explicit DialogGrassTexture(GrassGraphics *grass, GrassIO *grass_io, Assets * assets, QWindow *parent = 0);
    ~DialogGrassTexture();
    const char *uiFile();
    void setupUi(QQuickItem *  ui);

private slots:
    void replaceTexture(int index, QString path);
    void importAsset(int index);
    
    void accepted();
    void rejected();

private:
    Assets * assets;

    QString * texture_path;
    GrassIO * grass_io;
    bool changed;

    enum ModelRoles {
        TitleRole = Qt::UserRole + 1,
        TextureRole,
        NormalRole,
        MaterialIdRole,
        ChangedRole
    };

    QStandardItemModel model;
};

#endif // DIALOGGRASSTEXTURE_H
