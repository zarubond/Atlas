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
import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Window 2.0
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1
import Atlas 1.0 as Atlas
import "./dialog"

ApplicationWindow {
    id: window;
    width: 1024
    height: 768

    FileDialog {
        id: dialog_open_project;
        title: qsTr("Please choose an Atlas project");
        selectMultiple: false;
        nameFilters: [ "Atlas project (*.apro)"];
        folder: atlas.homeFolder;
        onAccepted: {
            atlas.openProject(fileUrl);
        }
    }

    FileDialog{
        id: dialog_saveas
        title: qsTr("Please choose target folder");
        selectMultiple: false;
        folder: atlas.homeFolder;
        onAccepted: {
            atlas.saveAs(fileUrl);
        }
        //fileUrl: "anv.apro";
    }

    HelpDialog{
        id:help_dialog;
    }

    AboutDialog{
        id: about_dialog;
    }

    NewProjectDialog{
        id: new_project_dialog;
        atlasWindow: atlas;
    }

    EnvironmentDialog{
        id: environment_dialog;
        project: atlas.project;
    }

    OptionsDialog {
        id: options_dialog;
    }

    SystemPalette{
        id:syspal;
    }

    ProjectDialog{
        id: project_dialog;
        project: atlas.project
    }

    Atlas.Atlas{
        id: atlas;
        workspace: workspace;
        projectLoader: loading;
        welcome: welcome;
    }

    property alias atlas: atlas;//make crash???


/* TBD
    MessageDialog{
        id: close_dialog;
        title: qsTr("Close project");
        text: qsTr("Do you want to close project without saving?");
        onAccepted: {
            window.closeProject();
        }
    }
*/

    menuBar: MenuBar{
        Menu {
            title: qsTr("File")
            MenuItem { text: qsTr("New Project"); shortcut: StandardKey.New; onTriggered:  new_project_dialog.open()}
            MenuItem { text: qsTr("Open Project");shortcut: StandardKey.Open;  onTriggered: dialog_open_project.open() }
           // MenuItem { text: qsTr("Recent Projects");}
            MenuSeparator{}
            MenuItem { text: qsTr("Save"); enabled: atlas.projectOpen; shortcut: StandardKey.Save; onTriggered: atlas.saveAll(); }
            //MenuItem { text: qsTr("Save As..."); enabled: atlas.projectOpen; onTriggered: dialog_saveas.open();}
            MenuItem { text: qsTr("Close Project"); enabled: atlas.projectOpen; shortcut: StandardKey.Close; onTriggered: atlas.closeProject();}
            MenuSeparator{}
            MenuItem { text: qsTr("Quit"); shortcut: StandardKey.Quit; onTriggered: Qt.quit()}
        }

        Menu {
            title: qsTr("Edit")
            MenuItem { text: qsTr("Undo"); enabled: atlas.projectOpen; shortcut: StandardKey.Undo; onTriggered: atlas.undo(); }
            MenuItem { text: qsTr("Redo"); enabled: atlas.projectOpen; shortcut: StandardKey.Redo; onTriggered: atlas.redo(); }
            MenuSeparator{}
            MenuItem { text: qsTr("Make Screenshot"); enabled: atlas.projectOpen; onTriggered: workspace.makeSnapshot();}
            MenuItem { text: qsTr("Environment"); enabled: atlas.projectOpen; onTriggered: environment_dialog.open(); }
            MenuItem { text:qsTr("Project"); enabled: atlas.projectOpen; onTriggered: project_dialog.open();}
            MenuSeparator{}
            MenuItem { text: qsTr("Options"); onTriggered: options_dialog.open()}
        }

        Menu {
            id: menu_view;
            title: qsTr("View");
            MenuItem { text: qsTr("Show Model"); enabled: atlas.projectOpen; checkable: true; checked: atlas.modelVisible; onCheckedChanged: atlas.modelVisible=checked;}
       //TODO     MenuItem { text: qsTr("Show Grass"); enabled: atlas.projectOpen; checkable: true; checked: true;}
            MenuItem { text: qsTr("Show Terrain"); enabled: atlas.projectOpen; checkable: true; checked: atlas.terrainVisible; onCheckedChanged:  atlas.terrainVisible=checked;}
            MenuItem { text: qsTr("Show Ocean");  enabled: atlas.projectOpen; checkable: true; checked: atlas.oceanVisible; onCheckedChanged: atlas.oceanVisible=checked;}
            MenuSeparator{}
            MenuItem { text: qsTr("Wireframe"); enabled: atlas.projectOpen; shortcut: "f2" ; checkable: true; checked: atlas.wireframe; onToggled: atlas.setWireframe(checked);}
            MenuItem { text: qsTr("Shadow"); enabled: atlas.projectOpen; checkable: true; checked: atlas.shadow; onToggled: atlas.setShadow(checked);}
            MenuItem { text: qsTr("Light"); enabled: atlas.projectOpen; checkable: true; checked: atlas.lightVisible; onToggled: atlas.setLightVisible(checked);}
            MenuSeparator{}
            MenuItem { text: qsTr("Virtual Gamepad"); enabled: atlas.projectOpen; checkable: true; checked: atlas.gamepad; onToggled: atlas.setGamepad(checked);}
            MenuItem { text: qsTr("Fullscreen"); shortcut: StandardKey.FullScreen; checkable: true; checked: atlas.fullscreen; onToggled: atlas.setFullscreen(checked)}
            MenuItem { text: qsTr("3D VR"); checkable: true; checked: atlas.renderVr; onToggled: atlas.setRenderVr(checked);}
        }

        Menu {
            id: menu_tools;
            title: qsTr("Tools");
            enabled: atlas.projectOpen;
        }

        Menu {
            title: qsTr("Help");
            MenuItem { text: qsTr("Help"); shortcut: StandardKey.HelpContents; onTriggered: help_dialog.open();}
            MenuItem { text: qsTr("About"); onTriggered: about_dialog.open();}
        }
    }

    Workspace{
        id: workspace;
        visible: false;
        anchors.fill: parent;
    }

    //WELCOME
    Welcome{
        id: welcome;
        anchors.fill: parent;
        visible: false;
    }
    //LOADING
    ProjectLoading{
        id: loading;
        visible: false;
        anchors.fill: parent;
    }
}

