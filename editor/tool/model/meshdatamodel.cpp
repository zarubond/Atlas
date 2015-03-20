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

#include "meshdatamodel.h"

MeshDataModel::MeshDataModel(QObject *parent) :
    QObject(parent)
{
    m_scale=0;
    m_mesh=NULL;
}

const QString & MeshDataModel::name() const
{
    return m_name;
}

float MeshDataModel::scale() const
{
    return m_scale;
}

ModelMesh *MeshDataModel::mesh() const
{
    return m_mesh;
}

void MeshDataModel::setMesh(ModelMesh * mesh)
{
    m_mesh=mesh;
    if(m_mesh!=NULL)
    {
        setName(mesh->getName().c_str());
        setScale(mesh->default_scale);
    }
}

void MeshDataModel::setName(const QString &arg)
{
    if (m_name == arg)
        return;
    m_name = arg;
    if(m_mesh!=NULL) m_mesh->setName(m_name.toUtf8().constData());
    emit nameChanged(arg);
}

void MeshDataModel::setScale(float arg)
{
    if (m_scale == arg)
        return;
    m_scale = arg;
    if(m_mesh!=NULL) m_mesh->default_scale=m_scale;
    emit scaleChanged(arg);
}
