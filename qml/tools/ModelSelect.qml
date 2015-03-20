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

//http://qt-project.org/forums/viewthread/30521/#146845
Rectangle {
    id: view
    border.color: syspal.shadow;
    border.width: 1
    //radius: 3;
    color: syspal.base;

    property var model;
    property var meshModelIndex;
    property var modelModelIndex;

    function selectModel(meshIndex,modelIndex)
    {
        mesh_view.currentIndex=meshIndex;
        mesh_view.currentItem.expand=true;
        mesh_view.currentItem.modelIndex=modelIndex;

        meshModelIndex=meshData.modelIndex(meshIndex);
        modelModelIndex=mesh_view.currentItem.modelData.modelIndex(modelIndex);
    }

    function selectMesh(meshIndex)
    {
        mesh_view.currentIndex=meshIndex;
        mesh_view.currentItem.expand=true;
        meshModelIndex=meshData.modelIndex(meshIndex);
    }

    SystemPalette{
        id:syspal
    }

    ScrollView{
        anchors.fill: parent;
        ListView {
            id: mesh_view
            anchors.margins: 1;
            focus: true;

            model: VisualDataModel {
                id: meshData
                model: view.model
                delegate: Column {
                    id: rect;
                    width: mesh_view.width;
                    property alias expand: model_scroll.visible;
                    property alias modelIndex: model_view.currentIndex;
                    property alias modelData: modelData;

                    Rectangle {
                        id: mesh_title
                        width: parent.width;
                        height: mesh_label.height;
                        color: syspal.button;

                        Text {
                            id: mesh_label
                            anchors.centerIn: parent;
                            text: title;
                            color: syspal.buttonText
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                view.meshModelIndex=meshData.modelIndex(index);
                                mesh_view.currentIndex=index;
                                rect.expand=!rect.expand;
                            }
                        }
                    }

                    ScrollView {
                        id: model_scroll;
                        height: mesh_label*3;
                        width: parent.width;
                        visible: false;
                        frameVisible: true;
                        ListView {
                            id: model_view
                            anchors.margins: 5;
                            model: VisualDataModel {
                                id: modelData
                                model: view.model;
                                rootIndex: meshData.modelIndex(index);
                                delegate: Rectangle {
                                    color: ListView.isCurrentItem ? syspal.highlight : syspal.base
                                    height: model_label.height
                                    width: model_view.width;
                                    Label {
                                        id: model_label
                                        text: title;
                                        color: ListView.isCurrentItem ? syspal.highlightedText : syspal.text
                                    }
                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: {
                                            view.modelModelIndex=modelData.modelIndex(index);
                                            model_view.currentIndex=index;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
