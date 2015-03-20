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
    width: 100
    height: 62
    TableView{
        anchors.fill: parent;
        model: ListModel{
            ListElement{ name: "Model"; about: ""}
            ListElement{ name: "Ocean"; about: ""}
            ListElement{ name: "Skybox"; about: ""}
            ListElement{ name: "Terrain"; about:""}
        }
        TableViewColumn{
            role: "name"
            title: qsTr("Name")
        }
        TableViewColumn{
            role: "about"
            title: qsTr("About");
        }
    }
}
