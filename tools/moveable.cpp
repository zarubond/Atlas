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
#include "moveable.h"

Moveable::Moveable()
{
    scale.set(1,1,1);
    position.set(0,0,0);
}

Matrix4f Moveable::makeMatrix() const
{
    Matrix4f tmp=rotation.makeMatrix();

    tmp.scale(scale[0],scale[1],scale[2]);
    tmp.translate(position[0],position[1],position[2]);

    return tmp;
}

Matrix4f Moveable::makeRotationMatrix() const
{
    return rotation.makeMatrix();
}

Matrix4f Moveable::makeScaleMatrix() const
{
}

Matrix4f Moveable::makeTranslateMatrix() const
{
}

const Vertex3f &Moveable::getScale() const
{
    return scale;
}

void Moveable::setScale(const Vertex3f &value)
{
    scale = value;
    onUpdate();
}

const Vertex3f &Moveable::getPosition() const
{
    return position;
}

void Moveable::setPosition(const Vertex3f &value)
{
    position = value;
    onUpdate();
}

void Moveable::translate(float x, float y, float z)
{
    position[0]+=x;
    position[1]+=y;
    position[2]+=z;
}

void Moveable::onUpdate()
{
}

const Quaternion &Moveable::getRotation() const
{
    return rotation;
}

void Moveable::setRotation(const Quaternion &value)
{
    rotation = value;
    onUpdate();
}

void Moveable::setMatrix(const Matrix4f &matrix)
{
    position[0]=matrix[12];
    position[1]=matrix[13];
    position[2]=matrix[14];
    rotation.setMatrix(matrix);

    Vertex3f len(matrix[0],matrix[4],matrix[8]);
    scale[0]=len.length();
    len.set(matrix[1],matrix[5],matrix[9]);
    scale[1]=len.length();
    len.set(matrix[2],matrix[6],matrix[10]);
    scale[2]=len.length();

    onUpdate();
}

