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
import QtQuick 2.1
import QtQuick.Controls 1.1

ScrollView{
    id: tex_h
    frameVisible: true;
    visible: false;
    flickableItem.anchors.margins: 3;
    property alias model: rect.model;
    property alias currentItem: rect.currentItem;
    property alias currentIndex: rect.currentIndex;
    property alias itemHeight: rect.maxHeight;

    function open(select)
    {
        visible=true;
        rect.currentIndex=select;
    }

    function close()
    {
        visible=false;
        closed();
    }

    signal closed();

    GridView{
        id: rect;
        highlight: Rectangle {
            color: syspal.highlight;
        }
        cellWidth: tex_h.viewport.width/3-6;
        property real maxHeight: 0;

        delegate: Column {
            width: GridView.view.width/3;
            height: b.height+h.height;
            onHeightChanged: {
                if(rect.maxHeight<height+6)
                    rect.maxHeight=height+6;
            }

            property string texture_path: (typeof path != 'undefined')?path:"";
            Button{
                id: b;
                width: parent.width-6;
                height: parent.width-6;
                anchors.horizontalCenter: parent.horizontalCenter;
                onClicked: {
                    tex_h.close();
                    rect.currentIndex=index;
                }
                Image {
                    anchors.fill: parent;
                    anchors.margins: 5;
                    source: (typeof path != 'undefined')?path:"";
                    anchors.horizontalCenter: parent.horizontalCenter;
                }
            }
            Text {
                id: h;
                text: title;
                anchors.horizontalCenter: parent.horizontalCenter;
            }
        }
    }
}
