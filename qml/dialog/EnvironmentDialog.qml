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
import QtQuick.Dialogs 1.1
import "../"
import Atlas 1.0 as Atlas

Atlas.EnvironmentDialog {
    id: root;
    title: qsTr("Environment");
    color: syspal.window;
    fillWindow: true;

    minimumHeight: 240;
    minimumWidth: 320;
    Item{
        implicitWidth: layout.implicitWidth+10;
        implicitHeight: layout.implicitHeight+10;

        ColorDialog {
            id: diffuseDialog
            title: qsTr("Please choose diffuse color");
            color: root.diffuse;
            onAccepted: {
                root.diffuse=currentColor;
            }
        }

        ColorDialog {
            id: specularDialog
            title: qsTr("Please choose specular color");
            color: root.specular;
            onAccepted: {
                root.specular=currentColor;
            }
        }

        ColorDialog {
            id: ambientDialog
            title: qsTr("Please choose ambient color");
            color: root.ambient;
            onAccepted: {
                root.ambient=currentColor;
            }
        }

        ColorDialog{
            id: fogColorDialog
            title: qsTr("Please choose fog color");
            color: root.fogColor;
            onAccepted: {
                root.fogColor=currentColor;
            }
        }

        ColorDialog{
            id: oceanColorDialog
            title: qsTr("Please choose ocean color");
            color: root.oceanColor;
            onAccepted: {
                root.oceanColor=currentColor;
            }
        }

        ColumnLayout{
            id: layout
            anchors.fill: parent;
            anchors.margins: 5;

            TabView{
                id: tab_view;
                Layout.fillHeight: true;
                Layout.fillWidth: true;


                Tab{
                    id: sun_tab
                    title: qsTr("Sun");
                    active: true;

                    GridLayout{
                        id: sun
                        columns: 2
                        anchors.fill: parent;
                        anchors.margins: 4;

                        Label{
                            id: ambient_label;
                            text: qsTr("Ambient");
                        }
                        Rectangle{
                            id:ambient
                            Layout.fillWidth: true;
                            Layout.minimumWidth: 100;
                            height: ambient_label.height;
                            color: root.ambient;
                            MouseArea{
                                anchors.fill: parent;
                                onClicked: {
                                    ambientDialog.open();
                                }
                            }
                        }

                        Label{
                            id: diffuse_label;
                            text: qsTr("Diffuse")
                        }
                        Rectangle{
                            id:diffuse
                            Layout.fillWidth: true;
                            Layout.minimumWidth: 100;
                            height: diffuse_label.height;
                            color: root.diffuse;
                            MouseArea{
                                anchors.fill: parent;
                                onClicked: {
                                    diffuseDialog.open();
                                }
                            }
                        }

                        Label{
                            id: specular_label;
                            text: qsTr("Specular");
                        }
                        Rectangle{
                            id:specular
                            Layout.fillWidth: true;
                            Layout.minimumWidth: 100;
                            height: specular_label.height;
                            color: root.specular;
                            MouseArea{
                                anchors.fill: parent;
                                onClicked: {
                                    specularDialog.open();
                                }
                            }
                        }
                        Label{
                            text: qsTr("Shininess");
                        }
                        Slider{
                            id: shininess
                            value: root.shininess;
                        }

                        Label{
                            text: "X direction";
                        }
                        Slider{
                            id: x_direction
                            minimumValue: 0;
                            maximumValue:  2*Math.PI
                            value: root.sunLambda;
                            onValueChanged: {
                                root.sunLambda=value;
                            }

                        }
                        Label{
                            text: "Y direction";
                        }
                        Slider{
                            id: y_direction;
                            minimumValue: 0;
                            maximumValue: 2*Math.PI;
                            value: root.sunTheta;
                            onValueChanged: {
                                root.sunTheta=value;
                            }
                        }
                        Item{
                            Layout.fillHeight: true;
                        }
                    }
                }
                Tab{
                    id: fog_tab
                    title: qsTr("Fog");

                    GridLayout{
                        id: fog;
                        anchors.fill: parent;
                        anchors.margins: 4;
                        columns: 2;

                        Label{
                            text: qsTr("Enable fog");
                        }
                        CheckBox{
                            checked: root.fogEnable;
                            onCheckedChanged: {
                                root.fogEnable=checked;
                            }
                        }

                        Label{
                            text: qsTr("Density");
                        }
                        SpinBox{
                            id: fog_density;
                            Layout.minimumWidth: 75;
                            minimumValue: 0;
                            maximumValue: 1;
                            stepSize: 0.01;
                            decimals: 2;
                            value: root.fogDensity;
                            onValueChanged: {
                                root.fogDensity=value;
                            }
                        }

                        Label{
                            id: fog_color_label;
                            text: qsTr("Color");
                        }
                        Rectangle{
                            Layout.fillWidth: true;
                            Layout.minimumWidth: 100;
                            height: fog_color_label.height;
                            color: root.fogColor;

                            MouseArea{
                                anchors.fill: parent;
                                onClicked: {
                                    fogColorDialog.open();
                                }
                            }
                        }
                        Item{
                            Layout.fillHeight: true;
                        }
                    }
                }

    /*
                Tab{
                    id: wind_tab
                    title: qsTr("Wind");
                    active: true;

                    GridLayout{
                        id: wind;
                        anchors.fill: parent;
                        anchors.margins: 4;
                        columns: 2;
                        Label{
                            text: qsTr("Speed");
                        }
                        SpinBox{
                            Layout.minimumWidth: 75;
                        }
                        Item{
                            Layout.fillHeight: true;
                        }
                    }
                }*/
                Tab{
                    id: ocean_tab;
                    title: qsTr("Ocean")

                    GridLayout{
                        id: ocean
                        anchors.fill: parent;
                        anchors.margins: 4;
                        columns: 2;

                        Label{
                            text: qsTr("Enabled")
                        }
                        CheckBox{
                            id: ocean_enabled;
                            checked: root.oceanEnable;
                            onCheckedChanged: {
                                root.oceanEnable=checked;
                            }
                        }
                        Label{
                            text: qsTr("Height");
                        }
                        SpinBox{
                            id:  ocean_height;
                            Layout.minimumWidth: 75;
                            decimals: 2;
                            minimumValue: 0;
                            maximumValue: root.oceanMaxHeight;
                            value: root.oceanHeight;
                            onValueChanged: {root.oceanHeight=value;}

                        }
                        Label{
                            id: ocean_color_label;
                            text: qsTr("Color")
                        }
                        Rectangle{
                            Layout.fillWidth: true;
                            Layout.minimumWidth: 100;
                            height: ocean_color_label.height;
                            color: root.oceanColor;
                            MouseArea{
                                anchors.fill: parent;
                                onClicked: {
                                    oceanColorDialog.open();
                                }
                            }
                        }
                        Label{
                            text: qsTr("Opacity");
                        }
                        Slider{
                            value: root.oceanOpacity
                            onValueChanged: root.oceanOpacity=value;
                        }

                        Item{
                            Layout.fillHeight: true;
                        }
                    }
                }
            }

            RowLayout{
                Layout.fillWidth: true;
                Item{
                    Layout.fillWidth: true;
                }

                Button{
                    text: qsTr("Ok");
                    onClicked: root.accept();
                }
                Button{
                    text: qsTr("Cancel");
                    onClicked: root.reject();
                }
            }
        }
    }
}
