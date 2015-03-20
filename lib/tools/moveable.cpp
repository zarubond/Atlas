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

#include "moveable.h"

Moveable::Moveable()
{
    m_scale.set(1,1,1);
    m_position.set(0,0,0);
}

Matrix4f Moveable::makeMatrix() const
{
    Matrix4f tmp=m_rotation.makeMatrix();

    tmp.scale(m_scale[0],m_scale[1],m_scale[2]);
    tmp.translate(m_position[0],m_position[1],m_position[2]);

    return tmp;
}

Matrix4f Moveable::makeRotationMatrix() const
{
    return m_rotation.makeMatrix();
}

Matrix4f Moveable::makeScaleMatrix() const
{
    return Matrix4f();
}

Matrix4f Moveable::makeTranslateMatrix() const
{
    return Matrix4f();
}

const Vector3f &Moveable::scale() const
{
    return m_scale;
}

void Moveable::setScale(const Vector3f &value)
{
    m_scale = value;
    onUpdate();
}

const Vector3f &Moveable::position() const
{
    return m_position;
}

void Moveable::setPosition(const Vector3f &value)
{
    m_position = value;
    onUpdate();
}

void Moveable::translate(float x, float y, float z)
{
    m_position[0]+=x;
    m_position[1]+=y;
    m_position[2]+=z;
    onUpdate();
}

void Moveable::translate(const Vector3f &pos)
{
    m_position+=pos;
    onUpdate();
}

void Moveable::onUpdate()
{
}

const Quaternion &Moveable::rotation() const
{
    return m_rotation;
}

void Moveable::setRotation(const Quaternion &value)
{
    m_rotation = value;
    onUpdate();
}

void Moveable::setMatrix(const Matrix4f &matrix)
{
    m_position[0]=matrix[12];
    m_position[1]=matrix[13];
    m_position[2]=matrix[14];
    m_rotation.setMatrix(matrix);

    Vector3f len(matrix[0],matrix[4],matrix[8]);
    m_scale[0]=len.length();
    len.set(matrix[1],matrix[5],matrix[9]);
    m_scale[1]=len.length();
    len.set(matrix[2],matrix[6],matrix[10]);
    m_scale[2]=len.length();

    onUpdate();
}

