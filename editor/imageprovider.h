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
#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QQuickPaintedItem>
#include <QPainter>
#include "../lib/tools/shell.h"

class ImageProvider : public QQuickPaintedItem
{
    Q_OBJECT
public:
    explicit ImageProvider(QQuickItem *parent = 0);
    void paint(QPainter * painter);
    void setImage(const QImage & image);
    QImage & image();

signals:

public slots:
private:
    QImage m_image;

};

#endif // IMAGEPROVIDER_H
