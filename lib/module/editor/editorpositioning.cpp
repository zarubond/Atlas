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

#include "editorpositioning.h"

EditorPositioning::EditorPositioning():
    m_position(0.0,0.0,0.0), m_visible(false), m_type(POSITION), m_scale(1.0)
{
}

bool EditorPositioning::visible() const
{
    return m_visible;
}

void EditorPositioning::setVisible(bool visible)
{
    m_visible=visible;
}

const Vector3f &EditorPositioning::position() const
{
    return m_position;
}

void EditorPositioning::setPosition(const Vector3f &position)
{
    m_position=position;
}

EditorPositioning::Type EditorPositioning::type() const
{
    return m_type;
}

void EditorPositioning::setType(const Type &type)
{
    m_type = type;
}
float EditorPositioning::scale() const
{
    return m_scale;
}

void EditorPositioning::setScale(float scale)
{
    m_scale = scale;
}


