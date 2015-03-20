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
import QtQuick.Window 2.1
import "../"
import Atlas 1.0 as Atlas

Atlas.TerrainMaterialDialog {
    id: root;
    color: syspal.window;
    title: qsTr("Terrain materials");
    width:800;
    height:480;
    fillWindow: true;

    Item{
        implicitWidth: layout.implicitWidth+10;
        implicitHeight: layout.implicitHeight+10;

        FileDialog{
            id: texture_dialog;
            title: qsTr("Please select teture");
            property int index: -1;
            onAccepted: {
                root.replaceTexture(index,fileUrl);
            }
            nameFilters:  qsTr("Image files")+" ("+root.supportedFormats+")";
        }

        FileDialog{
            id: normalmap_dialog;
            title: qsTr("Please select normalmap");
            property int index: -1;
            onAccepted: {
                root.replaceNormalMap(index,fileUrl);
            }
            nameFilters:  qsTr("Image files")+" ("+root.supportedFormats+")";
        }

        AssetTextureDialog{
            id: asset_dialog;
            project: root.project;
            property int index: -1;
            onAccepted: {
                root.setMaterial(index, materialName, materialDiffuse, materialNormal, materialSpecular);
            }
        }

        ColumnLayout{
            id: layout
            anchors.fill: parent;
            anchors.margins: 5;

            TableView{
                id: list;
                Layout.fillWidth: true;
                Layout.fillHeight: true;
                model: root.model;

                rowDelegate: Rectangle{
                    height: list.height/root.maximumMaterials;
                    color: styleData.selected ? itemForeground : (styleData.alternate? syspal.base : syspal.alternateBase)
                }

                TableViewColumn{ title: qsTr("Name"); role: "title"; delegate: Item{width:text_name.font.pointSize*50; TextField{
                        id: text_name;
                        anchors.centerIn: parent;
                        width: parent.width;
                        text: styleData.value;

                        onTextChanged: {
                            //console.log(styleData.value);
                           // root.setMaterialName(styleData.row,text);
                        }
                    }}
                }
                TableViewColumn{ title: qsTr("Texture"); movable: false;  resizable: false; width: list.height/root.maximumMaterials; role: "texture"; delegate: Button{
                        onClicked: {
                            texture_dialog.index=styleData.row;
                            texture_dialog.open();
                        }

                        Image{
                            anchors.fill: parent;
                            anchors.margins: 5;
                            source: styleData.value;
                        }
                    }
                }
                TableViewColumn{title: qsTr("Normal Map"); movable: false; resizable: false; role: "normal"; width: list.height/root.maximumMaterials; delegate: Button{
                        onClicked: {
                            normalmap_dialog.index=styleData.row;
                            normalmap_dialog.open();
                        }

                        Image{
                            anchors.fill: parent;
                            anchors.margins: 5;
                            source: styleData.value;
                        }
                    }
                }
                TableViewColumn{id:asset_button; movable: false; resizable: false; title: qsTr("Import"); delegate: Item{ Button{
                        anchors.centerIn: parent;
                        width: parent.width;
                        text: qsTr("Import Asset");
                        Component.onCompleted: {
                            if(asset_button.width<implicitWidth*1.1)
                                asset_button.width=implicitWidth*1.1;
                        }

                        onClicked: {
                            asset_dialog.index=styleData.row;
                            asset_dialog.open();
                        }
                    }
                }}
            }

            Button{
                text: qsTr("Add material");
                onClicked: {
                    root.addMaterial();
                }
                enabled: (list.rowCount<=root.maximumMaterials);
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
