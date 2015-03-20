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
    function keyName(key_code)
    {
        switch(key_code)
        {
            case Qt.Key_Up:    return "Up";
            case Qt.Key_Down:  return "Down";
            case Qt.Key_Right: return "Right";
            case Qt.Key_Left:  return "Left";
            case Qt.Key_Plus:  return "Plus";
            case Qt.Key_Minus: return "Minus";
            case Qt.Key_Space: return "Space";
            case Qt.Key_PageDown:  return "PageDown";
            case Qt.Key_PageUp:    return "PageUp";
            case Qt.Key_0: return "0";
            case Qt.Key_1: return "1";
            case Qt.Key_2: return "2";
            case Qt.Key_3: return "3";
            case Qt.Key_4: return "4";
            case Qt.Key_5: return "5";
            case Qt.Key_6: return "6";
            case Qt.Key_7: return "7";
            case Qt.Key_8: return "8";
            case Qt.Key_9: return "9";
            case Qt.Key_A: return "A";
            case Qt.Key_B: return "B";
            case Qt.Key_C: return "C";
            case Qt.Key_D: return "D";
            case Qt.Key_E: return "E";
            case Qt.Key_F: return "F";
            case Qt.Key_G: return "G";
            case Qt.Key_H: return "H";
            case Qt.Key_I: return "I";
            case Qt.Key_J: return "J";
            case Qt.Key_K: return "K";
            case Qt.Key_L: return "L";
            case Qt.Key_M: return "M";
            case Qt.Key_N: return "N";
            case Qt.Key_O: return "O";
            case Qt.Key_P: return "P";
            case Qt.Key_Q: return "Q";
            case Qt.Key_R: return "R";
            case Qt.Key_S: return "S";
            case Qt.Key_T: return "T";
            case Qt.Key_U: return "U";
            case Qt.Key_V: return "V";
            case Qt.Key_W: return "W";
            case Qt.Key_X: return "X";
            case Qt.Key_Y: return "Y";
            case Qt.Key_Z: return "Z";
        }
        return null;
    }

    TabView{
        anchors.fill: parent;
        Tab{
            title: qsTr("Keyboard");
            ScrollView{
                anchors.fill: parent;
                anchors.margins: 4;
                GridLayout{
                    columns: 3;
                    Item{
                        Layout.fillWidth: true;
                    }
                    Column{
                        Label{
                            width: parent.width;
                            horizontalAlignment: Text.AlignHCenter;
                            text: qsTr("Forward");
                        }
                        Button{
                            text: keyName(options.keyForward);
                            checkable: true;
                            activeFocusOnPress: true;
                            onClicked: {
                                text=qsTr("press key...");
                                checked=true;
                            }
                            Keys.onPressed: {
                                var key=keyName(event.key);

                                if(key)
                                {
                                    text=key;
                                    checked=false;
                                    focus=false;
                                    options.keyForward=event.key;
                                }
                            }
                        }
                    }
                    Item{
                        Layout.fillWidth: true;
                    }
                    Column{
                        Label{
                            width: parent.width;
                            horizontalAlignment: Text.AlignHCenter;
                            text: qsTr("Left");
                        }
                        Button{
                            text: keyName(options.keyLeft);
                            checkable: true;
                            activeFocusOnPress: true;
                            onClicked: {
                                text=qsTr("press key...");
                                checked=true;
                            }
                            Keys.onPressed: {
                                var key=keyName(event.key);

                                if(key)
                                {
                                    text=key;
                                    checked=false;
                                    focus=false;
                                    options.keyLeft=event.key;
                                }
                            }
                        }
                    }
                    Column{
                        Label{
                            width: parent.width;
                            horizontalAlignment: Text.AlignHCenter;
                            text: qsTr("Jump");
                        }
                        Button{
                            text: keyName(options.keyJump);
                            checkable: true;
                            activeFocusOnPress: true;
                            onClicked: {
                                text=qsTr("press key...");
                                checked=true;
                            }
                            Keys.onPressed: {
                                var key=keyName(event.key);

                                if(key)
                                {
                                    text=key;
                                    checked=false;
                                    focus=false;
                                    options.keyJump=event.key;
                                }
                            }
                        }
                    }
                    Column{
                        Label{
                            width: parent.width;
                            horizontalAlignment: Text.AlignHCenter;
                            text: qsTr("Right");
                        }
                        Button{
                            text: keyName(options.keyRight);
                            checkable: true;
                            activeFocusOnPress: true;
                            onClicked: {
                                text=qsTr("press key...");
                                checked=true;
                            }
                            Keys.onPressed: {
                                var key=keyName(event.key);

                                if(key)
                                {
                                    text=key;
                                    checked=false;
                                    focus=false;
                                    options.keyRight=event.key;
                                }
                            }
                        }
                    }
                    Item{
                        Layout.fillWidth: true;

                    }
                    Column{
                        Label{
                            width: parent.width;
                            horizontalAlignment: Text.AlignHCenter;
                            text: qsTr("Backward");
                        }
                        Button{
                            text: keyName(options.keyBackward);
                            checkable: true;
                            activeFocusOnPress: true;
                            onClicked: {
                                text=qsTr("press key...");
                                checked=true;
                            }
                            Keys.onPressed: {
                                var key=keyName(event.key);

                                if(key)
                                {
                                    text=key;
                                    checked=false;
                                    focus=false;
                                    options.keyBackward=event.key;
                                }
                            }
                        }
                    }
                    Item{Layout.fillWidth: true;}

                    //
                    Column{
                        Label{
                            width: parent.width
                            horizontalAlignment: Text.AlignHCenter;
                            text: qsTr("Up");
                        }
                        Button{
                            text: keyName(options.keyUp);
                            checkable: true;
                            activeFocusOnPress: true;
                            onClicked: {
                                text=qsTr("press key...");
                                checked=true;
                            }
                            Keys.onPressed: {
                                var key=keyName(event.key);

                                if(key)
                                {
                                    text=key;
                                    checked=false;
                                    focus=false;
                                    options.keyUp=event.key;
                                }
                            }
                        }
                    }
                    Item{ Layout.fillWidth: true;}
                    Column{
                        Label{
                            width: parent.width
                            horizontalAlignment: Text.AlignHCenter;
                            text: qsTr("Down");
                        }
                        Button{
                            text: keyName(options.keyDown);
                            checkable: true;
                            activeFocusOnPress: true;
                            onClicked: {
                                text=qsTr("press key...");
                                checked=true;
                            }
                            Keys.onPressed: {
                                var key=keyName(event.key);

                                if(key)
                                {
                                    text=key;
                                    checked=false;
                                    focus=false;
                                    options.keyDown=event.key;
                                }
                            }
                        }
                    }
                }
            }
        }

        Tab{
            title: qsTr("Mouse");
            GridLayout{
                anchors.fill: parent;
                anchors.margins: 4;
                columns:2;
                Label{
                    text: qsTr("Sensitivity");
                }
                Slider{
                    updateValueWhileDragging: false;
                    value: options.mouseSensitivity;
                }

                Label{
                    text: qsTr("Repeat frequency");
                }
                Slider{
                    minimumValue: 1;
                    maximumValue: 60;
                    stepSize: 1;
                    updateValueWhileDragging: false;
                    value: options.mouseFreq;
                }

                Label{
                    text: qsTr("Inverse");
                }
                CheckBox{
                    checked: options.mouseInverse;
                }
                Item {
                    Layout.columnSpan: 2;
                    Layout.fillHeight: true;
                }
            }
        }

        Tab{
            title: qsTr("Gamepad");

            GridLayout{
                anchors.fill: parent;
                anchors.margins: 4;
                columns: 2;
                Label{
                    text: qsTr("Control positin");
                }
                Column {
                    ExclusiveGroup { id: group }
                    RadioButton {
                        text: qsTr("Right tilt - Left move");
                        exclusiveGroup: group
                        checked: !options.gamepadSwap;
                        onCheckedChanged: {
                            if(checked) options.gamepadSwap=false;
                        }
                    }
                    RadioButton {
                        text: qsTr("Right move - Left tilt");
                        exclusiveGroup: group;
                        checked: options.gamepadSwap;
                        onCheckedChanged: {
                            if(checked) options.gamepadSwap=true;
                        }
                    }
                }

                Label{
                   text: qsTr("Sensitivity");
                }
                Slider{
                    minimumValue: -1;
                    maximumValue: 1;
                    value: options.gamepadSensitivity;
                    property bool first: true;
                    onValueChanged: {
                        if(first)
                            first=false;
                        else
                            options.gamepadSensitivity=value;
                    }

                }
                Label{
                    text: qsTr("Size");
                }
                Slider{
                    minimumValue: 10;
                    maximumValue: 100;
                    value: options.gamepadSize;
                    property bool first: true;
                    onValueChanged: {
                        if(first)
                            first=false;
                        else
                            options.gamepadSize=value;
                    }
                }

                Item{
                    Layout.fillHeight: true;
                    Layout.fillWidth: true;
                }

            }
        }
    }
}
