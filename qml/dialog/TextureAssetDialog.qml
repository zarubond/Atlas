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

Dialog{
    id:root;
    width: layout.implicitWidth
    height: layout.implicitHeight

    property alias texture_model: texture_view.model;

    signal accepted(int index);
    signal rejected();

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
                    id: texture_view;
                    delegate: Image {
                        asynchronous: true;
                        id: texture
                        source: src
                        width: 98;
                        height: 98;
                        fillMode: Image.PreserveAspectFit;
                        MouseArea{
                            anchors.fill: parent;
                            onClicked: {
                                info_image.source=src;
                                info_title.text=title;
                                info_licence.text=licence;
                                info_author.text=author;
                                texture_view.currentIndex=index;
                            }
                        }
                    }
                }
            }
            ColumnLayout{
                Layout.minimumWidth: 200;
                Layout.fillHeight: true;
                Image{
                    id: info_image
                    Layout.fillWidth: true;
                    Layout.maximumHeight: 200;
                    fillMode: Image.PreserveAspectFit;
                }
                Label{
                    id: info_title;
                    Layout.fillWidth: true;
                }
                Label{
                    id: info_author
                    Layout.fillWidth: true;
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
                text: "Ok"
                onClicked:
                {
                    root.accepted(texture_view.currentIndex);
                }
            }

            Button
            {
                text: "Cancel";
                onClicked: root.rejected();
            }
        }
    }
}
