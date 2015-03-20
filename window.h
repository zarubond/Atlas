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
#ifndef WINDOW_H
#define WINDOW_H

#include <QQuickView>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <QStandardItemModel>
#include <QOpenGLDebugLogger>

#include "canvas.h"
#include "welcome.h"
#include "dialog/environmentdialog.h"
#include "dialog/optionsdialog.h"
#include "dialog/newprojectdialog.h"
#include "dialog/help.h"
#include "dialog/about.h"
#include "dialog/projectdialog.h"

#include "project.h"
#include "globalsettings.h"

#include "lib/tools/shell.h"
#include "assets/assettexturedialog.h"
#include "workspace.h"
#include "projectloader.h"

//http://www.ics.com/blog/qt-and-opengl-part-1-loading-3d-model-open-asset-import-library-assimp#.U6F03f7ZJUQ
//http://pandoramapper.googlecode.com/svn-history/r212/experimental/threadrenderer.cpp
//http://xanm.blogspot.cz/2014/03/admob-in-qt.html
class Window: public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool projectOpen READ isProjectOpen WRITE setProjectOpen NOTIFY projectOpenChanged)
    Q_PROPERTY(bool gamepad READ isGamepad WRITE setGamepad NOTIFY gamepadChanged)
    Q_PROPERTY(bool wireframe READ isWireframe WRITE setWireframe NOTIFY wireframeChanged)
    Q_PROPERTY(bool fullscreen READ isFullscreen WRITE setFullscreen NOTIFY fullscreenChanged)
    Q_PROPERTY(bool lightVisible READ isLightVisible WRITE setLightVisible NOTIFY lightVisibleChanged)
    Q_PROPERTY(bool shadow READ isShadow WRITE setShadow NOTIFY shadowChanged)
    Q_PROPERTY(bool renderVr READ renderVr WRITE setRenderVr NOTIFY renderVrChanged)
    Q_PROPERTY(QUrl homeFolder READ homeFolder CONSTANT)
    Q_PROPERTY(Project* project READ project NOTIFY projectChanged)
    Q_PROPERTY(Workspace* workspace READ workspace WRITE setWorkspace NOTIFY workspaceChanged)
    Q_PROPERTY(Welcome* welcome READ welcome WRITE setWelcome NOTIFY welcomeChanged)
    Q_PROPERTY(ProjectLoader* projectLoader READ projectLoader WRITE setProjectLoader NOTIFY projectLoaderChanged)

    Q_PROPERTY(bool terrainVisible READ terrainVisible WRITE setTerrainVisible NOTIFY terrainVisibleChanged)
    Q_PROPERTY(bool modelVisible READ modelVisible WRITE setModelVisible NOTIFY modelVisibleChanged)
    Q_PROPERTY(bool oceanVisible READ oceanVisible WRITE setOceanVisible NOTIFY oceanVisibleChanged)

public:
    Window(QObject *parent = 0);
    ~Window();
    bool init(QQuickWindow *window, QOpenGLContext * main_context, Assets *assets);
    /**
     * @brief isProjectOpen
     * @return If a project is loaded in to program.x
     */
    bool isProjectOpen() const;
    bool isGamepad() const;
    bool isWireframe() const;
    bool isFullscreen() const;
    bool isLightVisible() const;
    bool isShadow() const;
    QUrl homeFolder() const;
    Project* project() const;
    Workspace* workspace() const;
    Welcome* welcome() const;
    ProjectLoader* projectLoader() const;
    bool renderVr() const;

    bool terrainVisible() const;
    bool modelVisible() const;
    bool oceanVisible() const;
public slots:
    /**
     * @brief openProject OpenProject with given name.
     * @param file_name
     * @return
     */
    void openProject(const QString & file_name);
    void openProject(const QUrl& file_path);
    void closeProject();
    void createProject(const QString &path, const QString &name, const QString &author, int width, int height, int depth);
    void undo();
    void redo();

    void saveAll();
    void saveAs(const QUrl & save_url);
    void action_map_export();

    void setProjectOpen(bool arg);
    void setGamepad(bool arg);
    void setWireframe(bool arg);
    void setFullscreen(bool arg);
    void setLightVisible(bool arg);
    void setShadow(bool arg);

    void setWorkspace(Workspace* arg);
    void setWelcome(Welcome* arg);
    void setProjectLoader(ProjectLoader* arg);

    void setRenderVr(bool arg);

    void setTerrainVisible(bool arg);
    void setModelVisible(bool arg);
    void setOceanVisible(bool arg);
signals:
    void projectOpenChanged(bool arg);
    void gamepadChanged(bool arg);
    void wireframeChanged(bool arg);
    void fullscreenChanged(bool arg);
    void lightVisibleChanged(bool arg);

    void shadowChanged(bool arg);
    void projectChanged(Project* arg);
    void workspaceChanged(Workspace* arg);
    void welcomeChanged(Welcome* arg);
    void projectLoaderChanged(ProjectLoader* arg);
    void renderVrChanged(bool arg);
    void terrainVisibleChanged(bool arg);
    void modelVisibleChanged(bool arg);
    void oceanVisibleChanged(bool arg);

private slots:
    void projectLoaded(Project* project);
    void close(QQuickCloseEvent *);
private:
    void setProject(Project* arg);
    void showWorkspace();
    void showWelcome();
    void showLoading();

    Assets * assets;
    QQuickWindow *window;
    QOpenGLContext *main_context;

    bool m_projectOpen;
    bool m_gamepad;
    bool m_wireframe;
    bool m_fullscreen;
    bool m_lightVisible;
    bool m_shadow;
    QUrl m_homeFolder;
    Project* m_project;
    Workspace* m_workspace;
    Welcome* m_welcome;
    ProjectLoader* m_projectLoader;
    bool m_renderVr;
    bool m_terrainVisible;
    bool m_modelVisible;
    bool m_oceanVisible;
};

#endif // WINDOW_H
