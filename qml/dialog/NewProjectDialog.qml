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
import "../"
import Atlas 1.0 as Atlas

Atlas.NewProjectDialog {
    id: dialog;
    title: qsTr("New project");
    color: syspal.window;

    Item{
        implicitWidth: layout.implicitWidth+10;
        implicitHeight: layout.implicitHeight+10;

        FileDialog {
            id: folder_dialog
            title: qsTr("Please choose a folder")
           // selectFolder: true; //will be enabled in futute, makes bug on android
            selectFolder: Qt.platform.os==="android"?false:true;
            folder: dialog.url;
            onFolderChanged: dialog.url=folder;
        }

        ColumnLayout{
            id: layout
            anchors.fill: parent;
            anchors.margins: 5;

            GridLayout{
                Layout.fillWidth: true;
                columns: 2;

                Label{
                    text: qsTr("Project Name:")
                }
                TextField{
                    id: project_name
                    text: dialog.name;
                    Layout.fillWidth: true;
                    onTextChanged: dialog.name=text;
                }
                Label{
                    text: qsTr("Author:")
                }
                TextField{
                    id: project_author
                    text:   dialog.author;
                    Layout.fillWidth: true;
                    onTextChanged: dialog.author=text;
                }
                Label{
                    text: qsTr("Folder:")
                }
                RowLayout{
                    Layout.fillWidth: true;
                    TextField{
                        text: dialog.path
                        Layout.fillWidth: true;
                    }
                    Button
                    {
                        text: qsTr("Select...");
                        onClicked: {
                            folder_dialog.open();
                        }
                    }
                }
                Label{
                    text: qsTr("Map size:");
                }
                RowLayout{
                    Layout.fillWidth: true;

                    Label{
                        text: qsTr("width")
                    }
                    SpinBox{
                        id: map_width;
                        Layout.fillWidth: true;
                        value: dialog.width;
                        onValueChanged: dialog.width=value;
                        minimumValue: dialog.step*8;
                        maximumValue: Qt.platform.os==="android" || Qt.platform.os==="windows"?256:4096;
                        stepSize: dialog.step;
                        decimals: 0;
                    }

                    Label{
                        text: qsTr("height")
                    }
                    SpinBox{
                        id: map_height;
                        Layout.fillWidth: true;
                        value: dialog.height;
                        minimumValue: dialog.step*8;
                        maximumValue: Qt.platform.os==="android" || Qt.platform.os==="windows"?256:4096;
                        stepSize: dialog.step;
                        onValueChanged: dialog.height=value;
                        decimals: 0;
                    }

                    Label{
                        text: qsTr("depth")
                    }
                    SpinBox{
                        id: map_depth;
                        Layout.fillWidth: true;
                        value: dialog.depth;
                        minimumValue: dialog.step*8;
                        maximumValue: Qt.platform.os==="android" || Qt.platform.os==="windows"?256:4096;
                        stepSize: dialog.step;
                        onValueChanged: dialog.depth=value;
                        decimals: 0;
                    }
                    Item{
                        Layout.fillWidth: true;
                    }
                }
            }
            Item
            {
                Layout.fillHeight: true;
            }

            RowLayout{
                Layout.fillWidth: true;
                spacing: 15;

                Item{
                    Layout.fillWidth: true;
                }

                Button{
                    text: qsTr("OK")
                    onClicked: dialog.accept();
                }
                Button{
                    text: qsTr("Cancel")
                    onClicked: dialog.reject();
                }
            }
        }
    }
}
