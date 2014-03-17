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
#ifndef DRIVER_H
#define DRIVER_H

#include <QMouseEvent>
#include <QKeyEvent>
#include <QApplication>

#include "../tools/math/math.h"
#include "../world/map.h"
#include "../canvas.h"
#include "player.h"

/**
 * @brief The Driver class
 */

class Driver
{
public:
    Driver();
    void setMap(Map * map,Canvas * canvas);
    bool keyPressEvent(QKeyEvent *e);
    bool keyReleaseEvent(QKeyEvent *e);
    bool mouseMoveEvent(QMouseEvent *e);
    bool mousePressEvent(QMouseEvent *e);
    bool mouseReleaseEvent(QMouseEvent *e);
    bool wheelEvent(QWheelEvent *e);
    void gamepadMotion(float deviation, float angle);
    void gamepadInclination(float deviation,float angle);

    void update(int elapsed_time);
    void setPlayer(Player * player);

    void lockMouse(bool lock);
    bool mouseLocked();
    void enable();
    void disable();

    enum View{
        PLAYER=0,
        EDITOR,
        FREE
    };
    void setView(View view);

private:
    void right(GLfloat dist, Vertex3f &pos);
    void forward(GLfloat dist, Vertex3f &pos);
    void rotateY(float angle);
    void rotateX(float angle);

    bool move;
    float player_jump;
    bool key_forward,key_backward,key_left,key_right,key_up,key_down;
    float speed;

    bool right_press, move_lock;
    int mouse_x,mouse_y;
    int last_time;

    bool touchdown;

    Player * player;
    Map * map;
    View view;

    Canvas * canvas;
};

#endif // DRIVER_H
