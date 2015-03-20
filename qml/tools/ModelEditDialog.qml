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
import "../"
import Atlas 1.0 as Atlas

Atlas.ModelEditDialog {
    id: dialog
    width: 320
    height: 240
    title: "Model"
    color: syspal.window;

    fillWindow: true;

    Item{
        implicitWidth: layout.implicitWidth+10;
        implicitHeight: layout.implicitHeight+10;
        ColumnLayout{
            id: layout;
            anchors.fill: parent;

            RowLayout{
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
            }

            RowLayout{
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
                    //minimumValue: -1;
                    maximumValue: 360;
                    Layout.fillWidth: true;
                    suffix: "°";
                    onValueChanged:{
                        tool.model.rotationZ=value;
                    }
                }

            }

            RowLayout{
                Label{
                    text: "x"
                }

                SpinBox{
                    decimals: 1;
                    value: tool.model.scaleX;
                    Layout.fillWidth: true;
                    suffix: "x";

                    onValueChanged:{
                        tool.model.scaleX=value;
                    }
                }
                Label{
                    text: "y"
                }

                SpinBox{
                    //id: model_b;
                    decimals: 1;
                    value: tool.model.scaleY;
                    //maximumValue: 360;
                    Layout.fillWidth: true;
                    suffix: "x";

                    onValueChanged:{
                        tool.model.scaleY=value;
                    }
                }
                Label{
                    text: "z"
                }
                SpinBox{
                    //id: model_g;
                    decimals: 1;
                    value: tool.model.scaleZ;
                    //minimumValue: -1;
                    //maximumValue: 360;
                    Layout.fillWidth: true;
                    suffix: "x";
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
            RowLayout{
                Item{
                    Layout.fillWidth: true;
                }

                Button {
                    text: qsTr("Ok");
                }
                Item{
                    width: 100;
                }

                Button{
                    text: qsTr("Close");
                    onClicked: dialog.close();
                }
            }
        }
    }
}
