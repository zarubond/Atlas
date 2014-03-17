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
#ifndef TEXTUREDIALOG_H
#define TEXTUREDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QThread>

#include "assets.h"

namespace Ui {
class TextureDialog;
}
/**
 * @brief The TextureDialog class
 */
class TextureDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit TextureDialog(Assets * assets,QWidget *parent = 0);
    ~TextureDialog();
    /**
     * @brief isSelected Is texture selected
     * @return
     */
    bool isSelected();
    /**
     * @brief file Path to selected texture.
     * @return
     */
    QString file();
    /**
     * @brief normal Path to the normal map of selected texture.
     * @return
     */
    QString normal();
    /**
     * @brief name Name of the selected texture.
     * @return
     */
    QString name();
public slots:
    void on_buttonBox_accepted();
    int exec();
protected:
    void showEvent(QShowEvent *);
private slots:

    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    class Item :public QListWidgetItem
    {
    public:
        QString file;
        QString normal;
        QString name;
    };

    void load();
    void readTexture(QXmlStreamReader& xml);
    Ui::TextureDialog *ui;
    Assets * assets;
    QDir dir;

    Item * selected;
    QThread thread;

};

#endif // TEXTUREDIALOG_H
