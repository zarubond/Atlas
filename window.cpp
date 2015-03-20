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

#include "window.h"

Window::Window(QObject *parent): QObject(parent)
{
    m_projectOpen=false;
    m_project=NULL;
    m_welcome=NULL;
    m_workspace=NULL;
    m_projectLoader=NULL;
    window=NULL;
#ifdef OS_ANDROID
    m_gamepad=true;
#else
    m_gamepad=false;
#endif
    m_wireframe=false;
    m_lightVisible=true;
    m_shadow=true;
    m_fullscreen=false;
    m_renderVr=false;

#if defined(Q_OS_ANDROID)
    m_homeFolder=QStandardPaths::writableLocation(QStandardPaths::DataLocation);
#elif defined(Q_OS_LINUX)
    m_homeFolder=QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
#elif defined(Q_OS_WIN)
    m_homeFolder=QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
#endif
    m_terrainVisible=true;
    m_modelVisible=true;
    m_oceanVisible=true;
}

Window::~Window()
{
    m_workspace->unloadProject();
}

bool Window::init(QQuickWindow *window, QOpenGLContext *main_context, Assets * assets)
{
    this->window=window;
    this->assets=assets;
    this->main_context=main_context;
    connect(window,SIGNAL(closing(QQuickCloseEvent*)),this,SLOT(close(QQuickCloseEvent*)));

    showWelcome();
    return true;
}

void Window::projectLoaded(Project *project)
{
    if(project!=NULL)
    {
        project->assets=assets;
        window->alert(0);
        m_workspace->setProject(project);
        setProjectOpen(true);
        showWorkspace();
    }
    else
    {
        setProjectOpen(false);
        showWelcome();
    }
    setProject(project);
}

void Window::createProject(const QString &path, const QString &name, const QString &author, int width, int height, int depth)
{
    closeProject();
    showLoading();
    m_projectLoader->createProject(path,name,author,width,height,depth);
}

void Window::undo()
{
    m_workspace->undo();
}

void Window::redo()
{
    m_workspace->redo();
}

void Window::close(QQuickCloseEvent*)
{
    m_workspace->unloadProject();
    delete m_project;
    setProject(NULL);
}

void Window::openProject(const QUrl &file_path)
{
    openProject(file_path.toLocalFile());
}

void Window::openProject(const QString &file_name)
{
    QFileInfo info(file_name);
    if(info.exists())
    {
        showLoading();
        m_projectLoader->openProject(file_name,main_context);
    }
    else
    {
        cerr<<"Unable to open project "<<file_name<<endl;
    }
}

bool Window::isProjectOpen() const
{
    return m_projectOpen;
}

bool Window::isGamepad() const
{
    return m_gamepad;
}

bool Window::isWireframe() const
{
    return m_wireframe;
}

bool Window::isFullscreen() const
{
    return m_fullscreen;
}

bool Window::isLightVisible() const
{
    return m_lightVisible;
}

bool Window::isShadow() const
{
    return m_shadow;
}

QUrl Window::homeFolder() const
{
    return m_homeFolder;
}

Project *Window::project() const
{
    return m_project;
}

Workspace *Window::workspace() const
{
    return m_workspace;
}

Welcome *Window::welcome() const
{
    return m_welcome;
}

ProjectLoader *Window::projectLoader() const
{
    return m_projectLoader;
}

bool Window::renderVr() const
{
    return m_renderVr;
}

bool Window::terrainVisible() const
{
    return m_terrainVisible;
}

bool Window::modelVisible() const
{
    return m_modelVisible;
}

bool Window::oceanVisible() const
{
    return m_oceanVisible;
}

void Window::saveAll()
{
    cout<<"saving project..."<<endl;
    m_projectLoader->saveProject();
    cout<<"project saved"<<endl;
}

void Window::saveAs(const QUrl &save_url)
{
    m_projectLoader->saveProjectAs(save_url);
}

void Window::action_map_export()
{
    /*QString dir = QFileDialog::getExistingDirectory(this, tr("Export Directory"),
                                                "~",
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);
    //session.project->exportMap(dir);
    if(session.project!=NULL)
    {
        WorldIO exp;
        //exp.exportProject(dir);
    }*/
}


void Window::closeProject()
{
    m_workspace->unloadProject();
    delete m_project;
    setProject(NULL);
    showWelcome();
    setProjectOpen(false);
}

void Window::setProjectOpen(bool arg)
{
    if (m_projectOpen != arg) {
        m_projectOpen = arg;
        emit projectOpenChanged(arg);
    }
}

void Window::setGamepad(bool arg)
{
    if (m_gamepad != arg) {
        m_gamepad = arg;
        emit gamepadChanged(arg);
    }
}

void Window::setWireframe(bool arg)
{
    if (m_wireframe != arg) {
        m_workspace->showWireframe(m_wireframe);
        m_wireframe = arg;
        emit wireframeChanged(arg);
    }
}



void Window::setFullscreen(bool arg)
{
    if (m_fullscreen != arg && window!=NULL) {
        m_fullscreen = arg;
        if(m_fullscreen)
            window->showFullScreen();
        else
        {
#ifdef OS_ANDROID
            window->setWindowState(Qt::WindowMaximized);
#else
            window->showNormal();
#endif
        }

        emit fullscreenChanged(arg);
    }
}

void Window::setLightVisible(bool arg)
{
    if (m_lightVisible != arg) {
        m_lightVisible = arg;
        m_workspace->showLight(m_lightVisible);
        emit lightVisibleChanged(arg);
    }
}

void Window::setShadow(bool arg)
{
    if (m_shadow != arg) {
        m_shadow = arg;
        m_workspace->showShadow(m_shadow);
        emit shadowChanged(arg);
    }
}

void Window::setWorkspace(Workspace *arg)
{
    if (m_workspace != arg) {
        m_workspace = arg;
        emit workspaceChanged(arg);
    }
}

void Window::setWelcome(Welcome *arg)
{
    if (m_welcome != arg) {
        m_welcome = arg;
        if(m_welcome!=NULL) m_welcome->load(this);
        emit welcomeChanged(arg);
    }
}

void Window::setProjectLoader(ProjectLoader *arg)
{
    if (m_projectLoader != arg) {
        m_projectLoader = arg;
        if(m_projectLoader!=NULL)  connect(m_projectLoader,SIGNAL(projectLoaded(Project*)),this,SLOT(projectLoaded(Project*)));
        emit projectLoaderChanged(arg);
    }
}

void Window::setRenderVr(bool arg)
{
    if (m_renderVr != arg) {
        m_renderVr = arg;
        m_workspace->renderVr(m_renderVr);
        emit renderVrChanged(arg);
    }
}

void Window::setTerrainVisible(bool arg)
{
    if (m_terrainVisible == arg)
        return;
    if(m_project!=NULL)
        m_project->world.terrain.setVisible(arg);

    m_terrainVisible = arg;
    emit terrainVisibleChanged(arg);
}

void Window::setModelVisible(bool arg)
{
    if (m_modelVisible == arg)
        return;
    if(m_project!=NULL)
        m_project->world.model.setVisible(arg);

    m_modelVisible = arg;
    emit modelVisibleChanged(arg);
}

void Window::setOceanVisible(bool arg)
{
    if (m_oceanVisible == arg)
        return;

    if(m_project!=NULL)
        m_project->world.ocean.setVisible(arg);

    m_oceanVisible = arg;
    emit oceanVisibleChanged(arg);
}

void Window::setProject(Project *arg)
{
    if (m_project != arg) {
        m_project = arg;
        emit projectChanged(arg);
    }
}

void Window::showWorkspace()
{
    m_welcome->setVisible(false);
    m_workspace->setVisible(true);
    m_projectLoader->setVisible(false);
}

void Window::showWelcome()
{
    m_welcome->setVisible(true);
    m_workspace->setVisible(false);
    m_projectLoader->setVisible(false);
}

void Window::showLoading()
{
    m_welcome->setVisible(false);
    m_workspace->setVisible(false);
    m_projectLoader->setVisible(true);
}
