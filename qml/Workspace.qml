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
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.0
import QtQuick.Window 2.1
import Atlas 1.0 as Atlas
import './tools'

Atlas.Workspace {
    id: main
    canvas: canvas;

    property bool showControls: true;

    FileDialog{
        id: screeshot_path;
        title: qsTr("Screenshot path");
        onAccepted: main.saveScreenShot(fileUrl);
        selectExisting: false;
        nameFilters: [ "Image files (*.png *.bmp)" ];
    }

    SystemPalette
    {
        id:syscolor;
    }

    TerrainTool{
        id: terrain_tool;
    }

    ModelTool{
        id: model_tool;
    }

    onSelectScreenhotPath:{
        screeshot_path.open();
    }

    Component.onCompleted: {
        main.addTool(terrain_tool);
        main.addTool(model_tool);
    }

    ColumnLayout
    {
        id: workspace_layout;
        anchors.fill: parent;
        anchors.leftMargin: 3;
        anchors.bottomMargin: 0;
        SplitView
        {
            Layout.fillHeight: true;
            Layout.fillWidth: true;
            orientation: Qt.Horizontal

            TabView{
                id: tool_tabview
                visible: main.showControls;
                Layout.fillHeight: true;
                Layout.minimumHeight: implicitHeight;
                Layout.minimumWidth: implicitWidth;
                onCurrentIndexChanged: {
                    main.selectTool(currentIndex);
                    updateSize();
                }

                Tab{
                    title: terrain_tool.title;
                    sourceComponent: terrain_tool.controlPanel;
                }

                Tab{
                    title: model_tool.title;
                    sourceComponent: model_tool.controlPanel;
                }

                onVisibleChanged: {
                    if(visible===true) updateSize();
                }

                function updateSize()
                {
                    var tab=getTab(currentIndex);
                    /*if(tab.item.implicitWidth+10>width)
                    {
                        Layout.minimumWidth=tab.item.implicitWidth+10;
                        width=tab.item.implicitWidth+10;
                    }
                    else
                    {
                        Layout.minimumWidth=implicitWidth;
                        width=implicitWidth;
                    }*/
                    if(tab.item.implicitWidth>0)
                    {
                        Layout.minimumWidth=tab.item.implicitWidth+10;
                        width=tab.item.implicitWidth+10;
                    }
                }
            }

            Atlas.Canvas{
                id: canvas;
                Layout.minimumWidth: 100
                Layout.fillWidth: true;
                Layout.fillHeight: true;

                RowLayout{
                    id: button_row;
                    spacing: 1;
                    anchors.top: parent.top;
                    anchors.topMargin: spacing
                    anchors.horizontalCenter: parent.horizontalCenter;
                    ExclusiveGroup{
                        id: driver
                    }
                    Button{
                        //text: qsTr("Free");
                        text: "||"
                        exclusiveGroup: driver;
                        onClicked: main.play=false;
                        checkable: true;
                        checked: !main.play;
                        implicitWidth: height*1.5;
                    }

                    Button{
                        //text: qsTr("Player");
                        id: play_button;
                        text: "►"
                        checked: main.play;
                        exclusiveGroup: driver;
                        onClicked: main.play=true;
                        checkable: true;
                        implicitWidth: height*1.5;
                    }
                    Button{
                        Rectangle{
                            anchors.fill: parent;
                            anchors.margins: parent.height*0.2;
                            border.color: "black";
                            border.width: 2;
                            Rectangle{
                                visible: !main.showControls;
                                color: "black"
                                anchors.left: parent.left;
                                anchors.top: parent.top;
                                anchors.bottom: parent.bottom;
                                height: parent.height;
                                width: parent.width/3;
                            }
                        }


                        implicitWidth: height*1.5;
                        onClicked: {
                            main.showControls=!main.showControls;
                        }
                    }
                    Button{
                        text: "><"
                        visible: atlas.fullscreen;
                        implicitWidth: height*1.5;
                        onClicked: {
                            atlas.fullscreen=false;
                        }

                    }
                }


                JoyStick{
                    id: gamepad1;
                    width: Screen.pixelDensity*main.gamepadSize<parent.width/3?Screen.pixelDensity*main.gamepadSize:parent.width/3;

                    height: width;
                    anchors.left: parent.left;
                    anchors.leftMargin: Screen.pixelDensity*3;
                    anchors.bottom: parent.bottom;
                    anchors.bottomMargin: Screen.pixelDensity*3;
                    onMove: main.gamepadJoyStickLeft(vec);
                    visible: atlas.gamepad;
                }
                Item{
                    visible: atlas.gamepad;
                    anchors.left: gamepad1.right
                    anchors.bottom: gamepad1.bottom;
                    anchors.leftMargin: 20;
                    height: gamepad1.height;
                    Button{
                        text: "↑";
                        anchors.top: parent.top;
                        anchors.topMargin: (parent.height-height*2)/2;
                        width: height;

                        onPressedChanged: {
                            main.goUp(pressed);
                        }
                    }
                    Button{
                        text: "↓";
                        anchors.bottom: parent.bottom;
                        anchors.bottomMargin: (parent.height-height*2)/2;
                        width: height;
                        onPressedChanged: {
                            main.goDown(pressed);
                        }
                    }
                }

                JoyStick{
                    id: gamepad2;
                    width: Screen.pixelDensity*main.gamepadSize<parent.width/3?Screen.pixelDensity*main.gamepadSize:parent.width/3;
                    height:width;
                    anchors.right: parent.right
                    anchors.rightMargin: Screen.pixelDensity*3;
                    anchors.bottom: parent.bottom;
                    anchors.bottomMargin: Screen.pixelDensity*3;
                    onMove: main.gamepadJoyStickRight(vec);
                    visible: atlas.gamepad;
                }

                Label{
                    id: fps;
                    anchors.right: parent.right;
                    anchors.top: parent.top;
                    color: "red"
                    text: canvas.fps;
                    font.pointSize: 24;
                }
/*

                Button{
                   // anchors.left: parent.left;
                    anchors.horizontalCenter: parent.horizontalCenter;
                    anchors.bottom: parent.bottom;
                    id: enlarge;
                    text:"<>";
                    visible:! main.showControls;

                    width: height*1.5;
                    onClicked: {
                        main.showControls=true;
                    }
                }*/
            }
        }
        /*
        RowLayout
        {
            id: bottom;
            visible: main.showControls;

            ExclusiveGroup{
                id: driver
            }

            Button{
                //text: qsTr("Free");
                text: "Pl"
                exclusiveGroup: driver;
                onClicked: main.driverFree();
                checkable: true;
                checked: true;
            }
            Button{
                //text: qsTr("Player");
                text: "Ps"
                exclusiveGroup: driver;
                onClicked: main.driverPlayer();
                checkable: true;
            }
            Item{
                Layout.fillWidth: true;
            }

            Button{
                text: "><";
                implicitWidth: height*1.5;
                onClicked: {
                    main.showControls=false;
                }
            }
            Item{
                Layout.fillWidth: true;
            }
            Button{
                visible: Qt.platform.os==="android" && atlas.fullscreen;
                onClicked: atlas.setFullscreen(false);
                text: qsTr("Fullscreen");
            }

            Label{
                text: qsTr("Speed");
            }

            SpinBox {
                id: speed
                decimals: 2;
                stepSize: 0.1;
                value: main.speed;
                onValueChanged: {
                    main.speed=value;
                }
            }
        }*/
    }
}
