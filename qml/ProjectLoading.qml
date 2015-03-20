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
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.1
import Atlas 1.0 as Atlas

Atlas.ProjectLoader {
    id: root
    SystemPalette{
        id:syspal;
    }

    //color: syspal.window;
    implicitHeight: columnLayout1.implicitHeight;
    implicitWidth: columnLayout1.implicitWidth;

    ColumnLayout{
        id: columnLayout1
        anchors.fill: parent
        Item{
            Layout.fillHeight: true;
        }
        ProgressBar{
            id: load_progress;
            anchors.horizontalCenter: parent.horizontalCenter
            value: root.loadingProgress;
        }
        Label{
            id:load_text;
            Layout.fillWidth: true;
            horizontalAlignment: Text.AlignHCenter;
            text: root.loadingMessage;
        }
        Item{
            Layout.fillHeight: true;
        }
    }
}
