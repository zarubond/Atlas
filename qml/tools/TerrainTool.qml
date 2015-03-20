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
import QtQuick.Controls.Styles 1.1
import QtQuick.Dialogs 1.1
import QtGraphicalEffects 1.0
import Atlas 1.0 as Atlas

Atlas.TerrainControl {
    id: main;
    title: qsTr("Terrain");

    Component.onCompleted: {
        var sub=menu_tools.addMenu(main.title);
        var item=sub.addItem(qsTr("Materials"));
        item.onTriggered.connect(material_dialog.open);
        item=sub.addItem(qsTr("Procedural"));
        item.onTriggered.connect(procedural_dialog.open);
        item=sub.addItem(qsTr("Import"));
        item.onTriggered.connect(import_dialog.open);
        //item=menu.addItem("Fill Ground");
        //item.onTriggered.connect(fillGround);
        item=sub.addItem(qsTr("Rebuld"));
        item.onTriggered.connect(rebuildTerrain);
        item=sub.addItem(qsTr("Clear"));
        item.onTriggered.connect(clear_dialog.open);
    }

    SystemPalette{
        id:syspal;
    }

    TerrainMaterialDialog{
        id: material_dialog;
        project: atlas.project;
        tool: main;
    }

    TerrainProceduralDialog{
        id: procedural_dialog;
        project: atlas.project;
    }

    TerrainImportDialog{
        id: import_dialog;
        project: atlas.project;
    }

    MessageDialog{
        id: clear_dialog;
        title: qsTr("Clear terrain");
        text: qsTr("Do you realy want to clear all terrain?");
        icon: StandardIcon.Warning;
        standardButtons: StandardButton.Ok | StandardButton.Cancel
        onAccepted: {
            main.clearTerrain();
        }
    }

    controlPanel:  ScrollView{
        implicitHeight: layout.implicitHeight;
        implicitWidth: layout.implicitWidth;
        flickableItem.anchors.margins: 3;
        flickableItem.anchors.fill: flickableItem.parent;

        ColumnLayout {
            id: layout
            spacing: 1;
            //anchors.fill: parent;
            //anchors.margins: 3;

            GroupBox {
                id: groupBox2
                Layout.fillWidth: true;
                title: qsTr("Tools");
                GridLayout{
                    anchors.fill: parent;
                    columns: 2;
                    rowSpacing: 1;
                    columnSpacing: 1;

                    ExclusiveGroup{
                        id: ex_tool;
                    }

                    Button{
                        text: qsTr("Sculptor");
                        Layout.fillWidth: true;
                        exclusiveGroup: ex_tool;
                        checkable: true;
                        checked: main.tool==Atlas.TerrainControl.SCULPTOR?true:false;
                        onClicked: main.tool=Atlas.TerrainControl.SCULPTOR;
                    }
                    Button{
                        text: qsTr("Eraser");
                        Layout.fillWidth: true;
                        exclusiveGroup: ex_tool;
                        checkable: true;
                        checked: main.tool==Atlas.TerrainControl.ERASER?true:false;
                        onClicked: main.tool=Atlas.TerrainControl.ERASER;
                    }
                    Button{
                        text: qsTr("Brush");
                        Layout.fillWidth: true;
                        exclusiveGroup: ex_tool;
                        checkable: true;
                        checked: main.tool==Atlas.TerrainControl.BRUSH?true:false;
                        onClicked: main.tool=Atlas.TerrainControl.BRUSH;
                    }
                    Button{
                        text: qsTr("Smooth");
                        Layout.fillWidth: true;
                        exclusiveGroup: ex_tool;
                        checkable: true;
                        checked: main.tool==Atlas.TerrainControl.SMOOTH?true:false;
                        onClicked: main.tool=Atlas.TerrainControl.SMOOTH;
                    }
                    Button{
                        text: qsTr("Increser")
                        Layout.fillWidth: true;
                        exclusiveGroup: ex_tool;
                        checkable: true;
                        checked: main.tool==Atlas.TerrainControl.INCREASE?true:false;
                        onClicked: main.tool=Atlas.TerrainControl.INCREASE;
                    }
                    Button{
                        text: qsTr("Decreser");
                        Layout.fillWidth: true;
                        exclusiveGroup: ex_tool;
                        checkable: true;
                        checked: main.tool==Atlas.TerrainControl.DECREASE?true:false;
                        onClicked: main.tool=Atlas.TerrainControl.DECREASE;
                    }
                }
            }

            GroupBox {
                id: material_box;
                Layout.fillWidth: true;
                title: qsTr("Material");

                ColumnLayout{
                    anchors.fill: parent;
                    spacing: 1;

                    RowLayout{
                        spacing: 1;
                        Layout.fillWidth: true;
                        Label{
                            text: qsTr("Horizontal");
                            horizontalAlignment: Text.AlignHCenter
                            Layout.fillWidth: true;
                        }
                        Label{
                            text: qsTr("Vertical")
                            horizontalAlignment: Text.AlignHCenter
                            Layout.fillWidth: true;
                        }
                    }
                    RowLayout{
                        spacing: 1;
                        Button{
                            id: tex_h_button;
                            Layout.fillWidth: true;
                            Layout.minimumHeight: 75;
                            checkable: true;
                            enabled: !blank_material.checked;
                            Image{
                                id: horizontal_texture;
                                fillMode: Image.PreserveAspectFit
                                anchors.fill: parent;
                                anchors.margins: 5;
                                anchors.centerIn: parent;
                                //source: tex_v.currentItem.texture_path;
                                source: (tex_h.currentItem !== null)?tex_h.currentItem.texture_path:"";
                            }
                            onClicked:{
                                if(tex_h.visible)
                                    tex_h.close();
                                else
                                    tex_h.open(main.horizontalTexture);
                            }
                        }

                        Button{
                            id: tex_v_button;
                            Layout.fillWidth: true;
                            Layout.minimumHeight: 75;
                            checkable: true;
                            enabled: !blank_material.checked;
                            Image{
                                id: vertical_texture;
                                fillMode: Image.PreserveAspectFit
                                anchors.fill: parent;
                                anchors.margins: 5;
                                anchors.centerIn: parent;
                                //source: tex_v.currentItem.texture_path;
                                source: (tex_v.currentItem !== null)?tex_v.currentItem.texture_path:"";
                            }
                            onClicked:{
                                if(tex_v.visible)
                                    tex_v.close();
                                else
                                    tex_v.open(main.verticalTexture);
                            }
                        }
                    }

                    TextureSelector{
                        id: tex_h;
                        onClosed: tex_h_button.checked=false;
                        Layout.fillWidth: true;
                        Layout.minimumHeight: itemHeight;
                        model: main.textureModel;
                        onCurrentItemChanged: {
                            horizontal_texture.source=currentItem.texture_path;
                            main.horizontalTexture=currentIndex;
                        }
                    }
                    TextureSelector{
                        id: tex_v;
                        Layout.fillWidth: true;
                        Layout.minimumHeight: itemHeight;
                        onClosed:tex_v_button.checked=false;
                        model: main.textureModel;
                        onCurrentItemChanged: {
                            vertical_texture.source=currentItem.texture_path;
                            main.verticalTexture=currentIndex;
                        }
                    }

                    GridLayout{
                        columns: 3;
                        rowSpacing: 1;
                        columnSpacing: 1;
                        Label{
                            Layout.columnSpan: 2;
                            text: qsTr("Blank material");
                        }
                        CheckBox{
                            id: blank_material;
                            checked: main.blankTexture;
                            onCheckedChanged: {
                                 main.blankTexture=checked;
                            }
                        }

                        Label{
                            id: fade_label;
                            text: qsTr("Fade");
                        }

                        Slider{
                            id: materialFade;
                            value: main.materialFade;
                            tickmarksEnabled: true;
                            stepSize: 0.1;
                            onValueChanged: {
                                main.materialFade=value;
                            }
                            Layout.fillWidth: true;
                        }
                        RadialGradient {
                            width: fade_label.height;
                            height: fade_label.height;
                            horizontalRadius: fade_label.height;
                            verticalRadius: fade_label.height;
                            gradient: Gradient {
                                GradientStop { position: 0.0; color: "black" }
                                GradientStop { position: 0.5+main.materialFade; color: "white" }
                            }
                            source:  Rectangle{
                                radius: height;
                                width: fade_label.height;
                                height: fade_label.height;
                            }
                        }

                        Label{
                            id: opacity_label;
                            text:qsTr("Opacity");
                        }

                        Slider{
                            id: materialOpacity;
                            value: main.materialOpacity;
                            tickmarksEnabled: true;
                            stepSize: 0.1;
                            onValueChanged: {
                                main.materialOpacity=value;
                            }
                            Layout.fillWidth: true;
                        }

                        Rectangle{
                            width: opacity_label.height;
                            height: opacity_label.height;
                            color: "white";
                            Rectangle{
                                anchors.fill: parent;
                                color: "black";
                                opacity: main.materialOpacity;
                            }

                        }
                    }
                }
            }
            GroupBox {
                id: groupBox3
                Layout.fillWidth: true;
                title: qsTr("Brush");

                GridLayout{
                    anchors.fill: parent;
                    columns: 2;
                    rowSpacing: 1;
                    columnSpacing: 1;
                    Label{
                        text: qsTr("Pointer");
                    }

                    RowLayout{
                        spacing: 1;
                        ExclusiveGroup{
                            id: pointerType;
                        }
                        Button{
                            text: qsTr("Magnet");
                            Layout.fillWidth: true;
                            onClicked: main.setPointerType(Atlas.TerrainControl.MAGNET);
                            exclusiveGroup: pointerType;
                            checkable: true;
                            checked: main.pointerType==Atlas.TerrainControl.MAGNET?true:false;
                        }
                        Button{
                            text: qsTr("Pointer");
                            Layout.fillWidth: true;
                            onClicked: main.setPointerType(Atlas.TerrainControl.POINTER);
                            exclusiveGroup: pointerType;
                            checkable: true;
                            checked: main.pointerType==Atlas.TerrainControl.POINTER?true:false;
                        }
                    }
                    Label{
                        text: qsTr("Lock");
                    }
                    RowLayout{
                        spacing: 1;
                        CheckBox{
                            id: lock_x;
                            text: "X";
                            checked: main.lockX;
                            onCheckedChanged: main.lockX=checked;
                        }
                        CheckBox{
                            id: lock_y;
                            text: "Y";
                            checked: main.lockY;
                            onCheckedChanged: main.lockY=checked;
                        }
                        CheckBox{
                            id: lock_z;
                            text: "Z";
                            checked: main.lockZ;
                            onCheckedChanged: main.lockZ=checked;
                        }
                    }

                    Label{
                        text: qsTr("Offset")
                        Layout.fillWidth: true;
                    }
                    Slider{
                        id: pointer_offset;
                        Layout.fillWidth: true;
                        value: main.pointerOffset;
                        onValueChanged: main.pointerOffset=value;
                    }
                    Label{
                        text: qsTr("Radius");
                        Layout.fillWidth: true;
                    }
                    Slider{
                        id: pointer_size;
                        Layout.fillWidth: true;
                        value: main.pointerSize;
                        onValueChanged: main.pointerSize=value;
                    }
                    Label{
                        text: qsTr("Shape");
                    }

                    RowLayout{
                        spacing: 1;
                        ExclusiveGroup{
                            id: ex_shape;
                        }
                        Button{
                            text: qsTr("Sphere");
                            Layout.fillWidth: true;
                            onClicked: main.setPointerShape(Atlas.TerrainControl.SPHERE);
                            checked: main.pointerShape==Atlas.TerrainControl.SPHERE?true:false;
                            exclusiveGroup: ex_shape;
                            checkable: true;
                        }
                        Button{
                            text: qsTr("Box")
                            Layout.fillWidth: true;
                            onClicked: main.setPointerShape(Atlas.TerrainControl.BOX);
                            exclusiveGroup: ex_shape;
                            checkable: true;
                            checked: main.pointerShape==Atlas.TerrainControl.BOX?true:false;
                        }
                    }
                }
            }

            Item{
                Layout.fillWidth: true;
                Layout.fillHeight: true;
            }
        }
    }
}
