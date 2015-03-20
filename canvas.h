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
#ifndef CANVAS_H
#define CANVAS_H

#include <QQuickView>
#include <QQuickItem>
#include <QGuiApplication>
#include <QPalette>
#include <QPainter>
#include <QSGSimpleTextureNode>
#include <QOpenGLFramebufferObject>
#include <QTime>
#include <QMutex>

#include "gamepad/gamepadevent.h"

#include "./lib/render/scenerender.h"
#include "./lib/render/scenerendererdeferred.h"
#include "./lib/render/scenerendererforward.h"
#include "./lib/graphics/opengl.h"
#include "./lib/tools/camera.h"
#include "./lib/tools/image.h"
#include "./lib/worldgraphics.h"


//http://horde3d.org/wiki/index.php5?title=Tutorial_-_Setup_Horde_with_Qt5_%26_QtQuick_2.1
//http://stackoverflow.com/questions/20606732/give-subclass-of-qquickitem-pointer-to-another-c-object
//http://stackoverflow.com/questions/19455518/periodically-redraw-qquickitem
//http://qt-project.org/doc/qt-5/qtquick-scenegraph-openglunderqml-example.html
class Canvas : public QQuickItem,OpenGL
{
    Q_OBJECT
    Q_PROPERTY(int fps READ fps  NOTIFY fpsChanged)
public:
    explicit Canvas(QQuickItem *parent = 0);
    ~Canvas();

    void timerEvent(QTimerEvent* evt);
    const Camera *getCamera() const;
    void setCamera(const Camera &value);
    void setWorld(WorldGraphics * world);

    void captureMouse();
    bool isCaptured();
    void releaseMouse();
    //this is HACK to compensate for https://bugreports.qt-project.org/browse/QTBUG-32004
    void wheelEvent(QWheelEvent * event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent * event);
    void hoverMoveEvent(QHoverEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent * event);
    void installEventFilter(QObject * object);

    int fps() const;

    void makeSnapshot();
    QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *);
    /**
     * @brief showVR this switch will enable vr projection, tj. will project one image pro each eye side by side.
     * @param enable
     */
    void renderVr(bool enable);


signals:
    void fpsChanged(int);
    void resized(int w, int h);
    void screenShotDone(Image * img);

private slots:
    void resize();
    void cleanup();
    void visibleChanged();
private:
    void updateFps();
    void createScreenShot();
    void createFrameBuffer();

    SceneRender * renderer;
    int m_timerID;
    int m_fps;
    int fps_cnt;
    bool init;
    bool _resized;
    float w,h;

    Camera camera;
    bool mouse_grab;
    bool make_screen_shot;
    bool render_vr;

    GLuint depth_rb;

    QObject * event_filter;
    QMutex mutex;
    QTime time;
    int last_time;
    QOpenGLFramebufferObject* m_fbo;
    QSGTexture * texture;
    WorldGraphics *world;

};

#endif // CANVAS_H
