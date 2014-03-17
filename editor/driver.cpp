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
#include "driver.h"

Driver::Driver()
{
#ifdef Q_WS_ANDROID
    move_lock=true;
#else
    move_lock=false;
#endif

    right_press=false;
    move=true;

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
    last_time=0;
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

void Driver::lockMouse(bool lock)
{
    move_lock=lock;
}

bool Driver::mouseLocked()
{
    return move_lock;
}

void Driver::setView(View view)
{
    this->view=view;
}

void Driver::right(GLfloat dist,Vertex3f & pos)
{
    pos+=player->getRotation()*Vertex3f(dist,0,0);
}

void Driver::forward(GLfloat dist, Vertex3f &pos)
{
    if(view==PLAYER)
        pos-=player->getRotation()*Vertex3f(0,0,dist);
    else
        pos-=player->getCamera()->getRotation()*Vertex3f(0,0,dist);
}

void Driver::rotateY(GLfloat angle)
{
    float tmp=player->getCamera()->getPitch()+angle;
    if(tmp<M_PI_2 && tmp>-M_PI_2)
    {
        player->getCamera()->setPitch(tmp);
        player->setRotation(player->getCamera()->getRotation());
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
    player->setRotation(player->getCamera()->getRotation());
}

void Driver::update(int elapsed_time)
{
    if(player==NULL && !move_lock)
        return;

    float delta=elapsed_time-last_time;
    last_time=elapsed_time;

    if(!move && view!=PLAYER)
        return;

    move=false;

    Vertex3f tmp(player->getCamera()->getPosition()),pos(tmp);

    if(key_forward)
    {
        forward(speed*delta,tmp);
        move=true;
    }
    if(key_backward)
    {
        forward(-speed*delta,tmp);
        move=true;
    }
    if(key_left)
    {
        right(-speed*delta,tmp);
        move=true;
    }

    if(key_right)
    {
        right(speed*delta,tmp);
        move=true;
    }

    if(key_up)
    {
        tmp[1]+=speed*delta;
        move=true;
    }

    if(key_down)
    {
        tmp[1]-=speed*delta;
        move=true;
    }

    if(view==PLAYER)
    {
        if(player_jump>0.01)
        {
            tmp[1]+=player_jump*0.01*delta;
            player_jump-=0.01*delta;
        }

        Vertex3f anchor(tmp);
        anchor-=pos;
        float l=anchor.length();
        if(l>0.0)
        {
            if(l<=4.0)
            {
                anchor.normalize();
                anchor*=4.0;
            }

            anchor+=pos;

            Collision hit = map->collision(pos,anchor);
            if(hit.object!=NULL)
            {
               // hit.collision.print();
                Vertex3f t=hit.collision;
                t-=pos;
                l=t.length();

                tmp=hit.collision;
                if(l<=4.0)
                {
                    t=anchor;
                    t-=pos;
                    t.normalize();
                    t*=4.0;
                    tmp-=t;
                }
            }
        }

        anchor=tmp;
        anchor[1]-=4.0+0.01*delta;
        Collision ground = map->collision(tmp,anchor);
        tmp=ground.collision;

        if(ground.object==NULL)
            touchdown=false;
        else
            touchdown=true;

        tmp[1]+=4.0;


        if(tmp[0]<0)
            tmp[0]=0;
        if(tmp[1]<0)
            tmp[1]=0;
        if(tmp[2]<0)
            tmp[2]=0;

        if(tmp[0]>map->maxDepth())
            tmp[0]=map->maxDepth();
        if(tmp[1]>map->maxHeight())
            tmp[1]=map->maxHeight();
        if(tmp[2]>map->maxWidth())
            tmp[2]=map->maxWidth();
    }

    //player->setPosition(tmp);
    player->getCamera()->setPosition(tmp);
}

void Driver::gamepadMotion(float deviation, float angle)
{
}

void Driver::gamepadInclination(float deviation, float angle)
{
    rotateY(angle*deviation);
}

bool Driver::mouseMoveEvent(QMouseEvent *e)
{
    if(right_press)
    {
        move_lock=false;

        rotateX((mouse_x - e->x())*0.01);
        rotateY((mouse_y - e->y())*0.01);

        mouse_x=e->x();
        mouse_y=e->y();
        move=true;
    }
    else if(move_lock)
    {
#ifndef Q_WS_ANDROID
        int x=player->getCamera()->width()/2;
        int y=player->getCamera()->height()/2;

        if(x!=e->x() || y!=e->y())
        {
            rotateX((x - e->x())*0.01);
            rotateY((y - e->y())*0.01);
            move=true;
            QPoint pos = canvas->mapToGlobal(QPoint(x,y));
            QCursor::setPos(pos.x(),pos.y());
        }
#else
        rotateX((mouse_x - e->x())*0.01);
        rotateY((mouse_y - e->y())*0.01);

        mouse_x=e->x();
        mouse_y=e->y();
        move=true;
#endif
    }
    move=true;
    return true;
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
            QPoint pos=canvas->mapToGlobal(QPoint(0,0));
            QCursor::setPos(pos.x()+mouse_x,pos.y()+mouse_y);

            canvas->releaseMouse();
        }
        else
            move_lock=true;

        return true;
    }
#ifdef Q_WS_ANDROID
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
            QPoint pos=canvas->mapToGlobal(QPoint(0,0));
            QCursor::setPos(pos.x()+x,pos.y()+y);

            canvas->captureMouse();
        }
        return true;
    }
    return false;
}

bool Driver::wheelEvent(QWheelEvent *e)
{
    Qt::KeyboardModifiers keyMod = QApplication::keyboardModifiers();
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

bool Driver::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_W:
    case Qt::Key_Up:
        key_forward=true;
        move=true;
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        key_backward=true;
        move=true;
        break;
    case Qt::Key_A:
    case Qt::Key_Left:
        key_left=true;
        move=true;
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        key_right=true;
        move=true;
        break;
    case Qt::Key_F:
        key_up=true;
        move=true;
        break;
    case Qt::Key_C:
        key_down=true;
        move=true;
        break;
    case Qt::Key_Space:
        if(touchdown)
            player_jump=3;
        break;
    default:
        return false;
        break;
    }
    return true;
}

bool Driver::keyReleaseEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_W:
    case Qt::Key_Up:
        key_forward=false;
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        key_backward=false;
        break;
    case Qt::Key_A:
    case Qt::Key_Left:
        key_left=false;
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        key_right=false;
        break;
    case Qt::Key_F:
        key_up=false;
        break;
    case Qt::Key_C:
        key_down=false;
        break;
    default:
        return false;
        break;
    }
    return true;
}
