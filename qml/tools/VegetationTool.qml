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
import Atlas 1.0 as Atlas

Atlas.VegetationTool {
    id: root;
    title: qsTr("Vegetation");

    signal grassPlant();
    signal grassDig();
    signal pointerSize(real value);

    signal openTextureDialog();
    property alias pointer_size: pointer_size.value;

    Component.onCreated:{
        var sub=menu_tool.addMenu(root.title);
        var item=sub.addItem("Set texture");
        item.onTriggered.connect(openTextureDialog);
    }

    controlPanel: Item {
        implicitHeight: tool_layout.implicitHeight;
        implicitWidth: tool_layout.implicitWidth;

        ColumnLayout
        {
            id:tool_layout;
            anchors.fill: parent;
            anchors.margins: 4

            GroupBox
            {
                Layout.fillWidth: true;
                title: qsTr("Grass");

                ColumnLayout
                {
                    anchors.fill: parent;
                    ExclusiveGroup{
                        id: ex_tool;
                    }
                    RowLayout{
                        Button{
                            exclusiveGroup: ex_tool;
                            checkable: true;
                            checked: true;
                            text: qsTr("Plant");
                            onClicked: root.grassPlant();
                        }

                        Button{
                            exclusiveGroup: ex_tool;
                            checkable: true;
                            text: qsTr("Dig");
                            onClicked: root.grassDig();
                        }
                    }

                    Label{
                        text: qsTr("Size");
                        Layout.fillWidth: true;
                    }

                    Slider{
                        id: pointer_size;
                        Layout.fillWidth: true;
                        onValueChanged: root.pointerSize(value);
                    }
                }
            }

            Item{
                Layout.fillHeight: true;
                Layout.fillWidth: true;
            }
        }
    }
}
