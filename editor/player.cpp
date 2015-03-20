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

#include "player.h"

Player::Player()
{
    Vector3f posi(30,120,30);
    camera.setPosition(posi);
    camera.setPitch(0);
    camera.setYaw(-M_PI_2-M_PI_4);
}

Player::~Player()
{
}

void Player::load(Project *project)
{
}

void Player::loadGL()
{
}

void Player::setPosition(const Vector3f &value)
{
    m_position=value;
    camera.setPosition(m_position);
}

void Player::setRotation(const Quaternion &value)
{
    m_rotation.rotate(0,camera.getYaw(),0);
}

void Player::update(int elapsed)
{
}

float Player::height() const
{
    return 7.0;
}

Camera *Player::getCamera()
{
    return &camera;
}

const Camera &Player::getCamera() const
{
    return camera;
}
