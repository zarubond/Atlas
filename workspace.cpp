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

#include "workspace.h"

Workspace::Workspace(QQuickItem *parent) :
    QQuickItem(parent)
{
    screen_shot=NULL;
    m_canvas=NULL;
    m_speed=0.1;
    project=NULL;
    driver.setSpeed(m_speed*driver.maximumSpeed());
    driver.setPlayer(&player);
    timer_id=0;
    m_gamepadSize=0;
    m_play=false;

    time.start();
    connect(&gamepad,SIGNAL(gamepadEvent(GamepadEvent*)),this,SLOT(passGamepadEvent(GamepadEvent*)));
}

void Workspace::unloadProject()
{
    killTimer(timer_id);
    timer_id=0;
    editor.unloadProject();
    m_canvas->setWorld(NULL);
}

void Workspace::setProject(Project *project)
{
    editor.unloadProject();
    editor.setProject(project);

    m_canvas->setWorld(&project->world);
    this->project=project;
    if(m_canvas!=NULL)
    driver.setMap(&project->world.map,m_canvas);

    timer_id=startTimer(16);
}

void Workspace::undo()
{
    editor.undo();
}

void Workspace::redo()
{
    editor.redo();
}

void Workspace::selectTool(int index)
{
    editor.selectTool(index);
}

void Workspace::makeSnapshot()
{
    if(m_canvas!=NULL)
        m_canvas->makeSnapshot();
}

void Workspace::goUp(bool move)
{
    driver.goUp(move);
}

void Workspace::goDown(bool move)
{
    driver.goDown(move);
}

void Workspace::setCanvas(Canvas *arg)
{
    if (m_canvas != arg) {
        m_canvas = arg;
        if(m_canvas!=NULL){
            connect(m_canvas,SIGNAL(screenShotDone(Image *)),this,SLOT(screenShotDone(Image*)));
            m_canvas->installEventFilter(this);

            m_canvas->setCamera(*(player.getCamera()));
            canvasResized(m_canvas->width(),m_canvas->height());
            connect(m_canvas,SIGNAL(resized(int,int)),this,SLOT(canvasResized(int,int)));
            if(project!=NULL)
                driver.setMap(&project->world.map,m_canvas);
        }
        emit canvasChanged(arg);
        /*
        QScreen * screen=QGuiApplication::primaryScreen();
        cout<<screen->physicalSize().width()<<endl;
        cout<<screen->geometry().width()/screen->physicalSize().width()<<endl;
        cout<<screen->physicalDotsPerInch()<<endl;

        foreach (QScreen *screen, QGuiApplication::screens()) {
            cout<< "Information for screen:" << screen->name()<<endl;
            cout << "  Available geometry:" << screen->availableGeometry().x() << screen->availableGeometry().y() << screen->availableGeometry().width() << "x" << screen->availableGeometry().height()<<endl;
            cout << "  Available size:" << screen->availableSize().width() << "x" << screen->availableSize().height()<<endl;
            cout << "  Available virtual geometry:" << screen->availableVirtualGeometry().x() << screen->availableVirtualGeometry().y() << screen->availableVirtualGeometry().width() << "x" << screen->availableVirtualGeometry().height()<<endl;
            cout << "  Available virtual size:" << screen->availableVirtualSize().width() << "x" << screen->availableVirtualSize().height()<<endl;
            cout << "  Depth:" << screen->depth() << "bits"<<endl;
            cout << "  Geometry:" << screen->geometry().x() << screen->geometry().y() << screen->geometry().width() << "x" << screen->geometry().height()<<endl;
            cout << "  Logical DPI:" << screen->logicalDotsPerInch()<<endl;
            cout << "  Logical DPI X:" << screen->logicalDotsPerInchX()<<endl;
            cout << "  Logical DPI Y:" << screen->logicalDotsPerInchY()<<endl;
           // qDebug() << "  Orientation:" << Orientation(screen->orientation());
            cout << "  Physical DPI:" << screen->physicalDotsPerInch()<<endl;
            cout << "  Physical DPI X:" << screen->physicalDotsPerInchX()<<endl;
            cout << "  Physical DPI Y:" << screen->physicalDotsPerInchY()<<endl;
            cout << "  Physical size:" << screen->physicalSize().width() << "x" << screen->physicalSize().height() << "mm"<<endl;
           // qDebug() << "  Primary orientation:" << Orientation(screen->primaryOrientation());
            cout << "  Refresh rate:" << screen->refreshRate() << "Hz"<<endl;
            cout << "  Size:" << screen->size().width() << "x" << screen->size().height()<<endl;
            cout << "  Virtual geometry:" << screen->virtualGeometry().x() << screen->virtualGeometry().y() << screen->virtualGeometry().width() << "x" << screen->virtualGeometry().height()<<endl;
            cout << "  Virtual size:" << screen->virtualSize().width() << "x" << screen->virtualSize().height()<<endl;
        }*/
    }
}

void Workspace::setSpeed(float arg)
{
    if (m_speed != arg) {
        m_speed = arg;
        driver.setSpeed(m_speed*driver.maximumSpeed());
        emit speedChanged(arg);
    }
}

void Workspace::addTool(ToolControl *tool)
{
    editor.addTool(tool);
}

void Workspace::canvasResized(int w, int h)
{
#ifdef OS_ANDROID
    float dist=512;
#else
    float dist = 2048;
#endif
    player.getCamera()->perspective(45.0,w,h,0.5,dist);
}

void Workspace::setGamepadSize(float arg)
{
    if (m_gamepadSize == arg)
        return;

    m_gamepadSize = arg;
    emit gamepadSizeChanged(arg);
}

void Workspace::passGamepadEvent(GamepadEvent *event)
{
    gamepadEvent(event);
}

void Workspace::setPlay(bool arg)
{
    if (m_play == arg)
        return;
    m_play = arg;
    if(m_play)
        driver.setView(Driver::PLAYER);
    else
        driver.setView(Driver::FREE);
    emit playChanged(arg);
}

void Workspace::screenShotDone(Image * img)
{
    //a little bit hacky
    delete screen_shot;
    screen_shot=img;
    emit selectScreenhotPath();
}


void Workspace::saveScreenShot(const QUrl &path)
{
    screen_shot->save(path.toLocalFile().toUtf8().constData());

    delete screen_shot;
    screen_shot=NULL;
}

void Workspace::showWireframe(bool checked)
{
    if(project!=NULL)
        project->world.environment.wireframe=checked;
}

void Workspace::showLight(bool checked)
{
    if(project!=NULL)
        project->world.environment.light=checked;
}

void Workspace::showShadow(bool checked)
{
    if(project!=NULL)
        project->world.environment.shadow=checked;
}

void Workspace::renderVr(bool enable)
{
    if(m_canvas!=NULL)
    {
        m_canvas->renderVr(enable);
        driver.setGyro(enable);
    }
}

Canvas *Workspace::canvas() const
{
    return m_canvas;
}

float Workspace::speed() const
{
    return m_speed;
}

bool Workspace::eventFilter(QObject *obj, QEvent *event)
{
    switch(event->type())
    {
    case QEvent::MouseMove:
        mouseMoveEvent(static_cast<QMouseEvent *>(event));
        break;
    case QEvent::MouseButtonPress:
        mousePressEvent(static_cast<QMouseEvent *>(event));
        break;
    case QEvent::MouseButtonRelease:
        mouseReleaseEvent(static_cast<QMouseEvent *>(event));
        break;
    case QEvent::MouseButtonDblClick:
        mouseDoubleClickEvent(static_cast<QMouseEvent*>(event));
        break;
    case QEvent::HoverMove:
        hoverMoveEvent(static_cast<QHoverEvent*>(event));
        break;
    case QEvent::Wheel:
        wheelEvent(static_cast<QWheelEvent*>(event));
        break;
    case QEvent::KeyPress:
        keyPressEvent(static_cast<QKeyEvent*>(event));
        break;
    case QEvent::KeyRelease:
        keyReleaseEvent(static_cast<QKeyEvent*>(event));
        break;
    default:
        return QObject::eventFilter(obj,event);
    }

    return true;
}

void Workspace::wheelEvent(QWheelEvent *e)
{
    if(!driver.wheelEvent(e))
        editor.wheelEvent(e);
}

void Workspace::mouseMoveEvent(QMouseEvent *e)
{
    driver.mouseMoveEvent(e);
    editor.mouseMoveEvent(e);
}

void Workspace::mousePressEvent(QMouseEvent *e)
{
    if(!driver.mousePressEvent(e))
        editor.mousePressEvent(e);
}

void Workspace::mouseReleaseEvent(QMouseEvent * e)
{
    if(!driver.mouseReleaseEvent(e))
        editor.mouseReleaseEvent(e);
}

void Workspace::mouseDoubleClickEvent(QMouseEvent *e)
{
    editor.mouseDoubleClickEvent(e);
}

void Workspace::keyPressEvent(QKeyEvent *e)
{
    if(!driver.keyPressEvent(e))
        editor.keyPressEvent(e);
}

void Workspace::keyReleaseEvent(QKeyEvent *e)
{
    if(!driver.keyReleaseEvent(e))
        editor.keyReleaseEvent(e);
}

void Workspace::hoverMoveEvent(QHoverEvent *e)
{
    driver.hoverMoveEvent(e);
    editor.hoverMoveEvent(e);
}

void Workspace::gamepadEvent(GamepadEvent *e)
{
   // cout<<event->value[0]<<" "<<event->value[1]<<endl;
   // driver.gamepadOne(event->value);
    driver.gamepadEvent(e);
}

void Workspace::timerEvent(QTimerEvent *)
{
    if(m_canvas!=NULL)
        m_canvas->setCamera(*(player.getCamera()));

    if(m_gamepadSize!=ControlSettings::gamepad_size)//HACK
        setGamepadSize(ControlSettings::gamepad_size);

    int elapsed_time=time.elapsed();
    driver.update(elapsed_time);
    editor.update(elapsed_time, player);
}

float Workspace::gamepadSize() const
{
    return m_gamepadSize;
}

bool Workspace::play() const
{
    return m_play;
}

void Workspace::gamepadJoyStickLeft(QVector2D angle)
{
    driver.gamepadOne(Vector2f(angle.x(), angle.y()));
   // editor.gamepadJoyStickLeft(angle);
}

void Workspace::gamepadJoyStickRight(QVector2D angle)
{
    driver.gamepadTwo(Vector2f(angle.x(), angle.y()));
    //editor.gamepadJoyStickRight(angle);
}

