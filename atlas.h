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
#ifndef ATLAS_H
#define ATLAS_H

#include <QMainWindow>

#include <QStandardItemModel>
#include <QMessageBox>
#include <QInputDialog>
#include <QColorDialog>
#include <QShortcut>
#include <QGraphicsView>

#include "canvas.h"
#include "welcome.h"
#include "editor/editor.h"
#include "dialog/dialognewproject.h"
#include "dialog/dialogsun.h"
#include "dialog/configdialog.h"

#include "project/project.h"
#include "project/projecthandler.h"
#include "session.h"
#include "settings.h"

#include "tools/shell.h"
#include "assets/texturedialog.h"

namespace Ui {
    class Atlas;
}

class Atlas : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Atlas(QWidget *parent = 0);
    ~Atlas();

    /**
     * @brief openProject OpenProject with given name.
     * @param file_name
     * @return
     */
    bool openProject(const QString & file_name);
public slots:
    /**
     * @brief newProjectDialog Open new project dialog
     */
    void newProjectDialog(bool);
    /**
     * @brief openProjectDialog Open dialog for opening project
     */
    void openProjectDialog(bool);

private:
    void closeEvent(QCloseEvent *);
    void reloadRecentProject();

    void updatePosition();

    //master object
    Ui::Atlas *ui;
    Canvas * space;
    Welcome * welcome;
    Session session;
    Editor editor;
    Settings settings;
    bool running;
    Shell shell;

    QElapsedTimer time;

protected:
    void keyPressEvent(QKeyEvent * key);
    void keyReleaseEvent(QKeyEvent * key);
private slots:
    bool init();

    void run();
    void stop();

    void showWelcome();
    void showSpace();

    void action_SaveAll();
    void action_map_export();

    void action_Sun();
    void action_help();
    void action_Undo();
    void action_Redo();
    void action_Quit();
    void action_CloseProject();
    void action_OpenRecentProject();
    void action_Wireframe(bool checked);
    void action_Light(bool checked);
    void action_Shadow(bool checked);
    void action_Fullscreen(bool checked);
    void action_Configure();
    void action_Assets();

    void on_toolTab_currentChanged(int index);
    void on_view_free_clicked();
    void on_view_editor_clicked();
    void on_view_player_clicked();
};

#endif // ATLAS_H
