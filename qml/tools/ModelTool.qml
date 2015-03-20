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

Atlas.ModelControl {
    id: tool;
    title: qsTr("Model");

    Component.onCompleted: {
        var sub=menu_tools.addMenu(tool.title);
        var item=sub.addItem(qsTr("Import Mesh"));
        item.onTriggered.connect(import_dialog.open);
        item=sub.addItem(qsTr("Import Asset"));
        item.onTriggered.connect(asset_dialog.open);
    }

    MessageDialog {
        id: mesh_delete_dialog;
        property var index;
        title: qsTr("Delete mesh");
        text: qsTr("Do you want to delete this mesh and all its objects?");
        standardButtons: StandardButton.Yes | StandardButton.No;
        onYes: {
            tool.deleteMesh(index);
            mesh_box.visible=true;
            model_box.visible=false;
        }
    }
/* TBD
    ModelEditDialog{
        id: model_edit
    }
*/
    AssetMeshDialog{
        id: asset_dialog;
        project: atlas.project;
        onAccepted: {
            tool.importMesh(meshUrl);
        }
    }

    FileDialog{
        id: import_dialog;
        onAccepted: {
            tool.importMesh(fileUrl);
        }
        nameFilters: [ "Mesh files (*.3ds *.obj)" ];
    }

    controlPanel: Item {
        implicitHeight: tool_layout.implicitHeight;
        implicitWidth: tool_layout.implicitWidth;

        Connections{
            target: tool;
            onSelectModel:{
                tree_view.selectModel(meshIndex,modelIndex);
            }
        }

        Connections{
            target: tool;
            onSelectMesh:{
                tree_view.selectMesh(meshIndex);
            }
        }

        ColumnLayout{
            id: tool_layout
            anchors.fill: parent;
            anchors.margins: 3;

            ModelSelect{
                id: tree_view;
                model: tool.treeModel;

                Layout.fillWidth: true;
                Layout.fillHeight: true;

                onModelModelIndexChanged:
                {
                    model_box.index=modelModelIndex;
                    tool.modelIndex=modelModelIndex;
                    mesh_box.visible=false;
                    model_box.visible=true;

                }
                onMeshModelIndexChanged:
                {
                    mesh_box.index=meshModelIndex;
                    tool.meshIndex=meshModelIndex;
                    model_box.visible=false;
                    mesh_box.visible=true;
                }
            }

            ScrollView{
                id: model_box;
                Layout.fillWidth: true;
                Layout.maximumHeight: parent.height/2;
                Layout.preferredHeight: contentItem.implicitHeight+6;
                visible: false;
                flickableItem.anchors.margins: 3;
                frameVisible: true;
                viewport.onWidthChanged: {
                    if(visible && modelInfo.width<viewport.width-6)
                        modelInfo.width=viewport.width-6;
                }

                property var index;
                property bool showBig: height<tool_layout.height/3;

                onShowBigChanged: {
                    position_data.visible=op_position.checked || showBig;
                    scale_data.visible      =op_scale.checked || showBig;
                    rotation_data.visible=op_rotation.checked || showBig;
                }

                onVisibleChanged: {
                    position_data.visible=op_position.checked || showBig;
                    scale_data.visible      =op_scale.checked || showBig;
                    rotation_data.visible=op_rotation.checked || showBig;
                }

                GridLayout{
                    id: modelInfo;
                    columns: 2;

                    Label{
                        text: qsTr("Name:");
                    }
                    TextField{
                        id: model_name;
                        text: tool.model.name;
                        implicitWidth: 10;// hack to disable idiotic width in Layout
                        Layout.fillWidth: true;
                        onTextChanged: {
                            tool.model.name=text;
                        }
                    }

                    RowLayout{
                        Layout.columnSpan: 2;
                        spacing: 0;
                        ExclusiveGroup{
                            id: op_group
                        }

                        Button{
                            id: op_position;
                            Layout.fillWidth: true;
                            text: qsTr("Position");
                            checkable: true;
                            checked: tool.modelOperation===Atlas.ModelControl.POSITION;
                            exclusiveGroup: op_group;
                            onCheckedChanged: {
                                if(checked)
                                {
                                    position_data.visible=true;
                                    scale_data.visible   =model_box.showBig;
                                    rotation_data.visible=model_box.showBig;
                                    tool.modelOperation=Atlas.ModelControl.POSITION;
                                }
                            }
                        }
                        Button{
                            id: op_scale;
                            Layout.fillWidth: true;
                            text: qsTr("Scale");
                            checkable: true;
                            checked: tool.modelOperation===Atlas.ModelControl.SCALE;
                            exclusiveGroup: op_group;
                            onCheckedChanged: {
                                if(checked)
                                {
                                    scale_data.visible   =true;
                                    position_data.visible=model_box.showBig;
                                    rotation_data.visible=model_box.showBig;
                                    tool.modelOperation=Atlas.ModelControl.SCALE;
                                }
                            }
                        }
                        Button{
                            id: op_rotation;
                            Layout.fillWidth: true;
                            text: qsTr("Rotation");
                            checkable: true;
                            checked: tool.modelOperation===Atlas.ModelControl.ROTATION;
                            exclusiveGroup: op_group;
                            onCheckedChanged: {
                                if(checked)
                                {
                                    scale_data.visible   =model_box.showBig;
                                    position_data.visible=model_box.showBig;
                                    rotation_data.visible=true;
                                    tool.modelOperation=Atlas.ModelControl.ROTATION;
                                }
                            }
                        }
                    }

                    RowLayout{
                        id: position_data;
                        visible: false;
                        Layout.columnSpan: 2;

                        Label{
                            text: "x"
                        }
                        SpinBox{
                            id: model_x;
                            Layout.fillWidth: true;
                            decimals: 1;
                            value: tool.model.positionX;
                            maximumValue: tool.mapWidth;
                            onValueChanged: {
                                tool.model.positionX=value;
                            }
                        }
                        Label{
                            text: "y"
                        }
                        SpinBox{
                            id: model_y;
                            Layout.fillWidth: true;
                            decimals: 1;
                            value: tool.model.positionY;
                            maximumValue: tool.mapHeight;
                            onValueChanged:{
                                tool.model.positionY=value;
                            }
                        }
                        Label{
                            text: "z"
                        }
                        SpinBox{
                            id: model_z;
                            Layout.fillWidth: true;
                            decimals: 1;
                            value: tool.model.positionZ;
                            maximumValue: tool.mapDepth;
                            onValueChanged: {
                                tool.model.positionZ=value;
                            }
                        }
                    }

                    RowLayout{
                        id: rotation_data;
                        visible: false;
                        Layout.columnSpan: 2;
                        Label{
                            text: "α"
                        }

                        SpinBox{
                            id: model_a;
                            decimals: 1;
                            value: tool.model.rotationX;
                            maximumValue: 360;
                            Layout.fillWidth: true;
                            suffix: "°";

                            onValueChanged:{
                                tool.model.rotationX=value;
                            }
                        }
                        Label{
                            text: "β"
                        }

                        SpinBox{
                            id: model_b;
                            decimals: 1;
                            value: tool.model.rotationY;
                            maximumValue: 360;
                            Layout.fillWidth: true;
                            suffix: "°";

                            onValueChanged:{
                                tool.model.rotationY=value;
                            }
                        }
                        Label{
                            text: "γ"
                        }
                        SpinBox{
                            id: model_g;
                            decimals: 1;
                            value: tool.model.rotationZ;
                            maximumValue: 360;
                            Layout.fillWidth: true;
                            suffix: "°";
                            onValueChanged:{
                                tool.model.rotationZ=value;
                            }
                        }
                    }

                    RowLayout{
                        id: scale_data;
                        visible: false;
                        Layout.columnSpan: 2;
                        Label{
                            text: "x"
                        }
                        SpinBox{
                            decimals: 2;
                            value: tool.model.scaleX;
                            Layout.fillWidth: true;
                            suffix: "x";
                            stepSize: 0.1;
                            onValueChanged:{
                                tool.model.scaleX=value;
                            }
                        }
                        Label{
                            text: "y"
                        }

                        SpinBox{
                            decimals: 2;
                            value: tool.model.scaleY;
                            Layout.fillWidth: true;
                            suffix: "x";
                            stepSize:0.1;

                            onValueChanged:{
                                tool.model.scaleY=value;
                            }
                        }
                        Label{
                            text: "z"
                        }
                        SpinBox{
                            decimals: 2;
                            value: tool.model.scaleZ;
                            Layout.fillWidth: true;
                            suffix: "x";
                            stepSize: 0.1;
                            onValueChanged:{
                                tool.model.scaleZ=value;
                            }
                        }
                    }

                    Button{
                        id: model_delete;
                        text: qsTr("Delete");
                        onClicked: {
                            tool.deleteModel(model_box.index);
                            mesh_box.visible=false;
                            model_box.visible=true;
                        }
                    }
                }
            }

            ScrollView{
                id: mesh_box;
                Layout.fillWidth: true;
                Layout.maximumHeight: parent.height/2;
                Layout.preferredHeight: contentItem.implicitHeight+6;
                visible: false;
                flickableItem.anchors.margins: 3;
                frameVisible: true;
                viewport.onWidthChanged: {
                    if(visible && contentItem.width<viewport.width-6)
                        contentItem.width=viewport.width-6;
                }
                property var index;
                GridLayout{
                    id: gridmesh
                    columns: 2;
                    rowSpacing: 1;
                    columnSpacing: 1;

                    Label{
                        text: qsTr("Name:");
                    }

                    TextField{
                        id: mesh_name;
                        implicitWidth: 10;// hack to disable idiotic width in Layout
                        Layout.fillWidth: true;
                        text: tool.mesh.name;
                        onTextChanged: {
                            tool.mesh.name=text;
                        }
                    }
                    Label{
                        text: qsTr("Scale");
                    }

                    SpinBox{
                        id: mesh_scale;
                        Layout.fillWidth: true;
                        decimals: 2;
                        value: tool.mesh.scale;
                        suffix: "x"
                        onValueChanged: {
                            tool.mesh.scale=value;
                        }
                        maximumValue: 100;
                        minimumValue: 0;
                        stepSize: 0.1;
                    }
                    Button{
                        id: mesh_delete;
                        text: qsTr("Delete");
                        onClicked: {
                            mesh_delete_dialog.index=mesh_box.index;
                            mesh_delete_dialog.open();
                        }
                    }
                }
            }
        }
    }
}
