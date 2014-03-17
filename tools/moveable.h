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
#ifndef MOVEABLE_H
#define MOVEABLE_H

#include "math/math.h"

//http://inside.mines.edu/~gmurray/ArbitraryAxisRotation/
class Moveable
{
public:
    Moveable();

    Matrix4f makeMatrix() const;
    Matrix4f makeRotationMatrix() const;
    Matrix4f makeScaleMatrix() const;
    Matrix4f makeTranslateMatrix() const;

    const Vertex3f &getScale() const;
    void setScale(const Vertex3f &value);

    const Vertex3f &getPosition() const;
    void setPosition(const Vertex3f &value);

    void translate(float x,float y,float z);

    const Quaternion & getRotation() const;
    void setRotation(const Quaternion &value);

    void setMatrix(const Matrix4f &matrix);

protected:
    virtual void onUpdate();

protected:
    Vertex3f scale;
    Vertex3f position;
    Quaternion rotation;
};

#endif // MOVEABLE_H
