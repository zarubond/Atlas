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
#include "atlas.h"
#include "ui_atlas.h"

Atlas::Atlas(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Atlas)
{
    space=new Canvas(this);
    welcome=new Welcome(this);

    ui->setupUi(this);
    ui->statusBar->hide();

    ui->screen->addWidget(space);
    ui->screen->addWidget(welcome);
    ui->screen->setCurrentWidget(welcome);

    on_toolTab_currentChanged(ui->toolTab->currentIndex());

    connect(ui->actionSave_All,SIGNAL(triggered()),this,SLOT(action_SaveAll()));
    ui->actionSave_All->setShortcut(QKeySequence::Save);
    connect(ui->actionHelp,SIGNAL(triggered()),this,SLOT(action_help()));
    ui->actionHelp->setShortcut(QKeySequence::HelpContents);
    connect(ui->actionOpen_Project,SIGNAL(triggered(bool)),this,SLOT(openProjectDialog(bool)));
    ui->actionOpen_Project->setShortcut(QKeySequence::Open);
    connect(ui->actionNew_Project,SIGNAL(triggered(bool)),this,SLOT(newProjectDialog(bool)));
    ui->actionNew_Project->setShortcut(QKeySequence::New);
    connect(ui->actionExport,SIGNAL(triggered()),this,SLOT(action_map_export()));

    connect(ui->actionUndo,SIGNAL(triggered()),this,SLOT(action_Undo()));
    ui->actionUndo->setShortcut(QKeySequence::Undo);
    connect(ui->actionRedo,SIGNAL(triggered()),this,SLOT(action_Redo()));
    ui->actionRedo->setShortcut(QKeySequence::Redo);

    connect(ui->actionClose_Project,SIGNAL(triggered()),this,SLOT(action_CloseProject()));
    ui->actionClose_Project->setShortcut(QKeySequence::Close);
    connect(ui->actionWireframe,SIGNAL(toggled(bool)),this,SLOT(action_Wireframe(bool)));
    ui->actionWireframe->setShortcut(Qt::Key_F2);
    connect(ui->actionLight,SIGNAL(toggled(bool)),this,SLOT(action_Light(bool)));
    ui->actionLight->setShortcut(Qt::Key_F3);
    connect(ui->actionShadow,SIGNAL(toggled(bool)),this,SLOT(action_Shadow(bool)));
    ui->actionShadow->setShortcut(Qt::Key_F4);
    connect(ui->actionFullscreen,SIGNAL(toggled(bool)),this,SLOT(action_Fullscreen(bool)));
    ui->actionFullscreen->setShortcut(Qt::Key_F11);
    connect(ui->actionQuit,SIGNAL(triggered()),this,SLOT(action_Quit()));
    ui->actionQuit->setShortcut(QKeySequence::Quit);

    connect(ui->actionConfigure,SIGNAL(triggered()),this,SLOT(action_Configure()));

    connect(ui->actionSun,SIGNAL(triggered()),this,SLOT(action_Sun()));

    connect(ui->actionAssets,SIGNAL(triggered()),this,SLOT(action_Assets()));

    time.start();
    running=false;

    QTimer::singleShot(100,this,SLOT(init()));
}

Atlas::~Atlas()
{
    delete ui;
}

bool Atlas::init()
{
    space->init();
    editor.init(space);
    session.setup(space,&editor);

    if(session.isLoaded())
    {
        running=true;
    }
    else
    {
        ui->actionClose_Project->setDisabled(true);
        showWelcome();
        stop();
    }

    reloadRecentProject();
    for(int i=0;i<editor.toolCount();i++)
    {
        QMenu * menu=editor.getTool(i)->createMenu();
        if(menu!=NULL)
            ui->menuTools->addMenu(menu);
        ui->toolTab->insertTab(i,editor.getTool(i),editor.getTool(i)->getName());
    }

#ifdef Q_WS_ANDROID
   /* session.openProject("/storage/sdcard1/android/android.apro");

    showSpace();
    run();*/

#endif

    return true;
}

void Atlas::newProjectDialog(bool)
{
    DialogNewProject new_project(this,&session);
    new_project.exec();
    if(session.isLoaded())
    {
        ui->actionClose_Project->setText(tr("Close Project ")+"\""+session.project->name+"\"");
        ui->actionClose_Project->setEnabled(true);
        reloadRecentProject();

        showSpace();
        run();
    }
}

void Atlas::openProjectDialog(bool)
{
    QString file_name = QFileDialog::getOpenFileName(this, tr("Open Project"),
                                                     "", tr("Files (*.apro)"));

    openProject(file_name);
}

void Atlas::updatePosition()
{
    const Vertex3f & pos=space->getCamera()->getPosition();
    ui->position_x->setValue(pos[0]);
    ui->position_y->setValue(pos[1]);
    ui->position_z->setValue(pos[2]);
}

void Atlas::run()
{
    running=true;
    while(running)
    {
        editor.update(time.elapsed());
        space->updateGL();
        ui->fps_counter->display(space->fps());
        updatePosition();

        QCoreApplication::processEvents(QEventLoop::ProcessEventsFlags());
    }
}

void Atlas::stop()
{
    running=false;
}

void Atlas::showWelcome()
{
    ui->toolTab->setEnabled(false);
    ui->screen->setCurrentWidget(welcome);
    welcome->load(this);

    ui->fps_counter->display(0);
}

void Atlas::showSpace()
{
    ui->toolTab->setEnabled(true);
    ui->screen->setCurrentWidget(space);
}

void Atlas::reloadRecentProject()
{
    QStringList files = settings.getStringList("recentFileList");
    settings.setValue("recentFileList", files);
    QMenu * menu=new QMenu(this);
    for(int i=0;i<files.size();i++)
    {
        QString text = tr("&%1. %2").arg(i + 1).arg(files[i]);
        QAction * action=new QAction(text,this);
        action->setVisible(true);
        connect(action, SIGNAL(triggered()),
                this, SLOT(action_OpenRecentProject()));
        action->setData(files[i]);
        menu->addAction(action);
    }
    ui->actionRecent_Projects->setMenu(menu);
}

void Atlas::keyPressEvent(QKeyEvent *key)
{
    switch(key->key())
    {
    default:
        editor.keyPressEvent(key);
        break;
    }
}

void Atlas::keyReleaseEvent(QKeyEvent *key)
{
    editor.keyReleaseEvent(key);
}

void Atlas::closeEvent(QCloseEvent *)
{
    running=false;
}

bool Atlas::openProject(const QString &file_name)
{
    if(session.openProject(file_name))
    {
        ui->actionClose_Project->setText(tr("Close Project ")+"\""+session.project->name+"\"");
        ui->actionClose_Project->setEnabled(true);
        ui->actionSun->setEnabled(true);
        reloadRecentProject();

        showSpace();
        run();
        return true;
    }
    return false;
}

void Atlas::action_SaveAll()
{
    std::cout<<"map save"<<std::endl;
    session.saveProject();
    std::cout<<"map saved"<<std::endl;
}

void Atlas::action_map_export()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Export Directory"),
                                                "~",
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);
    //session.project->exportMap(dir);
    if(session.project!=NULL)
    {
        ProjectHandler exp;
        //exp.exportProject(dir);
    }
}

void Atlas::action_Sun()
{
    DialogSun * dialog =new DialogSun(&session.project->light,this);
    dialog->show();
}

void Atlas::action_help()
{
    QMessageBox::about(this, tr("Atlas help"),
            tr("<center>How to</center>"
               "Moving WSAD<br>"
               "Navigator distance wheel<br>"
               "Move up-f down-c<br>"
               "Hand size ctrl+wheel<br>"
               "Speed alt+wheel<br>"
               "Undo ctrl+z<br>"
               "If you want to know more please read help!"));
}

void Atlas::action_Undo()
{
    editor.undo();
}

void Atlas::action_Redo()
{
    editor.redo();
}

void Atlas::action_Quit()
{
    if(session.isLoaded())
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Close without saving."));
        msgBox.setInformativeText("Would you like to save project?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();
        if(ret==QMessageBox::Yes)
        {
            action_SaveAll();
        }
    }

    exit(0);
}

void Atlas::action_CloseProject()
{
    if(session.isLoaded())
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Close without saving."));
        msgBox.setInformativeText("Would you like to save project?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();
        if(ret==QMessageBox::Yes)
        {
            action_SaveAll();
        }
    }

    if(session.closeProject())
    {
        ui->actionClose_Project->setDisabled(true);
        ui->actionSun->setDisabled(true);
        showWelcome();
        stop();
    }
}

void Atlas::action_OpenRecentProject()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        openProject(action->data().toString());
    }
}

void Atlas::action_Wireframe(bool checked)
{
    space->showWireframe(checked);
}

void Atlas::action_Light(bool checked)
{
    space->showLights(checked);
}

void Atlas::action_Shadow(bool checked)
{
    space->showShadows(checked);
}

void Atlas::action_Fullscreen(bool checked)
{
    if(checked)
        showFullScreen();
    else
        showNormal();
}

void Atlas::action_Configure()
{
    ConfigDialog * dialog =new ConfigDialog(this);
    dialog->show();
}

void Atlas::action_Assets()
{/*
    TextureDialog dialog(&editor.assets);
    dialog.exec();*/
}

void Atlas::on_toolTab_currentChanged(int index)
{
    if(index>=0)
        editor.selectTool(index);
}

void Atlas::on_view_free_clicked()
{
    editor.driver.setView(Driver::FREE);
}

void Atlas::on_view_editor_clicked()
{
    editor.driver.setView(Driver::EDITOR);
}

void Atlas::on_view_player_clicked()
{
    editor.driver.setView(Driver::PLAYER);
}
