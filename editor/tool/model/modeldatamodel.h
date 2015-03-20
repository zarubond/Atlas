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
#ifndef MODELDATAMODEL_H
#define MODELDATAMODEL_H

#include <QObject>
#include <QModelIndex>
#include "../../../lib/module/model/model.h"

class ModelDataModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(float positionX READ positionX WRITE setPositionX NOTIFY positionXChanged)
    Q_PROPERTY(float positionY READ positionY WRITE setPositionY NOTIFY positionYChanged)
    Q_PROPERTY(float positionZ READ positionZ WRITE setPositionZ NOTIFY positionZChanged)
    Q_PROPERTY(float rotationX READ rotationX WRITE setRotationX NOTIFY rotationXChanged)
    Q_PROPERTY(float rotationY READ rotationY WRITE setRotationY NOTIFY rotationYChanged)
    Q_PROPERTY(float rotationZ READ rotationZ WRITE setRotationZ NOTIFY rotationZChanged)
    Q_PROPERTY(float scaleX READ scaleX WRITE setScaleX NOTIFY scaleXChanged)
    Q_PROPERTY(float scaleY READ scaleY WRITE setScaleY NOTIFY scaleYChanged)
    Q_PROPERTY(float scaleZ READ scaleZ WRITE setScaleZ NOTIFY scaleZChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    explicit ModelDataModel(QObject *parent = 0);

    float positionX() const;
    float positionY() const;
    float positionZ() const;
    float rotationX() const;
    float rotationY() const;
    float rotationZ() const;
    float scaleX() const;
    float scaleY() const;
    float scaleZ() const;
    /**
     * @brief name
     * @return Name of the attached model, or empty string.
     */
    const QString & name() const;

    void setPosition(const Vector3f position);
    void setRotation(const Quaternion & rotation);
    void setRotation(const Vector3f & rotation);
    void setScale(const Vector3f & scale);

    void setModel(Model * model);
    /**
     * @brief model
     * @return Pointer to attaged model or NULL.
     */
    Model *model() const;

signals:
    void positionXChanged(float arg);
    void positionYChanged(float arg);
    void positionZChanged(float arg);
    void rotationXChanged(float arg);
    void rotationYChanged(float arg);
    void rotationZChanged(float arg);
    void scaleXChanged(float arg);
    void scaleYChanged(float arg);
    void scaleZChanged(float arg);
    void nameChanged(const QString & arg);

public slots:
    void setPositionX(float arg);
    void setPositionY(float arg);
    void setPositionZ(float arg);

    void setRotationX(float arg);
    void setRotationY(float arg);
    void setRotationZ(float arg);
    void setName(const QString & arg);
    void setScaleX(float arg);
    void setScaleY(float arg);
    void setScaleZ(float arg);

private:
    void updateAxis();
    float m_positionX;
    float m_positionY;
    float m_positionZ;
    float m_rotationX;
    float m_rotationY;
    float m_rotationZ;
    QString m_name;
    float m_scaleX;
    float m_scaleY;
    float m_scaleZ;
    Model * m_model;
};

#endif // MODELDATAMODEL_H
