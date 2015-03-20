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
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.1
import QtQuick.Dialogs 1.1
import QtQuick.Window 2.0
import Atlas 1.0 as Atlas

Atlas.Welcome {
    width: 800;
    height: 600;
    id: welcome;
    implicitWidth: layout.implicitWidth;
    implicitHeight: layout.implicitHeight;

    SystemPalette{
        id: syspal;
    }

    ColumnLayout{
        id: layout;
        spacing: 50;
        anchors.fill: parent
        anchors.margins: 5

        Atlas.Dialog{
            id: colors;
            GridLayout{
                columns: 4;
                Rectangle{
                    width: 100;
                    height: 50;
                    color: syspal.alternateBase;
                    Text{
                        text: "alternateBase"
                    }
                }
                Rectangle{
                    width: 100;
                    height: 50;
                    color: syspal.base;
                    Text{
                        text: "base"
                    }
                }
                Rectangle{
                    width: 100;
                    height: 50;
                    color: syspal.button;
                    Text{
                        text: "button"
                    }
                }
                Rectangle{
                    width: 100;
                    height: 50;
                    color: syspal.buttonText;
                    Text{
                        text: "buttonText"
                    }
                }
                Rectangle{
                    width: 100;
                    height: 50;
                    color: syspal.dark;
                    Text{
                        text: "dark"
                    }
                }
                Rectangle{
                    width: 100;
                    height: 50;
                    color: syspal.highlight;
                    Text{
                        text: "hightlight"
                    }
                }
                Rectangle{
                    width: 100;
                    height: 50;
                    color: syspal.highlightedText;
                    Text{
                        text: "highlightedText"
                    }
                }
                Rectangle{
                    width: 100;
                    height: 50;
                    color: syspal.light;
                    Text{
                        text: "light"
                    }
                }
                Rectangle{
                    width: 100;
                    height: 50;
                    color: syspal.mid;
                    Text{
                        text: "mid"
                    }
                }
                Rectangle{
                    width: 100;
                    height: 50;
                    color: syspal.midlight;
                    Text{
                        text: "midlight"
                    }
                }
                Rectangle{
                    width: 100;
                    height: 50;
                    color: syspal.shadow;
                    Text{
                        text: "shadow"
                    }
                }
                Rectangle{
                    width: 100;
                    height: 50;
                    color: syspal.text;
                    Text{
                        text: "text"
                    }
                }
                Rectangle{
                    width: 100;
                    height: 50;
                    color: syspal.window;
                    Text{
                        text: "window"
                    }
                }
                Rectangle{
                    width: 100;
                    height: 50;
                    color: syspal.windowText
                    Text{
                        text: "windowText"
                    }
                }
            }
        }

        Label{
            text: "Atlas"
            style: Text.Raised
            font.family: "Verdana"
            font.bold: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 64;
            Layout.fillWidth: true;
            Layout.minimumHeight: contentHeight;
            Layout.minimumWidth: contentWidth;
        }

        RowLayout{
            Layout.fillWidth: true;
            spacing: 5;
            ColumnLayout{
                Layout.fillHeight: true;
                Layout.fillWidth: true;
                Item{
                    Layout.fillWidth: true;
                    height:recent.height;
                }

                Button{
                    text: qsTr("New Project");
                    Layout.fillWidth: true;
                    onClicked: new_project_dialog.open();
                }
                Button{
                    text: qsTr("Open Project");
                    Layout.fillWidth: true;
                    onClicked: {
                        dialog_open_project.open();
                    }
                }
                Button{
                    text: qsTr("Sample project");
                    Layout.fillWidth: true;
                   // onClicked:  atlas.openProject("/sdcard/Atlas/Island/Island.apro");
                    onClicked:  atlas.openProject("assets:/maps/Sample Island/Sample Island.apro");
                    visible: Qt.platform.os==="android";
                }
/*
                Button{
                    text: "colors"
                    onClicked:colors.open();
                }*/

                Item{
                    Layout.fillHeight: true;
                    Layout.fillWidth: true;
                }
            }

            ColumnLayout{
                Layout.fillWidth: true;
                Label{
                    id: recent;
                    text: qsTr("Recent projects");
                    horizontalAlignment: Text.AlignHCenter;
                    Layout.fillWidth: true;
                    font.bold: true;
                    font.pointSize: 18;
                }

                ListView {
                    id: recent_project;
                    Layout.fillHeight: true;
                    Layout.fillWidth: true;
                    model: welcome.projectModel;
                    spacing: 3;
                    delegate: Rectangle {
                        width: recent_project.width;
                        height: colum.height;
                        color: "#00000000";
                        MouseArea{
                            anchors.fill: parent;
                            hoverEnabled: true;
                            onClicked: {
                                welcome.openProject(index);
                            }
                            onHoveredChanged: {
                                if(containsMouse)
                                    parent.color=syspal.alternateBase;
                                else
                                    parent.color="#00000000";
                            }
                        }

                        Column{
                            id: colum;
                            width: parent.width;
                            height: label_title.implicitHeight+label_path.implicitHeight;
                            spacing: 2

                            Label{
                                id: label_title;
                                width: parent.width;
                                text: (index+1)+". "+title;
                                font.underline: true;
                                font.bold: true;
                                anchors.margins: 10;
                            }
                            Label{
                                id: label_path;
                                width: parent.width;
                                anchors.left: parent.left;
                                anchors.leftMargin: 20;
                                text: path;
                                wrapMode: Text.Wrap;
                            }
                        }
                    }
                }
            }
        }
    }
}
