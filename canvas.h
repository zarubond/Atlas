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
#ifndef SPACE_H
#define SPACE_H

#include <QGLWidget>
#include <QApplication>

#include <QTimer>
#include <QTime>
#include <QLCDNumber>
#include <QShortcut>

#include <QBasicTimer>
#include <QMouseEvent>
#include <QDebug>

#include <math.h>
#include <locale.h>

#include "tools/graphics.h"
#include "tools/math/math.h"
#include "editor/navigator.h"
#include "project/project.h"

#include "world/mapdrawer.h"
#include "editor/player.h"

#include "world/module/skybox/skybox.h"

#ifndef OPENGL_ES
#include "postprocess.h"
#endif
/**
 * @brief The Canvas class is place for drawing.
 */
class Canvas : public QGLWidget
{
     Q_OBJECT
public:
    explicit Canvas(QWidget *parent = 0);
    virtual ~Canvas();
    /**
     * @brief init Initialize all required things.
     * @return
     */
    bool init();
    /**
     * @brief setNavigator Set navigator hand.
     * @param navig
     */
    void setNavigator(Navigator * navig);
    /**
     * @brief setProject Set used project.
     * @param project
     */
    void setProject(Project * project);

    GLfloat getDepth(unsigned x, unsigned y);
    /**
     * @brief showWireframe Switch wireframe
     * @param wireframe
     */
    void showWireframe(bool wireframe);
    /**
     * @brief showLights Switch lights
     * @param lights
     */
    void showLights(bool lights);
    /**
     * @brief showShadows Switch shadows
     * @param shadows
     */
    void showShadows(bool shadows);
    /**
     * @brief isShadows Are shadown on?
     * @return
     */
    bool isShadows();
    /**
     * @brief fps Number of frames rendered in second.
     * @return
     */
    int fps();
    /**
     * @brief isWireframe Is wireframe visible?
     * @return
     */
    bool isWireframe();
    /**
     * @brief isLights
     * @return
     */
    bool isLights();
    /**
     * @brief capture Capture mouse to the space.
     */
    void captureMouse();
    /**
     * @brief release Release mouse from space.
     */
    void releaseMouse();
    /**
     * @brief isCaptured Is mouse captered?
     * @return
     */
    bool isCaptured();

    MapDrawer drawer;

    Camera *getCamera() const;
    void setCamera(Camera *value);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

private:

    Camera * camera;
    Project * project;
    SkyBox skybox;

    void render(const Matrix4f &mvp, int elapsed);
    void renderScene(const Matrix4f &mvp);
    //to be http://organicvectory.com/index.php?option=com_content&view=article&id=54
    Matrix4f renderShadow(int elapsed);
    bool loadMapBox();
    void renderBox(const Matrix4f & mvp);

    //wireframe map visualization
    bool wireframe_switch;
    bool mouse_grab;
    //fps counter
    QTime m_time;

    Navigator * navig;

#ifndef OPENGL_ES
    PostProcess postpro;
#endif
    Matrix4f lastMVP;
    int frame_count;
    int fps_count;
    int time_sec;
    int ls2;

    bool post_poscessing;
    bool render_shadow;
    bool init_gl;

    //BOX
    Shader program;
    GLint uniform_mvp,uniform_pos,uniform_size;
    GLint attribute_coord3d;
    GLuint vbo_vertices,vbo_cube;
    int triangle_num,cube_num;
};


#endif // SPACE_H
