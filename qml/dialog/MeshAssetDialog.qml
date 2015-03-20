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
import Atlas 1.0

Dialog {
    id:root;
    width: layout.implicitWidth
    height: layout.implicitHeight

    property alias mesh_model: mesh_view.model;
    property alias index: mesh_view.currentIndex;

    ColumnLayout{
        id: layout;
        anchors.fill: parent;
        SplitView{
            Layout.fillHeight: true;
            Layout.fillWidth: true;

            ScrollView{
                Layout.fillWidth: true;
                Layout.fillHeight: true;
                Layout.minimumWidth: 250;
                Layout.minimumHeight: 250;
                GridView{
                    id: mesh_view;
                    delegate: Image {
                        asynchronous: true;
                        id: texture
                        source: preview
                        width: 98;
                        height: 98;
                        fillMode: Image.PreserveAspectFit;
                        MouseArea{
                            anchors.fill: parent;
                            onClicked: {
                                info_image.source=preview;
                                info_title.text=title;
                                info_licence.text=licence;
                                info_author.text=author;
                                mesh_view.currentIndex=index;
                            }
                        }
                    }
                }
            }
            GridLayout{
                Layout.minimumWidth: 200;
                Layout.fillHeight: true;
                columns:2

                Image{
                    id: info_image
                    Layout.fillWidth: true;
                    Layout.maximumHeight: 200;
                    Layout.columnSpan: 2;
                    fillMode: Image.PreserveAspectFit;
                }
                Label{
                    text: "Name:"
                }

                Label{
                    id: info_title;
                    Layout.fillWidth: true;
                }
                Label{
                    text: "Author";
                }

                Label{
                    id: info_author
                    Layout.fillWidth: true;
                }
                Label{
                    text: "Licence";
                }

                Label{
                    id: info_licence;
                    Layout.fillWidth: true;
                }
            }
        }

        RowLayout{
            Item{
                Layout.fillWidth: true;
            }

            Button{
                text: qsTr("Ok")
                onClicked:
                {
                    root.accept();
                }
            }

            Button
            {
                text: qsTr("Cancel");
                onClicked: root.reject();
            }
        }
    }
}
