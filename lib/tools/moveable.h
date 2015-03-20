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

#ifndef MOVEABLE_H
#define MOVEABLE_H

#include "../math/math.h"

//http://inside.mines.edu/~gmurray/ArbitraryAxisRotation/
class Moveable
{
public:
    Moveable();

    Matrix4f makeMatrix() const;
    Matrix4f makeRotationMatrix() const;
    Matrix4f makeScaleMatrix() const;
    Matrix4f makeTranslateMatrix() const;

    const Vector3f &scale() const;
    void setScale(const Vector3f &value);

    const Vector3f &position() const;
    void setPosition(const Vector3f &value);

    void translate(float x,float y,float z);
    void translate(const Vector3f & pos);

    const Quaternion & rotation() const;
    void setRotation(const Quaternion &value);

    void setMatrix(const Matrix4f &matrix);

protected:
    virtual void onUpdate();

protected:
    Vector3f m_scale;
    Vector3f m_position;
    Quaternion m_rotation;
};

#endif // MOVEABLE_H
