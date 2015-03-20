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

#include "atlas.h"

Atlas::Atlas(QObject *parent) :
    QObject(parent),component(NULL)
{
    component = new QQmlComponent(&engine);
    qmlRegisterType<Canvas> ("Atlas", 1, 0, "Canvas");
    qmlRegisterType<Window>  ("Atlas", 1, 0, "Atlas");
    qmlRegisterType<Dialog> ("Atlas", 1, 0, "Dialog");
    qmlRegisterType<Project>("Atlas", 1, 0, "Project");
    //dialogs
    qmlRegisterType<NewProjectDialog>   ("Atlas",1,0,"NewProjectDialog");
    qmlRegisterType<EnvironmentDialog>  ("Atlas",1,0,"EnvironmentDialog");
    qmlRegisterType<OptionsDialog>      ("Atlas",1,0,"OptionsDialog");
    qmlRegisterType<ProjectDialog>      ("Atlas",1,0,"ProjectDialog");
    //tools
    qmlRegisterType<TerrainMaterialDialog>  ("Atlas",1,0,"TerrainMaterialDialog");
    qmlRegisterType<TerrainProceduralDialog>("Atlas",1,0,"TerrainProceduralDialog");
    qmlRegisterType<TerrainImportDialog>    ("Atlas",1,0,"TerrainImportDialog");
    qmlRegisterType<TerrainControl>           ("Atlas",1,0,"TerrainControl");

    qmlRegisterType<VegetationTool>         ("Atlas",1,0,"VegetationTool");

    qmlRegisterType<ModelControl>           ("Atlas",1,0,"ModelControl");
    qmlRegisterType<ModelDataModel>         ("Atlas",1,0,"ModelDataModel");
    qmlRegisterType<MeshDataModel>          ("Atlas",1,0,"MeshDataModel");
    qmlRegisterType<ModelEditDialog>        ("Atlas",1,0,"ModelEditDialog");

    //assets
    qmlRegisterType<AssetTextureDialog> ("Atlas", 1, 0, "AssetTextureDialog");
    qmlRegisterType<AssetMeshDialog>    ("Atlas", 1, 0, "AssetMeshDialog");
    //other
    qmlRegisterType<QStandardItemModel>();
    qmlRegisterType<ImageProvider>  ("Atlas", 1, 0, "ImageProvider");
    qmlRegisterType<Workspace>      ("Atlas", 1, 0, "Workspace");
    qmlRegisterType<Welcome>        ("Atlas", 1, 0, "Welcome");
    qmlRegisterType<ProjectLoader>  ("Atlas", 1, 0, "ProjectLoader");


    QObject::connect(&engine, SIGNAL(quit()), QCoreApplication::instance(), SLOT(quit()));
    logger = new QOpenGLDebugLogger(this);
    connect(logger, &QOpenGLDebugLogger::messageLogged, this, &Atlas::handleLoggedMessage);
    atlas=NULL;
    topLevel=NULL;
    window=NULL;
    settings.load();
}

Atlas::~Atlas()
{
    settings.save();
    delete component;
    delete topLevel;

    main_context->doneCurrent();
    delete main_context;
    main_context=NULL;
    fake_surface->destroy();
    delete fake_surface;
    fake_surface=NULL;
}

bool Atlas::start()
{
    component->loadUrl(QUrl(QStringLiteral("qrc:/qml/atlas.qml")));

    if (!component->isReady() ) {
        qWarning("%s", qPrintable(component->errorString()));
        return false;
    }

    topLevel = component->create();
    window = qobject_cast<QQuickWindow*>(topLevel);
    connect(window,SIGNAL(sceneGraphInitialized()),this,SLOT(initGL()),Qt::DirectConnection);
    connect(window,SIGNAL(closing(QQuickCloseEvent*)),this,SLOT(close(QQuickCloseEvent*)));

    QSurfaceFormat fmt = window->requestedFormat();

    fmt.setDepthBufferSize(24);
#if defined(OS_LINUX) || defined(OS_WINDOWS)
    if(GlobalSettings::instance()->renderBackend()==GlobalSettings::OPENGL30)
    {
#ifdef OPENGL_DEBUG
        fmt.setOption(QSurfaceFormat::DebugContext);
#endif
        fmt.setRenderableType(QSurfaceFormat::OpenGL);
       // fmt.setVersion(3,0); This makes crash with glyphs under qt 5.3.1 and shader compilation error under 5.2
        fmt.setProfile(QSurfaceFormat::CoreProfile);
    }
    else if(GlobalSettings::instance()->renderBackend()==GlobalSettings::OPENGLES20 || GlobalSettings::instance()->renderBackend()==GlobalSettings::OPENGLES30)
    {
        fmt.setRenderableType(QSurfaceFormat::OpenGLES);
    }
#else
    fmt.setRenderableType(QSurfaceFormat::OpenGLES);
#endif

    window->setFormat(fmt);

    main_context=new QOpenGLContext;
    fake_surface=new QOffscreenSurface;

    window->show();

    return true;
}

bool Atlas::expired()
{
    QDate date=QLocale(QLocale::C).toDate(QString(__DATE__).simplified(), QLatin1String("MMM d yyyy"));;
    QDate today=QDate::currentDate ();

    if(date.daysTo(today) > 3)
    {
        //QMessageBox messageBox;
        //messageBox.critical(0,"Error","Your trial period has expired! Please download newer version.");
        return false;
    }

    return true;
}

void Atlas::initGL()
{
    OpenGL::OpenGLVersion ver=OpenGL::OPENGL_ES_2_0;
    switch(settings.renderBackend())
    {
    case GlobalSettings::OPENGL30:
        ver=OpenGL::OPENGL_3_0;
        break;
    case GlobalSettings::OPENGLES20:
        ver=OpenGL::OPENGL_ES_2_0;
        break;
    case GlobalSettings::OPENGLES30:
        ver=OpenGL::OPENGL_ES_3_0;
        break;
    }

    OpenGL::glInit(window->openglContext(), ver);

#ifdef OPENGL_DEBUG
    if(OpenGL::glVersion()==OpenGL::OPENGL_3_0)
    {
        logger->initialize();
        logger->startLogging();
    }
#endif

    QTimer::singleShot(100,this,SLOT(init()));
    main_context->setShareContext(window->openglContext());
    startTimer(500);
}

void Atlas::close(QQuickCloseEvent *)
{

}

void Atlas::init()
{
    main_context->create();

    fake_surface->create();
    main_context->makeCurrent(fake_surface);
    if(!settings.assetsPath().isEmpty())
    {
        QString file;
        if(settings.assetsPath().isLocalFile())
            file=settings.assetsPath().toLocalFile();
        else
            file=settings.assetsPath().toString();

        if(file.isEmpty() || !assets.load(file))
        {
            cerr<<"Unable to load assets"<<endl;
        }
    }
    //engine.rootContext()->setContextProperty("settings",&settings);
    atlas=qvariant_cast<Window*>(window->property("atlas"));
    atlas->init(window, main_context, &assets);
}

void Atlas::timerEvent(QTimerEvent *)
{///some kind of bug in QT??? HACK
    main_context->makeCurrent(fake_surface);
}

void Atlas::handleLoggedMessage(const QOpenGLDebugMessage &debugMessage)
{
    cerr<<debugMessage.source()<<" "<<debugMessage.message()<<endl;
}
