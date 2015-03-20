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

OptionsPage {
    TabView{
        anchors.fill: parent;

        Tab{
            title: qsTr("Environment")
            id: environment;
            visible: false;
            anchors.fill: parent;
            anchors.margins: 4;

            ColumnLayout{
                anchors.fill: parent;
                GridLayout{
                    Layout.fillWidth: true;
                    columns: 2;
                    Label{
                        text: qsTr("Language");
                    }

                    ComboBox{
                        model: ListModel {
                            ListElement { text: "English";}
                            //ListElement { text: "Česky";}
                        }
                        currentIndex: options.language;
                        onCurrentIndexChanged: {
                            if(options.language!==currentIndex)
                            {
                                language_warn.visible=true;
                                changed=true;
                            }
                            options.language=currentIndex;
                        }
                    }
                    Label{
                        id: language_warn;
                        Layout.columnSpan: 2;
                        Layout.fillWidth: true;
                        visible: false;
                        color: "red";
                        wrapMode: Text.Wrap;
                        text: qsTr("The color will change after restarting of the program!");
                    }

                    Label{
                        text: qsTr("Rendering system");
                    }
                    ComboBox{
                        model: ListModel {
                            ListElement { text: "OpenGL 3.0";}
                            ListElement { text: "OpenGL ES 2.0";}
                            ListElement { text: "OpenGL ES 3.0";}
                        }
                        currentIndex: options.renderBackend;
                        onCurrentIndexChanged: {
                            if(options.renderBackend!==currentIndex)
                            {
                                render_warn.visible=true;
                                changed=true;
                            }
                            options.renderBackend=currentIndex;
                        }
                    }
                    Label{
                        id: render_warn;
                        Layout.columnSpan: 2;
                        Layout.fillWidth: true;
                        visible: false;
                        color: "red";
                        wrapMode: Text.Wrap;
                        text: qsTr("The rendering backend will change after restarting of the program!");
                    }

                    Label{
                        text: qsTr("Theme");
                    }
                    ComboBox{
                        model: ListModel{
                            ListElement{ text: "System Default";}
                           // ListElement{ text: "Dark";}
                        }
                        currentIndex: options.theme;
                        onCurrentIndexChanged: {
                            if(options.theme!==currentIndex)
                            {
                                theme_warn.visible=true;
                                changed=true;
                            }

                            options.theme=currentIndex;
                        }
                    }
                    Label{
                        id: theme_warn;
                        Layout.columnSpan: 2;
                        Layout.fillWidth: true;
                        visible: false;
                        color: "red";
                        wrapMode: Text.Wrap;
                        text: qsTr("The color will change after restarting of the program!");
                    }
                }
                Item{
                    Layout.fillHeight: true;
                }
            }
        }
    }
}
