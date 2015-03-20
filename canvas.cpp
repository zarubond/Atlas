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

#include "canvas.h"

#include <QOpenGLContext>

Canvas::Canvas(QQuickItem *parent) :
    QQuickItem(parent)
{
    m_timerID=0;
    init=false;
    renderer=NULL;
    event_filter=NULL;

    setAcceptedMouseButtons(Qt::AllButtons);
    mouse_grab=false;
    connect(this,SIGNAL(widthChanged()),this,SLOT(resize()));
    connect(this,SIGNAL(heightChanged()),this,SLOT(resize()));
    connect(this,SIGNAL(xChanged()),this,SLOT(resize()));
    connect(this,SIGNAL(yChanged()),this,SLOT(resize()));
    connect(this,SIGNAL(visibleChanged()),this,SLOT(visibleChanged()));

    setActiveFocusOnTab(true);
    setAcceptHoverEvents(true);
    setFlag(QQuickItem::ItemHasContents);

    time.start();
    last_time=time.elapsed();
    m_fps=60;
    make_screen_shot=false;
    _resized=false;

    m_fbo=NULL;
    depth_rb=0;
    texture=NULL;
    renderer=NULL;
    render_vr=false;
    //setRotation(180);
}

Canvas::~Canvas()
{
    delete m_fbo;
    delete texture;
    delete renderer;
}

void Canvas::timerEvent(QTimerEvent* evt)
{
    if (evt && evt->timerId() == m_timerID)
        update();
}

QSGNode *Canvas::updatePaintNode(QSGNode* node, UpdatePaintNodeData*)
{
    if (width() <= 0 || height() <= 0 || !isLoaded() || !isVisible()) {
        delete node;
        return NULL;
    }
#ifdef QT_DEBUG
    glError();
#endif

    QSGSimpleTextureNode* textureNode = static_cast<QSGSimpleTextureNode*>(node);
    if (!textureNode)
        textureNode = new QSGSimpleTextureNode();

    if(!m_fbo || _resized)
    {
        delete m_fbo;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 3, 0))
        m_fbo = new QOpenGLFramebufferObject(width(), height(), QOpenGLFramebufferObject::Depth);//,QOpenGLFramebufferObject::Depth);
       // m_fbo->setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        //for some reason Qt has a problem with depth buffer, so this is a HACK to add it to framebuffer
        //crashed under OpenGL ES 3.0
#else
        m_fbo = new QOpenGLFramebufferObject(width(), height());//,QOpenGLFramebufferObject::Depth);
        if(depth_rb!=0)
            glDeleteRenderbuffers(1, &depth_rb);

        m_fbo->bind();
        glGenRenderbuffers(1, &depth_rb);
        glBindRenderbuffer(GL_RENDERBUFFER, depth_rb);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width(), height());
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_rb);
        m_fbo->release();
#endif
    }
    _resized=false;

    delete texture;
    texture=this->window()->createTextureFromId(m_fbo->texture(), m_fbo->size());
    textureNode->setTexture(texture);
    textureNode->setRect(this->boundingRect());

    if (m_fbo)
    {
        if(renderer!=NULL)
        {

            mutex.lock();
            if(init)
            {/*
              //  glEnable(GL_DEPTH_TEST);
                glDepthMask(GL_TRUE);
                glDepthFunc(GL_LEQUAL);
                glEnable(GL_BLEND);
                glEnable(GL_DEPTH_CLAMP);*/

                glDepthMask(GL_TRUE);
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_LEQUAL);

                if(render_vr)
                {
                    textureNode->setTextureCoordinatesTransform(QSGSimpleTextureNode::MirrorVertically);

                    renderer->renderBuffersVR(world, camera);
                    m_fbo->bind();
                    renderer->renderSceneVR(world, camera);
                    m_fbo->release();
                }
                else
                {
                    textureNode->setTextureCoordinatesTransform(QSGSimpleTextureNode::NoTransform);
                    renderer->renderBuffers(world, camera);
                    m_fbo->bind();
                    renderer->renderScene(world, camera);
                    m_fbo->release();
                }


                if(make_screen_shot)
                    createScreenShot();
            }
            else
            {
                connect(window()->openglContext(), SIGNAL(aboutToBeDestroyed()),
                                this, SLOT(cleanup()), Qt::DirectConnection);
                init=renderer->init();

                resize();
                init=true;
            }
            mutex.unlock();
        }
        else {
            if(glVersion()==OpenGL::OPENGL_ES_2_0)
                renderer=new SceneRendererForward;
            else
                renderer=new SceneRendererDeferred;
        }
    }
    updateFps();
    return textureNode;
}

void Canvas::renderVr(bool enable)
{
    render_vr=enable;
}

void Canvas::makeSnapshot()
{
    make_screen_shot=true;
}

void Canvas::cleanup()
{
    killTimer(m_timerID);
}


void Canvas::updateFps()
{
    int t=time.elapsed();
/*
        int f=(1000.0f/float(t-last_time)+m_fps*4.0)/5.0;
       // if(m_fps>60) m_fps=60;
        last_time=t;
    //    if(f!=m_fps)
        {
            m_fps=f;
            emit fpsChanged(int(m_fps));
        }
        cout<<"flip"<<endl;
*/

    if(t-last_time>1000)
    {
        last_time=t;
        m_fps=fps_cnt;
        emit fpsChanged(int(fps_cnt));
        fps_cnt=0;
    }
    else
        fps_cnt++;
}

void Canvas::visibleChanged()
{
    if(isVisible())
    {
        if(window()!=NULL)
        {
            m_timerID = startTimer(17); //17 to be lower than 60, flickering problem
        }
    }
    else
    {
        killTimer(m_timerID);
        m_timerID=0;
    }
}

void Canvas::createScreenShot()
{
    make_screen_shot=false;
    int w=width();
    int h=height();
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    QPointF point=mapFromScene(QPointF(0,0));
    //OpenGL has inverse vertical position
    int y=window()->height()-height()+point.y();
    Image * img=new Image(w,h,QImage::Format_RGBA8888);
    glFinish();
    //-point.x(),y
    glReadPixels(-point.x(), y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, img->bits());
    glFinish();
    img->mirror(false, true);
    emit screenShotDone(img);
}

const Camera *Canvas::getCamera() const
{
    return &camera;
}

void Canvas::setCamera(const Camera &value)
{
    mutex.lock();
    camera = value;
    mutex.unlock();
}

void Canvas::setWorld(WorldGraphics *world)
{
    mutex.lock();
    this->world=world;
    mutex.unlock();
}

void Canvas::captureMouse()
{
     grabMouse();
     QGuiApplication::setOverrideCursor( QCursor( Qt::BlankCursor ));
     mouse_grab=true;
}

void Canvas::releaseMouse()
{
     QGuiApplication::setOverrideCursor( QCursor( Qt::ArrowCursor ));
     mouse_grab=false;
}

bool Canvas::isCaptured()
{
    return mouse_grab;
}

//HACK
void Canvas::wheelEvent(QWheelEvent *event)
{
    if(event_filter!=NULL) event_filter->eventFilter(this,event);
}



void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    if(event_filter!=NULL) event_filter->eventFilter(this,event);
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    setFocus(true);
    if(event_filter!=NULL)
    {
        mutex.lock();
        event_filter->eventFilter(this,event);
        mutex.unlock();
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    if(event_filter!=NULL) event_filter->eventFilter(this,event);
}

void Canvas::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event_filter!=NULL) event_filter->eventFilter(this,event);
}

void Canvas::hoverMoveEvent(QHoverEvent * event)
{
    if(event_filter!=NULL) event_filter->eventFilter(this,event);
}

void Canvas::keyPressEvent(QKeyEvent *event)
{
    if(event_filter!=NULL) event_filter->eventFilter(this,event);
}

void Canvas::keyReleaseEvent(QKeyEvent *event)
{
    if(event_filter!=NULL) event_filter->eventFilter(this,event);
}

/*
bool Canvas::event(QEvent *event)
{
    std::cout<<"evet "<<event->type()<<std::endl;
    if(event_filter==NULL)
        return false;
    return event_filter->eventFilter(this,event);
}*/


void Canvas::installEventFilter(QObject *object)
{
    event_filter=object;
}

int Canvas::fps() const
{
    return m_fps;
}

void Canvas::resize()
{
    int w=width();
    int h=height();
    if(renderer!=NULL)
        renderer->resize(w ,h);
    emit resized(w, h);
    _resized=true;
}
