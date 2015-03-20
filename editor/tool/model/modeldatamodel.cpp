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

#include "modeldatamodel.h"

ModelDataModel::ModelDataModel(QObject *parent) :
    QObject(parent)
{
    m_positionX=0;
    m_positionY=0;
    m_positionZ=0;

    m_rotationX=0;
    m_rotationY=0;
    m_rotationZ=0;

    m_scaleX=0;
    m_scaleY=0;
    m_scaleZ=0;
    m_model=NULL;
}

float ModelDataModel::positionX() const
{
    return m_positionX;
}

float ModelDataModel::positionY() const
{
    return m_positionY;
}

float ModelDataModel::positionZ() const
{
    return m_positionZ;
}

float ModelDataModel::rotationX() const
{
    return m_rotationX;
}

float ModelDataModel::rotationY() const
{
    return m_rotationY;
}

float ModelDataModel::rotationZ() const
{
    return m_rotationZ;
}

const QString &ModelDataModel::name() const
{
    return m_name;
}

float ModelDataModel::scaleX() const
{
    return m_scaleX;
}

void ModelDataModel::setModel(Model *model)
{
    m_model=NULL;
    if(model!=NULL)
    {
        setPosition(model->position());
        setRotation(model->rotation());
        setScale(model->scale());
        setName(model->getName());
    }
    m_model=model;
}

Model *ModelDataModel::model() const
{
    return m_model;
}

void ModelDataModel::setPosition(const Vector3f position)
{
    setPositionX(position[0]);
    setPositionY(position[1]);
    setPositionZ(position[2]);
}

void ModelDataModel::setRotation(const Quaternion &rotation)
{
    setRotation(rotation.getAngle());
}

void ModelDataModel::setRotation(const Vector3f &rotation)
{
    setRotationX(rotation[0]*RAD_TO_DEG);
    setRotationY(rotation[1]*RAD_TO_DEG);
    setRotationZ(rotation[2]*RAD_TO_DEG);
}

void ModelDataModel::setScale(const Vector3f &scale)
{
    setScaleX(scale[0]);
    setScaleY(scale[1]);
    setScaleZ(scale[2]);
}

float ModelDataModel::scaleY() const
{
    return m_scaleY;
}

float ModelDataModel::scaleZ() const
{
    return m_scaleZ;
}

void ModelDataModel::setPositionX(float arg)
{
    if (m_positionX == arg)
        return;
    m_positionX = arg;
    if(m_model!=NULL) m_model->setPosition(Vector3f(m_positionX, m_positionY, m_positionZ));
    emit positionXChanged(arg);
}

void ModelDataModel::setPositionY(float arg)
{
    if (m_positionY == arg)
        return;
    m_positionY = arg;
    if(m_model!=NULL) m_model->setPosition(Vector3f(m_positionX, m_positionY, m_positionZ));
    emit positionYChanged(arg);
}

void ModelDataModel::setPositionZ(float arg)
{
    if (m_positionZ == arg)
        return;
    m_positionZ = arg;
    if(m_model!=NULL) m_model->setPosition(Vector3f(m_positionX, m_positionY, m_positionZ));
    emit positionZChanged(arg);
}

void ModelDataModel::setRotationX(float arg)
{
    if (m_rotationX == arg)
        return;
    m_rotationX = arg;
    if(m_model!=NULL)  m_model->setRotation(Quaternion(m_rotationX*DEG_TO_RAD, m_rotationY*DEG_TO_RAD, m_rotationZ*DEG_TO_RAD));
    emit rotationXChanged(arg);
}

void ModelDataModel::setRotationY(float arg)
{
    if (m_rotationY == arg)
        return;

    m_rotationY = arg;
    if(m_model!=NULL)  m_model->setRotation(Quaternion(m_rotationX*DEG_TO_RAD, m_rotationY*DEG_TO_RAD, m_rotationZ*DEG_TO_RAD));
    emit rotationYChanged(arg);
}

void ModelDataModel::setRotationZ(float arg)
{
    if (m_rotationZ == arg)
        return;

    m_rotationZ = arg;
    if(m_model!=NULL)  m_model->setRotation(Quaternion(m_rotationX*DEG_TO_RAD, m_rotationY*DEG_TO_RAD, m_rotationZ*DEG_TO_RAD));
    emit rotationZChanged(arg);
}

void ModelDataModel::setName(const QString &arg)
{
    if (m_name == arg)
        return;

    m_name = arg;
    if(m_model!=NULL) m_model->setName(m_name.toUtf8().constData());
    emit nameChanged(arg);
}

void ModelDataModel::setScaleX(float arg)
{
    if (m_scaleX == arg)
        return;

    m_scaleX = arg;
    if(m_model!=NULL) m_model->setScale(Vector3f(m_scaleX, m_scaleY, m_scaleZ));
    emit scaleXChanged(arg);
}

void ModelDataModel::setScaleY(float arg)
{
    if (m_scaleY == arg)
        return;

    m_scaleY = arg;
    if(m_model!=NULL) m_model->setScale(Vector3f(m_scaleX, m_scaleY, m_scaleZ));
    emit scaleYChanged(arg);
}

void ModelDataModel::setScaleZ(float arg)
{
    if (m_scaleZ == arg)
        return;

    m_scaleZ = arg;
    if(m_model!=NULL) m_model->setScale(Vector3f(m_scaleX, m_scaleY, m_scaleZ));
    emit scaleZChanged(arg);
}
