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

Atlas.TerrainImportDialog {
    id: root;
    color: syspal.window;
    fillWindow: true;
    imageProvider: image;
    title: qsTr("Import terrain");

    Item{
        implicitWidth: layout.implicitWidth+10;
        implicitHeight: layout.implicitHeight+10;
        FileDialog{
            id: file_dialog;
            title: qsTr("Please select an image");
            onAccepted: {
                root.importImage(fileUrl);
            }
            nameFilters:  qsTr("Image files ")+"("+root.supportedFormats+")";
        }

        ColumnLayout{
            id: layout
            anchors.fill: parent;
            anchors.margins: 5;

            RowLayout{
                Layout.fillHeight: true;
                ScrollView{
                    Layout.fillHeight: true;
                    Layout.minimumWidth: grid.implicitWidth;
                    ColumnLayout{
                        id: grid;

                        GroupBox{
                            title: qsTr("Size");
                            Layout.fillWidth: true;
                            GridLayout{
                                columns: 2
                                Label{
                                    text: qsTr("Min. Height");
                                }
                                SpinBox{
                                    id: height_min;
                                    Layout.fillWidth: true;
                                    decimals: 2;
                                    value: root.minHeight;
                                    maximumValue: root.maxHeight;
                                    onValueChanged: {
                                        root.minHeight=value;
                                    }
                                }

                                Label{
                                    text: qsTr("Max. Height");
                                }
                                SpinBox{
                                    id: height_max;
                                    Layout.fillWidth: true;
                                    decimals: 2;
                                    value: root.maxHeight;
                                    minimumValue: root.minHeight;
                                    maximumValue: root.heightLimit;
                                    onValueChanged: {
                                        root.maxHeight=value;
                                    }
                                }
                            }
                        }
                        GroupBox{
                            title: qsTr("Material");
                            Layout.fillWidth: true;
                            GridLayout{
                                columns: 2
                                Label{
                                    text: qsTr("Vertical")
                                }

                                ComboBox{
                                    id: material_v;
                                    Layout.fillWidth: true;
                                    currentIndex: root.verticalTexture;
                                    model: root.model;
                                    onCurrentIndexChanged: {
                                        root.verticalTexture=currentIndex;
                                    }
                                }
                                Label{
                                    text: qsTr("Horizontal")
                                }

                                ComboBox{
                                    id: material_h;
                                    Layout.fillWidth: true;
                                    currentIndex: root.horizontalTexture;
                                    model: root.model;
                                    onCurrentIndexChanged: {
                                        root.horizontalTexture=currentIndex;
                                    }
                                }
                                Label{
                                    Layout.columnSpan:2;
                                    visible: material_h.count===0
                                    text: qsTr("There is no material to apply on terrain!");
                                    color: "red";
                                }
                            }
                        }
                        Item{
                            Layout.fillHeight: true;
                        }
                        Button{
                            text: qsTr("Import image");
                            onClicked: file_dialog.open();
                        }

                    }
                }
                Rectangle{
                    color: "white";
                    Layout.fillHeight: true;
                    Layout.fillWidth: true;
                    Layout.minimumHeight: 256;
                    Layout.minimumWidth: 256;
                    Atlas.ImageProvider{
                        id: image;
                        anchors.fill: parent;
                    }
                }
            }

            RowLayout{
                Layout.fillWidth: true;
                Item{
                    Layout.fillWidth: true;
                }
                Button
                {
                    text: qsTr("Ok");
                    onClicked: {
                        root.accept();
                    }
                }
                Button{
                    text: qsTr("Cancel");
                    onClicked: {
                        root.reject();
                    }
                }
            }
        }
    }
}
