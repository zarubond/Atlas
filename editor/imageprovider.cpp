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

#include "imageprovider.h"

ImageProvider::ImageProvider(QQuickItem *parent) :
    QQuickPaintedItem(parent)
{
    setFlag(QQuickItem::ItemHasContents,true);
    setRenderTarget(QQuickPaintedItem::FramebufferObject);
}

void ImageProvider::paint(QPainter *painter)
{
    if(m_image.isNull())
        return;

    float scale=1.0;
    if(width() < m_image.width())
        scale=float(width())/float(m_image.width());

    if(height() < m_image.height() && float(height())/float(m_image.height()) < scale)
        scale=float(height())/float(m_image.height());

    QRect rect;
    rect.setWidth(m_image.width()*scale);
    rect.setHeight(m_image.height()*scale);

    rect.moveCenter(boundingRect().center().toPoint());
    painter->drawImage(rect,m_image);
}

void ImageProvider::setImage(const QImage &image)
{
    m_image=image;
    update();
}

QImage &ImageProvider::image()
{
    return m_image;
}
