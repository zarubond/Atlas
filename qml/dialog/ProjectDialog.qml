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
import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1
import "../"
import Atlas 1.0 as Atlas

Atlas.ProjectDialog {
    id: root;
    color: syspal.window;

    Item{
        implicitWidth: layout.implicitWidth+10;
        implicitHeight: layout.implicitHeight+10;

        FileDialog{
            id: snapshotBrowse;
            nameFilters: [ "Image files (*.png *.bmp)" ];
        }

        ColumnLayout{
            id: layout
            anchors.fill: parent;
            anchors.margins: 5;

            GridLayout{
                columns: 2;
                Label{
                    text: qsTr("Project name");
                }
                TextField{
                    id: project_name;
                    Layout.fillWidth: true;
                    text: root.name;
                    onTextChanged: root.name=text;
                }

                Label{
                    text: qsTr("Author");
                }
                TextField{
                    id: project_author;
                    Layout.fillWidth: true;
                    text: root.author;
                    onTextChanged: root.author=text;
                }

                Label{
                    text: qsTr("Screenshot")
                }
                RowLayout{
                    spacing: 3;
                    Layout.fillWidth: true;
                    TextField{
                        Layout.fillWidth: true;
                        id: project_snapshot;
                        text: root.snapshot;
                        onTextChanged: root.snapshot=text;
                    }
                    Button{
                        text: qsTr("Browse...");
                        onClicked: snapshotBrowse.open();
                    }
                }

                Label{
                    text: qsTr("Info");
                }
                TextArea{
                    id: project_info;
                    Layout.fillHeight: true;
                    Layout.fillWidth: true;
                    text: root.info;
                    onTextChanged: root.info=text;
                }
            }
            Item{
                Layout.fillHeight: true;
            }

            RowLayout{
                Item{
                    Layout.fillWidth: true;
                }
                Button{
                    text: qsTr("Ok");
                    onClicked: root.accept();
                }
                Button{
                    text: qsTr("Cancel")
                    onClicked: root.reject();
                }
            }
        }
    }
}
