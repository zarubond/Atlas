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
#ifndef MESHDATAMODEL_H
#define MESHDATAMODEL_H

#include <QObject>
#include <QModelIndex>
#include "../../../lib/module/model/modelmesh.h"

class MeshDataModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(float scale READ scale WRITE setScale NOTIFY scaleChanged)
public:
    explicit MeshDataModel(QObject *parent = 0);

    const QString & name() const;
    float scale() const;

    ModelMesh * mesh() const;
    void setMesh(ModelMesh *mesh);

signals:
    void nameChanged(const QString & arg);
    void scaleChanged(float arg);

public slots:

    void setName(const QString & arg);
    void setScale(float arg);
private:
    QModelIndex m_index;
    QString m_name;
    float m_scale;
    ModelMesh * m_mesh;
};

#endif // MESHDATAMODEL_H
