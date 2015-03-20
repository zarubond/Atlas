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

Atlas.AssetTextureDialog {
    id: root;
    color: syspal.window;
    fillWindow: true;

    Item{
        implicitWidth: layout.implicitWidth+10;
        implicitHeight: layout.implicitHeight+10;

        ColumnLayout{
            id: layout
            anchors.fill: parent;
            anchors.margins: 5;

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
                        model: root.model;
                        cellWidth: 100;
                        cellHeight:100;

                        highlight: Rectangle {width: 100; height: 100;color: syspal.highlight; radius: 5 }

                        delegate: Item{
                            width: texture_view.cellWidth;
                            height: texture_view.cellHeight;
                            property var detail: model;
                            Image {
                                id: texture;
                                anchors.centerIn: parent;
                                asynchronous: true;
                                source: diffuse;
                                width: 90;
                                height: 90;
                                fillMode: Image.PreserveAspectFit;
                            }
                            MouseArea{
                                anchors.fill: parent;
                                onClicked: {
                                    texture_view.currentIndex=index;
                                }
                            }
                        }
                    }
                }
                GridLayout{
                    Layout.minimumWidth: 200;
                    Layout.fillHeight: true;
                    columns: 2
                    Image{
                        id: info_image;
                        Layout.columnSpan: 2
                        Layout.fillWidth: true;
                        Layout.maximumHeight: 200;
                        fillMode: Image.PreserveAspectFit;
                        source: texture_view.currentItem!=null?texture_view.currentItem.detail.diffuse:"";
                    }
                    Label{
                        text: qsTr("Name:");
                    }

                    Label{
                        id: info_title;
                        Layout.fillWidth: true;
                        text: texture_view.currentItem!=null?texture_view.currentItem.detail.title:"";
                    }
                    Label{
                        text: qsTr("Author:");
                    }

                    Label{
                        id: info_author
                        Layout.fillWidth: true;
                        text: texture_view.currentItem!=null?texture_view.currentItem.detail.author:"";
                    }
                    Label{
                        text: qsTr("Licence:");
                    }

                    Label{
                        id: info_licence;
                        Layout.fillWidth: true;
                        text: texture_view.currentItem?texture_view.currentItem.detail.licence:"";
                    }
                    Item{
                        Layout.columnSpan: 2;
                        Layout.fillHeight: true;
                    }
                }
            }

            RowLayout{
                Item{
                    Layout.fillWidth: true;
                }

                Button{
                    text: qsTr("Ok");
                    onClicked: {
                        root.materialDiffuse    = texture_view.currentItem.detail.diffuse;
                        if(texture_view.currentItem.detail.specular)root.materialSpecular = texture_view.currentItem.detail.specular;
                        if(texture_view.currentItem.detail.normal)  root.materialNormal   = texture_view.currentItem.detail.normal;
                        if(texture_view.currentItem.detail.title)   root.materialName     = texture_view.currentItem.detail.title;
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
}
