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
#ifndef DRIVER_H
#define DRIVER_H

#include <QMouseEvent>
#include <QKeyEvent>
#include <QtSensors/QRotationSensor>
#include "controlsettings.h"

#include "../lib/math/math.h"
#include "../lib/map.h"
#include "../canvas.h"
#include "player.h"

/**
 * @brief The Driver class
 */

class Driver
{
public:
    Driver();
    void setMap(Map * map, Canvas *canvas);
    bool keyPressEvent(QKeyEvent *e);
    bool keyReleaseEvent(QKeyEvent *e);
    bool hoverMoveEvent(QHoverEvent *e);
    bool mousePressEvent(QMouseEvent *e);
    bool mouseReleaseEvent(QMouseEvent *e);
    bool mouseMoveEvent(QMouseEvent *e);
    bool wheelEvent(QWheelEvent *e);
    bool gamepadEvent(GamepadEvent * e);

    void gamepadOne(Vector2f angle);
    void gamepadTwo(Vector2f angle);

    void update(int elapsed_time);
    void setPlayer(Player * player);
    /**
     * @brief setGyro Use gyroskope instead of mouse (or gamepad) to determine rotation
     * @param enable
     */
    void setGyro(bool enable);

    void lockMouse(bool lock);
    bool mouseLocked();
    void enable();
    void disable();

    void goUp(bool move);
    void goDown(bool move);

    enum View{
        PLAYER=0,
        //EDITOR,
        FREE
    };
    void setView(View view);

    float getSpeed() const;
    void setSpeed(float value);
    float maximumSpeed();

private:
    void moveSide(GLfloat dist, Vector3f &pos);
    void moveDirect(GLfloat dist, Vector3f &pos);
    void rotateY(float angle);
    void rotateX(float angle);
    void updateGyro(Camera *camera);

    bool move;
    float player_jump;
    bool key_forward,key_backward,key_left,key_right,key_up,key_down;
    float speed;
    float vertical_speed;

    bool right_press, move_lock;
    int mouse_x,mouse_y;
    float smooth_x,smooth_y;
    int use_x,use_y;
    int last_time;

    bool touchdown;
    unsigned long mouse_timestamp;
    bool gyro;

    Player * player;
    Map * map;
    View view;
    Vector2f gamepad_inc,gamepad_move;

    Canvas * canvas;
    //
    QRotationSensor rotation;
};

#endif // DRIVER_H
