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

#ifndef EDITORPOSITIONING_H
#define EDITORPOSITIONING_H

#include "../../math/math.h"

class EditorPositioning
{
public:
    EditorPositioning();
    enum Type{
        POSITION,
        SCALE,
        ROTATION
    };
    bool visible() const;
    void setVisible(bool visible);

    const Vector3f &position() const;
    void setPosition(const Vector3f & position);

    Type type() const;
    void setType(const Type &type);


    float scale() const;
    void setScale(float scale);

private:
    Vector3f m_position;
    bool m_visible;
    Type m_type;
    float m_scale;
};

#endif // EDITORPOSITIONING_H
