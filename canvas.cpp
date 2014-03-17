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
#include "canvas.h"

#include <QBasicTimer>
#include <QMouseEvent>
#include <QDebug>

#include <math.h>
#include <locale.h>

//#define SPEED_TEST

Canvas::Canvas(QWidget *parent) :
    QGLWidget(    QGLFormat(
                      QGL::AlphaChannel |
                      QGL::DoubleBuffer |
                      QGL::DepthBuffer |
                      QGL::Rgba |
                      QGL::StencilBuffer
                    ),parent)
{
    wireframe_switch=false;
    mouse_grab=false;

    post_poscessing=true;
    render_shadow=true;


    navig=NULL;
    frame_count=0;
    fps_count=0;
    time_sec=0;
    ls2=0;

    setMouseTracking(true);

    project=NULL;
    camera=NULL;

    init_gl=false;

    lastMVP.loadIdentity();
}

Canvas::~Canvas()
{
}

bool Canvas::init()
{
    setFocusPolicy(Qt::StrongFocus);
    glInit();

    drawer.load();
    skybox.load(1.0);

    loadMapBox();
    return true;
}

void Canvas::setNavigator(Navigator *navig)
{
    this->navig=navig;
}

void Canvas::setProject(Project *project)
{
    if(project!=NULL)
    {
        this->project=project;
        drawer.clearRenderList();
    }
    else
        this->project=NULL;
}

GLfloat Canvas::getDepth(unsigned x, unsigned y)
{
    GLfloat depth;
    glReadPixels(x, height() - y - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    return depth;
}

void Canvas::showWireframe(bool wireframe)
{
    wireframe_switch=wireframe;
}

bool Canvas::isWireframe()
{
    return wireframe_switch;
}

void Canvas::showLights(bool lights)
{
    post_poscessing=lights;
}

void Canvas::showShadows(bool shadows)
{
    render_shadow=shadows;
}

bool Canvas::isShadows()
{
    return render_shadow;
}

int Canvas::fps()
{
    return fps_count;
}

bool Canvas::isLights()
{
    return post_poscessing;
}

void Canvas::captureMouse()
{
     grabMouse();
     QApplication::setOverrideCursor( QCursor( Qt::BlankCursor ));
     int x=width()/2,y=height()/2;
     QPoint pos=mapToGlobal(QPoint(0,0));
     QCursor::setPos(pos.x()+x,pos.y()+y);
     mouse_grab=true;
}

void Canvas::releaseMouse()
{
     QGLWidget::releaseMouse();
     QApplication::setOverrideCursor( QCursor( Qt::ArrowCursor ));
     mouse_grab=false;
}

bool Canvas::isCaptured()
{
    return mouse_grab;
}

void Canvas::initializeGL()
{
#if QT_VERSION < 0x050000
    OpenGL ogl(context());
#else
    OpenGL ogl(context()->contextHandle());
#endif
    ogl.init();
#ifndef OPENGL_ES
    ogl.vSync(true);
#endif

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    //antiallising
    glEnable(GL_MULTISAMPLE);

#ifndef OPENGL_ES
    postpro.init();
#endif

    init_gl=true;
}

void Canvas::resizeGL(int w, int h)
{
    // Set OpenGL viewport to cover whole widget
    glViewport(0, 0, w, h);
    if(project!=NULL)
    {
        float dist=sqrt(project->map.depth()*project->map.depth() +
                        project->map.width()*project->map.width() +
                        project->map.height()*project->map.height());
        if(camera!=NULL) camera->perspective(45,w,h,0.05,dist);
    }
    else
        if(camera!=NULL) camera->perspective(45.0,w,h,0.05,1000.0);
#ifndef OPENGL_ES
    if(init_gl)
        postpro.resize(w,h);
#endif
#ifdef SPEED_TEST
    std::cout<<"Resolution: "<<w<<"x"<<h<<" px--------------"<<std::endl;
#endif
}

void Canvas::paintGL()
{
    if(project==NULL)
        return;

    if (frame_count == 0)
    {
         m_time.start();
         time_sec=0;
         frame_count=0;
         ls2=m_time.elapsed();
    }
    else
    {
        if(m_time.elapsed()-time_sec>=1000)
        {
            fps_count=frame_count;
#ifdef Q_WS_ANDROID
            qDebug()<<frame_count;
#endif
            frame_count=0;
            time_sec=m_time.elapsed();
        }
    }

    frame_count++;

    float t=m_time.elapsed()-ls2;
    ls2=m_time.elapsed();

#ifdef SPEED_TEST
    QElapsedTimer timer;

    glFinish();
    timer.start();
#endif
   // post_poscessing=false;
    if(post_poscessing)
    {
#ifndef OPENGL_ES

        Matrix4f shad;

        if(render_shadow)
        {
            postpro.shadow();
            shad=renderShadow(m_time.elapsed());
#ifdef SPEED_TEST
            glFinish();
            std::cout<<"Shadow time:"<<timer.elapsed()<<"ms"<<std::endl;
            timer.restart();
#endif
        }

        postpro.process(m_time.elapsed());
       // glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
      //  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );


        glClear(GL_COLOR_BUFFER_BIT);
        //glClearColor(0.7,1.0,1.0,1.0);
//glClearDepth(1.0);
        skybox.render(camera->getLook(),camera->getPosition());
        //glClear(GL_DEPTH_BUFFER_BIT);
      //  glClearDepth(1.0);
        glClear(GL_DEPTH_BUFFER_BIT);
        renderScene(camera->getLook());
        renderBox(camera->getLook());

        glDepthMask(GL_FALSE);
        //glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        postpro.render(project->light,camera->getPosition(),shad);
        glDepthMask(GL_TRUE);
#ifdef SPEED_TEST
            glFinish();
            std::cout<<"Render time with light:"<<timer.elapsed()<<"ms"<<std::endl;
            timer.restart();
#endif

#else

        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glClearColor(0.7,1.0,1.0,1.0);

        renderBox(current_camera.getLook());
        renderScene(current_camera.getLook());
#ifdef SPEED_TEST
            glFinish();
            std::cout<<"Render time:"<<timer.elapsed()<<"ms"<<std::endl;
            timer.restart();
#endif

#endif

    }
    else
    {
        //render scene
        //glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        //glClearColor(0.7,1.0,1.0,1.0);
         skybox.render(camera->getLook(),camera->getPosition());
        // glClearDepth(1.0);
        glClear(GL_DEPTH_BUFFER_BIT);
        renderBox(camera->getLook());
        renderScene(camera->getLook());

#ifdef SPEED_TEST
            glFinish();
            std::cout<<"Render time:"<<timer.elapsed()<<"ms"<<std::endl;
            timer.restart();
#endif
    }

    OpenGL::Error();
}
Camera *Canvas::getCamera() const
{
    return camera;
}

void Canvas::setCamera(Camera *value)
{
    camera = value;
    camera->setPitch(-M_PI_4);
    camera->setYaw(-M_PI_2);
}

void Canvas::render(const Matrix4f &mvp,int elapsed)
{
    drawer.render(mvp,elapsed);

    if(navig!=NULL)
        navig->render(mvp);
}

void Canvas::renderScene(const Matrix4f &mvp)
{
    #ifndef Q_WS_ANDROID
            if(wireframe_switch)
                glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    #endif
            //if(!(lastMVP==mvp))
            {
                drawer.clearRenderList();
                project->map.buildRenderList(*camera);
                lastMVP=mvp;
            }
            render(mvp,m_time.elapsed());


    #ifndef Q_WS_ANDROID
            if(wireframe_switch)
                glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    #endif
}

Matrix4f Canvas::renderShadow(int elapsed)
{
    //TODO RENDER LIST ACCORDING TO LIGHT VIEW POSITION
    project->map.buildRenderList(*camera);

    Vertex3f dir=project->light.sun.getDirection();
    dir.inverse();

    Matrix4f view;
    view.lookAt(dir,Vertex3f(0,0,0),Vertex3f(0,1,0));

    Vertex4f vec[8];

    vec[0].set(0,0,0,1.0f);
    vec[1].set(project->map.width(),0,0,1.0f);
    vec[2].set(0,project->map.height(),0,1.0f);
    vec[3].set(0,0,project->map.depth(),1.0f);

    vec[4].set(project->map.width(),project->map.height(),0,1.0f);
    vec[5].set(project->map.width(),0,project->map.depth(),1.0f);
    vec[6].set(0,project->map.height(),project->map.depth(),1.0f);
    vec[7].set(project->map.width(),project->map.height(),project->map.depth(),1.0f);

    /*
    vec[0].set(current_camera.ntl[0],current_camera.ntl[1],current_camera.ntl[2],1.0f);
    vec[1].set(current_camera.ntr[0],current_camera.ntr[1],current_camera.ntr[2],1.0f);
    vec[2].set(current_camera.nbl[0],current_camera.nbl[1],current_camera.nbl[2],1.0f);
    vec[3].set(current_camera.nbr[0],current_camera.nbr[1],current_camera.nbr[2],1.0f);

    vec[4].set(current_camera.ftl[0],current_camera.ftl[1],current_camera.ftl[2],1.0f);
    vec[5].set(current_camera.ftr[0],current_camera.ftr[1],current_camera.ftr[2],1.0f);
    vec[6].set(current_camera.fbl[0],current_camera.fbl[1],current_camera.fbl[2],1.0f);
    vec[7].set(current_camera.fbr[0],current_camera.fbr[1],current_camera.fbr[2],1.0f);
*/

    Vertex4f tmp=view*vec[0];
    Vertex3f min(tmp[0],tmp[1],tmp[2]),max(tmp[0],tmp[1],tmp[2]);

    for(int i=1;i<8;i++)
    {
        tmp=view*vec[i];

        if (tmp[0] > max[0])
            max[0] = tmp[0];
        else if (tmp[0] < min[0])
            min[0] = tmp[0];

        if (tmp[1] > max[1])
            max[1] = tmp[1];
        else if (tmp[1] < min[1])
            min[1] = tmp[1];

        if (tmp[2] > max[2])
            max[2] = tmp[2];
        else if (tmp[2] < min[2])
            min[2] = tmp[2];
    }

    Matrix4f proj;
    proj.ortho(min[0],max[0],min[1],max[1], -max[2], -min[2]);

    proj.transpose();
    view.transpose();

    Matrix4f mvp=view*proj;

    drawer.renderShadow(mvp,elapsed);
    drawer.clearRenderList();

    return mvp;
}

bool Canvas::loadMapBox()
{
    setlocale(LC_NUMERIC, "C");

    const char * vertex_src =
            "uniform mat4 mvp;\n"
            "uniform vec4 pos;\n"
            "uniform vec4 size;\n"
            "varying vec4 coord;\n"
            "attribute vec3 coord3d;\n"
            "void main(void) {\n"
            "   coord=vec4(coord3d,1.0)*size+pos;\n"
            "   gl_Position = mvp * coord;\n"
            "}\n";


    const char * fragment_src =
            "varying vec4 coord;\n"
            "vec4 encode (vec3 n)\n"
            "{\n"
            "    float p = sqrt(n.z*8.0+8.0);\n"
            "    return vec4(n.xy/p + 0.5,0.0,1.0);\n"
            "}\n"
            "void main(void) {\n"
            "   gl_FragData[0] = vec4(1.0,1.0,1.0, 1.0);\n"
            "   gl_FragData[1] = encode(normalize(vec3(1.0,1.0,1.0)));\n"
            "   gl_FragData[2] = coord;\n"
            "}\n";

    if(!program.load(vertex_src,fragment_src))
        return false;

    if((uniform_mvp = program.getUniform("mvp"))==-1)
        return false;

    if((uniform_pos = program.getUniform("pos"))==-1)
        return false;

    if((uniform_size = program.getUniform("size"))==-1)
        return false;

    if((attribute_coord3d = program.getAttribute("coord3d"))==-1)
        return false;

    // Restore system locale
    setlocale(LC_ALL, "");

    //box shape
    GLfloat * cube_verts;
    Cube::verticesLines(cube_verts,cube_num);

    glGenBuffers(1, &vbo_cube);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*cube_num, cube_verts, GL_STATIC_DRAW);

    delete [] cube_verts;

    return true;
}

void Canvas::renderBox(const Matrix4f &mvp)
{
    program.bind();
    program.uniformMatrix(uniform_mvp,mvp);
    program.uniform(uniform_pos,project->map.width()/2.0f
                    ,project->map.height()/2.0f,project->map.depth()/2.0f,0.0f);
    program.uniform(uniform_size,project->map.width(),project->map.height(),project->map.depth(),1.0f);
    glEnableVertexAttribArray(attribute_coord3d);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube);
    glVertexAttribPointer(attribute_coord3d, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_LINES,0,cube_num);

    glDisableVertexAttribArray(attribute_coord3d);
    program.unbind();
}


#undef SPEED_TEST
