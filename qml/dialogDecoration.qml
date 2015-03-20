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

Rectangle {
    color: "#80000000"
    anchors.fill: parent
    z: 1000000
    property alias content: borderImage.content
    property alias backgroundColor: borderImage.color;
    property bool dismissOnOuterClick: true
    signal dismissed
    MouseArea {
        anchors.fill: parent
        onClicked: if (dismissOnOuterClick) dismissed()

        Rectangle {
            id: borderImage;
            property Item content: null;
            color: "white";

            MouseArea { anchors.fill: parent }

            width: content ? content.width + 10 : 0
            height: content ? content.height + 10 : 0
            onWidthChanged: content.x = 5
            onHeightChanged: content.y = 5
            border.width: 2;
            border.color: "black";
            radius: 5;
            clip: true
            anchors.centerIn: parent
            onContentChanged: if (content) content.parent = borderImage
        }
    }
}
