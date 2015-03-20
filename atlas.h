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
#ifndef ATLAS_H
#define ATLAS_H

#include <QObject>
#include <QQmlEngine>
#include <QTimer>
#include "window.h"

//http://httpwww.ics.com/blog/integrating-c-qml#.U53v-f7ZJUQ
class Atlas : public QObject
{
    Q_OBJECT
public:
    explicit Atlas(QObject *parent = 0);
    ~Atlas();
    bool start();
public slots:
    void handleLoggedMessage(const QOpenGLDebugMessage &debugMessage);
private:
    bool expired();
private slots:
    void initGL();
    void close(QQuickCloseEvent*);
    void init();

private:
    void timerEvent(QTimerEvent *);
    Window * atlas;
    GlobalSettings settings;
    Assets assets;

    QOpenGLContext * main_context;
    QOffscreenSurface * fake_surface;

    QQmlEngine engine;
    QQmlComponent *component;
    QObject *topLevel;
    QQuickWindow *window;
    QOpenGLDebugLogger *logger;
};

#endif // ATLAS_H
