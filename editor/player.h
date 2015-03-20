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
#ifndef PLAYER_H
#define PLAYER_H

#include "../lib/tools/moveable.h"
#include "../lib/tools/camera.h"
#include "../project.h"

/**
 * @brief The Player class
 */

class Player: public Moveable
{
public:
    Player();
    ~Player();
    void load(Project * project);
    void loadGL();

    void update(int elapsed);

    float height() const;
    Camera *getCamera();
    const Camera &getCamera() const;
    void setPosition(const Vector3f &value);
    void setRotation(const Quaternion &value);
private:
    Camera camera;
};

#endif // PLAYER_H
