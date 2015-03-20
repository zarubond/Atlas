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

Atlas.Dialog {
    id: root;
    width: layout.implicitWidth+8;
    height: layout.implicitHeight+8;

    property alias model: list.model;
    property string supported_formats;
    property int max_materials:0;

    signal accepted();
    signal rejected();

    signal replaceTexture(int index,string path);
    signal importAsset(int index);

    FileDialog{
        id: texture_dialog;
        title: qsTr("Please select teture");
        property int index;
        onAccepted: {
            root.replaceTexture(index,fileUrl);
        }
        nameFilters:  "Image files ("+root.supported_formats+")";
    }

    contentItem: ColumnLayout{
        id:layout
        anchors.fill: parent;

        ScrollView{
            Layout.fillHeight: true;
            Layout.fillWidth: true;
            implicitWidth: 400;
            ListView{
                id: list
                Component {
                    id: listDelegate
                    Rectangle {
                        id: wrapper
                        width: delegateLayout.implicitWidth;
                        height: delegateLayout.implicitHeight;
                        color: "white";
                        RowLayout
                        {
                            id: delegateLayout
                            anchors.fill: parent;
                            Label{
                                text: title
                            }
                            Button{
                                Layout.minimumHeight: 100;
                                Layout.minimumWidth: 100;
                                onClicked: {
                                    texture_dialog.index=index;
                                    texture_dialog.open();
                                }

                                Image{
                                    anchors.fill: parent;
                                    anchors.margins: 4;

                                    source: texture;
                                }
                            }
                            Button{
                                text: qsTr("Import Asset");
                                onClicked: {
                                    root.importAsset(index);
                                }
                            }
                        }
                    }
                }
                delegate: listDelegate;
            }
        }

        RowLayout{
            Layout.fillWidth: true;
            Item{
                Layout.fillWidth: true;
            }
            Button{
                text: "Ok";
                onClicked: root.accepted();
            }
            Button{
                text: "Cancel";
                onClicked: root.rejected();
            }
        }
    }
}
