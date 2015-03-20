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

Atlas.Dialog {
    id: root;
    title: qsTr("Help");
    color: syspal.window;

    Item{
        implicitWidth: layout.implicitWidth+10;
        implicitHeight: layout.implicitHeight+10;

        ColumnLayout{
            id: layout
            anchors.fill: parent;
            anchors.margins: 5;

            Label{
                text: "TO BE";
            }
            RowLayout{
                Item{
                Layout.fillWidth: true;
                }
                Button{
                    text: qsTr("Close");
                    onClicked: root.close();
                }
            }
        }
    }
}
