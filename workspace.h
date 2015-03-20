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
#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QObject>
#include <QScreen>
#include "canvas.h"
#include "project.h"
#include "editor/toolbox.h"
#include "./lib/render/scenerendererdeferred.h"
#include "./lib/render/scenerendererforward.h"
#include "gamepad/gamepad.h"

class Workspace : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(Canvas* canvas READ canvas WRITE setCanvas NOTIFY canvasChanged)
    Q_PROPERTY(float speed READ speed WRITE setSpeed NOTIFY speedChanged)
    Q_PROPERTY(float gamepadSize READ gamepadSize WRITE setGamepadSize NOTIFY gamepadSizeChanged)
    Q_PROPERTY(bool play READ play WRITE setPlay NOTIFY playChanged)
public:
    explicit Workspace(QQuickItem *parent = 0);

    void unloadProject();
    void setProject(Project * project);

    void showWireframe(bool checked);
    void showLight(bool checked);
    void showShadow(bool checked);
    void renderVr(bool enable);


    bool eventFilter(QObject *obj, QEvent *event);
    void wheelEvent(QWheelEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
    void hoverMoveEvent(QHoverEvent *e);
    void gamepadEvent(GamepadEvent * e);

    void timerEvent(QTimerEvent *);

    float gamepadSize() const;
    bool play() const;
    Canvas* canvas() const;
    float speed() const;

signals:

    void canvasChanged(Canvas* arg);
    void speedChanged(float arg);
    void selectScreenhotPath();

    void gamepadSizeChanged(float arg);

    void playChanged(bool arg);

public slots:
    void undo();
    void redo();
    void selectTool(int index);
    void makeSnapshot();
    void goUp(bool move);
    void goDown(bool move);

    void setCanvas(Canvas* arg);
    void setSpeed(float arg);
    void addTool(ToolControl * tool);

    void gamepadJoyStickLeft(QVector2D angle);
    void gamepadJoyStickRight(QVector2D angle);
    void screenShotDone(Image *img);
    void saveScreenShot(const QUrl & path);
    void canvasResized(int w, int h);
    void setGamepadSize(float arg);

    void passGamepadEvent(GamepadEvent * event);

    void setPlay(bool arg);

private:

    Driver driver;
    Player player;
    Project *project;
    Toolbox editor;
    Image * screen_shot;
    Canvas* m_canvas;
    float m_speed;
    int timer_id;
    QElapsedTimer time;
    float m_gamepadSize;
    Gamepad gamepad;
    bool m_play;
};

#endif // WORKSPACE_H
