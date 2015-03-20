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
import "options"
import Atlas 1.0 as Atlas

Atlas.OptionsDialog {
    id: options;
    color: syspal.window;
    fillWindow: true;
    width: 800;
    height: 480;


    /*
    property var pagesModel: [
        { name: qsTr("Environment"),page: "options/pageEnvironment.qml" },
        { name: qsTr("Assets"),     page: "options/pageAssets.qml" },
        { name: qsTr("Tools"),      page: "options/pageTools.qml" }
    ]*/

    Item{
        implicitWidth: layout.implicitWidth+10;
        implicitHeight: layout.implicitHeight+10;


        SystemPalette{
            id: syspal;
        }

        ListModel {
            id: pagesModel;
            ListElement {
                name: QT_TR_NOOP("Environment");
                page: "options/pageEnvironment.qml";
                image:"";
            }
            ListElement {
                name: QT_TR_NOOP("Control");
                page: "options/pageControl.qml";
                image:"";
            }
            ListElement {
                name: QT_TR_NOOP("Assets");
                page: "options/pageAssets.qml";
                image: "";
            }
            ListElement {
                name: QT_TR_NOOP("Tools");
                page: "options/pageTools.qml";
                image: "";
            }
            ListElement {
                name: QT_TR_NOOP("Modules");
                page: "options/pageModules.qml";
                image: "";
            }
        }

        ColumnLayout{
            id: layout
            anchors.fill: parent;
            anchors.margins: 5;

            RowLayout{
                Layout.fillHeight: true;
                //Layout.fillWidth: true;
                //color: syspal.alternateBase;

                ScrollView {
                    id: item_scroll;
                    frameVisible: true;
                    Layout.fillHeight: true;
                    implicitWidth: item_list.maxWidth*1.2+viewport.parent.verticalScrollbarOffset;

                    ListView {
                            id: item_list;
                            focus: true;
                            model: pagesModel;
                            property real maxWidth: 0;
                            delegate: Item{
                                width: ListView.view.width;

                                Component.onCompleted: {
                                    if(item_list.maxWidth<label.implicitWidth)
                                        item_list.maxWidth=label.implicitWidth;
                                }

                                height: image.height+label.height;

                                Image{
                                    id: image;
                                    //to be some image
                                   // source: image;
                                    width: parent.width*0.3;
                                    height: parent.width*0.3;
                                }

                                Label{
                                    id: label;
                                    anchors.top: image.bottom;
                                    text: qsTr(name);
                                    width: parent.width;
                                    horizontalAlignment: Text.AlignHCenter;
                                    verticalAlignment: Text.AlignBottom;
                                }
                                MouseArea{
                                    anchors.fill: parent;
                                    onClicked: {
                                        parent.ListView.view.currentIndex=index;
                                        pages.open(pagesModel.get(index).page, pagesModel.get(index).name);
                                    }
                                }
                            }
                            highlight: Rectangle {
                                color: syspal.highlight;
                            }
                        }
                }

                ColumnLayout{
                    Layout.fillWidth: true;
                    Layout.fillHeight: true;
                    Label{
                        id: pageTitle;
                        text: pagesModel.get(0).name;
                        font.weight: Font.Bold;
                    }

                    Loader{
                        id: pages;
                        Layout.fillWidth: true;
                        Layout.fillHeight: true;
                        Layout.preferredHeight: item?item.height: 240;
                        Layout.preferredWidth: item?item.width: 320;
                        source: pagesModel.get(0).page;
                        onLoaded: {
                            item.load();
                            item.changed=false;
                        }
                        function save(){
                            if(item.changed)
                                item.save();
                            item.changed=false;
                        }
                        function open(path,title){
                            pages.source = path;
                            pageTitle.text = title;
                        }
                    }
                }
            }

            RowLayout{
                Layout.fillWidth: true;
                Item{
                    Layout.fillWidth: true;
                }

                Button{
                    text: qsTr("Ok");
                    onClicked: {
                        pages.save();
                        options.accept();
                    }
                }

                Button{
                    text: qsTr("Apply");
                    enabled: pages.item.changed;
                    onClicked: {
                        pages.save();
                        options.apply();
                    }
                }

                Button{
                    text: qsTr("Cancel");
                    onClicked: options.reject();
                }
            }
        }
    }
}
