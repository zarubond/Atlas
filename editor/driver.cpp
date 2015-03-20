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

#include "driver.h"

Driver::Driver()
{
#ifdef OS_ANDROID
    move_lock=true;
#else
    move_lock=false;
#endif

    right_press=false;
    move=false;

    speed=0.01;

    key_forward=false;
    key_backward=false;
    key_left=false;
    key_right=false;
    key_down=false;
    key_up=false;

    player_jump=0.0;
    player=NULL;
    touchdown=true;
    view=FREE;

    mouse_x=0;
    mouse_y=0;
    smooth_x=0;
    smooth_y=0;
    last_time=0;
    vertical_speed=0;

    gyro=false;
}

void Driver::setMap(Map *map, Canvas *canvas)
{
    this->map=map;
    this->canvas=canvas;
}

void Driver::setPlayer(Player *player)
{
    this->player=player;
}

void Driver::setGyro(bool enable)
{
    if(enable)
        rotation.start();
    gyro=enable;
}

void Driver::lockMouse(bool lock)
{
    move_lock=lock;
}

bool Driver::mouseLocked()
{
    return move_lock;
}

void Driver::goUp(bool move)
{
    if(move)
    {
        key_up=true;
        this->move=true;
    }
    else
    {
        key_up=false;
    }
}

void Driver::goDown(bool move)
{
    if(move)
    {
        key_down=true;
        this->move=true;
    }
    else
        key_down=false;
}

void Driver::setView(View view)
{
    this->view=view;
}

void Driver::moveSide(GLfloat dist,Vector3f & pos)
{
    Vector3f tmp=player->rotation()*Vector3f(dist,0,0);
    tmp*=fabs(dist);
    pos+=tmp;
}

void Driver::moveDirect(GLfloat dist, Vector3f &pos)
{
    Vector3f tmp;
    if(view==PLAYER)
        tmp=player->rotation()*Vector3f(0,0,dist);
    else
        tmp=player->getCamera()->rotation()*Vector3f(0,0,dist);

    tmp*=fabs(dist);
    pos-=tmp;
}

void Driver::updateGyro(Camera * camera)
{
    QRotationReading * read=rotation.reading();
    camera->setRotation(Quaternion((-read->y()-90.0)*DEG_TO_RAD, (read->z())*DEG_TO_RAD,(read->x())*DEG_TO_RAD));
}

void Driver::rotateY(GLfloat angle)
{
    float tmp=player->getCamera()->getPitch()+angle;
    if(tmp<M_PI_2 && tmp>-M_PI_2)
    {
        player->getCamera()->setPitch(tmp);
        player->setRotation(player->getCamera()->rotation());
    }
}

void Driver::rotateX(GLfloat angle)
{
    float angle_x=player->getCamera()->getYaw()+angle;
    if(angle_x<0)
        angle_x=(M_PI*2)+angle_x;
    if(angle_x>M_PI*2)
        angle_x=angle_x-(M_PI*2);

    player->getCamera()->setYaw(angle_x);
    player->setRotation(player->getCamera()->rotation());
}
float Driver::getSpeed() const
{
    return speed;
}

void Driver::setSpeed(float value)
{
    speed = value;
}

float Driver::maximumSpeed()
{
    return 1;
}

void Driver::update(int elapsed_time)
{
    if(player==NULL && !move_lock)
        return;

    float delta=elapsed_time-last_time;
    last_time=elapsed_time;

    if(gyro)
    {
        updateGyro(player->getCamera());
    }

    if(!move && view!=PLAYER)
        return;

    move=false;

    Vector3f target(player->getCamera()->position()),pos(target);

    if(key_forward)
    {
        moveDirect(speed*delta,target);
        move=true;
    }
    if(key_backward)
    {
        moveDirect(-speed*delta,target);
        move=true;
    }
    if(key_left)
    {
        moveSide(-speed*delta,target);
        move=true;
    }

    if(key_right)
    {
        moveSide(speed*delta,target);
        move=true;
    }

    if(key_up)
    {
        target[1]+=speed*delta;
        move=true;
    }

    if(key_down)
    {
        target[1]-=speed*delta;
        move=true;
    }

    if(!gyro && (gamepad_inc[0]!=0.0 || gamepad_inc[1]!=0.0))
    {
        rotateX(-gamepad_inc[0]*0.001*delta);
        rotateY(-gamepad_inc[1]*0.001*delta);
        move=true;
    }

    if(gamepad_move[0]!=0.0 || gamepad_move[1]!=0.0)
    {
        moveSide(gamepad_move[0]*speed*delta,target);
        moveDirect(-gamepad_move[1]*speed*delta,target);
        move=true;
    }

    if(view==PLAYER)
    {
        float height=player->height();
        float gravity_accel=98;//m/s^2
        float terminal_speed=1000;//m/s^2
        float max_step=0.7;
        Vector3f dir=target-pos;
        dir.normalize();
        target+=dir;

        pos[1]-=height-max_step;
        target[1]-=height-max_step;

        Collision front=map->collision(pos,target);
        target=front.collision-dir;

        Vector3f to=target,from=target;
        to[1]-=vertical_speed*(delta/1000.0)+max_step;
        Collision bottom=map->collision(from,to);
        target=bottom.collision;
        target[1]+=height;

        if(bottom.object!=NULL)
        {
            vertical_speed=0.0;
        }
        else
        {
            if(vertical_speed<terminal_speed)
                vertical_speed+=gravity_accel*(delta/1000.0);
        }

        if(target[0]<0)
            target[0]=0;
        if(target[1]<0)
            target[1]=0;
        if(target[2]<0)
            target[2]=0;

        if(target[0]>map->maxDepth())
            target[0]=map->maxDepth();
        if(target[1]>map->maxHeight())
            target[1]=map->maxHeight();
        if(target[2]>map->maxWidth())
            target[2]=map->maxWidth();

    }

    player->getCamera()->setPosition(target);
}

void Driver::gamepadOne(Vector2f angle)
{
    if(ControlSettings::gamepad_swap)
    {
        gamepad_inc=angle;
        move=true;
    }
    else
    {
        gamepad_move=angle;
        move=true;
    }
}

void Driver::gamepadTwo(Vector2f angle)
{
    if(ControlSettings::gamepad_swap)
    {
        gamepad_move=angle;
        move=true;
    }
    else
    {
        gamepad_inc=angle;
        move=true;
    }
}

bool Driver::hoverMoveEvent(QHoverEvent *e)
{
    if(move_lock)
    {
#ifndef Q_OS_ANDROID
        int x=canvas->width()/2.0;
        int y=canvas->height()/2.0;

        if(x!=e->pos().x() || y!=e->pos().y())
        {
            rotateX((x - e->pos().x())*0.01);
            rotateY((y - e->pos().y())*0.01);
            move=true;
            QPointF point=canvas->mapFromScene(QPointF(0,0));
            QPoint pos =canvas->window()->mapToGlobal(QPoint(x-point.x(),y-point.y()));
            QCursor::setPos(pos.x(),pos.y());
        }
#else
        rotateX((mouse_x - e->pos().x())*0.01);
        rotateY((mouse_y - e->pos().y())*0.01);

        mouse_x=e->pos().x();
        mouse_y=e->pos().y();
        move=true;
#endif
    }
    return true;
}

bool Driver::mouseMoveEvent(QMouseEvent *e)
{
    if(right_press)
    {
        move_lock=false;

        rotateX((mouse_x - e->pos().x())*0.02);
        if(ControlSettings::mouse_inverse)
            rotateY((mouse_y + e->pos().y())*0.02);
        else
            rotateY((mouse_y - e->pos().y())*0.02);

        mouse_x=e->pos().x();
        mouse_y=e->pos().y();
        move=true;
        return true;
    }
    return false;
}

bool Driver::mousePressEvent(QMouseEvent *e)
{
    if(e->button()==Qt::RightButton)
    {
        mouse_x=e->x();
        mouse_y=e->y();

        right_press=true;

        if(canvas->isCaptured())
        {
            move_lock=false;
            int x=canvas->width()/2,y=canvas->height()/2;

            QPoint pos=canvas->window()->mapToGlobal(QPoint(0,0));
            QCursor::setPos(pos.x()+mouse_x,pos.y()+mouse_y);

            canvas->releaseMouse();
        }
        else
            move_lock=true;

        return true;
    }
#ifdef Q_OS_ANDROID
    mouse_x=e->x();
    mouse_y=e->y();
#endif
    return false;
}

bool Driver::mouseReleaseEvent(QMouseEvent * e)
{
    if(e->button()==Qt::RightButton)
    {
        right_press=false;
        if(move_lock)
        {
            int x=canvas->width()/2,y=canvas->height()/2;
            QPointF point=canvas->mapFromScene(QPointF(0,0));
            QPoint pos = canvas->window()->mapToGlobal(QPoint(x-point.x(),y-point.y()));

            QCursor::setPos(pos.x(),pos.y());

            canvas->captureMouse();
        }
        return true;
    }
    return false;
}

bool Driver::wheelEvent(QWheelEvent *e)
{
    Qt::KeyboardModifiers keyMod = QGuiApplication::keyboardModifiers();
    if(keyMod.testFlag(Qt::AltModifier))
    {
        float tmp=speed+e->delta()*0.00001 ;

        if(tmp<0.001)
            speed=0.001;
        else if(tmp>0.4)
            speed=0.4;
        else
            speed=tmp;
        return true;
    }
    return false;
}

bool Driver::gamepadEvent(GamepadEvent *e)
{
    if(e->type()==GamepadEvent::AXIS)
    {
        switch(e->pivot())
        {
        case GamepadEvent::AXIS1_X:
            gamepad_inc[0]=e->axisValue();
            move=true;
            break;
        case GamepadEvent::AXIS1_Y:
            gamepad_inc[1]=e->axisValue();
            move=true;
            break;
        case GamepadEvent::AXIS0_X:
            gamepad_move[0]=e->axisValue();
            move=true;
            break;
        case GamepadEvent::AXIS0_Y:
            gamepad_move[1]=e->axisValue();
            move=true;
            break;
        default:break;
        }
    }
    return false;
}

bool Driver::keyPressEvent(QKeyEvent *e)
{
    //switch must be constant :(
    if(ControlSettings::key_forward==e->key())
    {
        key_forward=true;
        move=true;
    }
    else if(ControlSettings::key_backward==e->key())
    {
        key_backward=true;
        move=true;
    }
    else if(ControlSettings::key_left==e->key())
    {
        key_left=true;
        move=true;
    }
    else if(ControlSettings::key_right==e->key())
    {
        key_right=true;
        move=true;
    }
    else if(ControlSettings::key_up==e->key())
    {
        key_up=true;
        move=true;
    }
    else if(ControlSettings::key_down==e->key())
    {
        key_down=true;
        move=true;
    }
    else if(ControlSettings::key_jump==e->key())
    {
        if(vertical_speed==0.0)
            vertical_speed=-30;
    }
    else
        return false;


    return true;
}

bool Driver::keyReleaseEvent(QKeyEvent *e)
{
    //switch must be constant :(
    if(ControlSettings::key_forward==e->key())
    {
        key_forward=false;
    }
    else if(ControlSettings::key_backward==e->key())
    {
        key_backward=false;
    }
    else if(ControlSettings::key_left==e->key())
    {
        key_left=false;
    }
    else if(ControlSettings::key_right==e->key())
    {
        key_right=false;
    }
    else if(ControlSettings::key_up==e->key())
    {
        key_up=false;
    }
    else if(ControlSettings::key_down==e->key())
    {
        key_down=false;
    }
    else
        return false;

    return true;
}
