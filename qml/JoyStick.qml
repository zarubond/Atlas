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
import QtGraphicalEffects 1.0

Item {
    id:joyStick;
    property int offset:30;

    signal move(vector2d vec);
    signal pressed();
    signal released();

    Rectangle {
        id:totalArea;
        color:"gray";
        radius: parent.width/2;
        opacity: 0.5
        border.width: 1
        border.color: "black"
        width:parent.width;height:parent.height
    }

    Rectangle{
        id:stick
        width:totalArea.width/2; height: width
        radius: width/2
        x: totalArea.width/2 - radius;
        y: totalArea.height/2 - radius;
        color:"black"
        Behavior on x{
            NumberAnimation { duration: 50 }
        }

        Behavior on y{
            NumberAnimation { duration: 50 }
        }
    }
    /*
    onWidthChanged: {
        stick.x=totalArea.width/2 - stick.radius;
        stick.y=totalArea.height/2 - stick.radius;
    }

    onHeightChanged: {
        stick.x=totalArea.width/2 - stick.radius;
        stick.y=totalArea.height/2 - stick.radius;
    }*/

    MultiPointTouchArea {
        anchors.fill: parent
        enabled: true;
        touchPoints: [
            TouchPoint { id: point1 }
        ]

        onUpdated: {
            if(touchPoints.length===1)
            {
                var vec_x = touchPoints[0].x - (totalArea.x + totalArea.radius);
                var vec_y = touchPoints[0].y - (totalArea.y + totalArea.radius);

                var radius = Math.sqrt(vec_x*vec_x+vec_y*vec_y);
                var angle = Qt.vector2d(vec_x,vec_y);
                angle=angle.normalized();

                if(radius>totalArea.radius)
                {
                    joyStick.move(angle);
                    angle=angle.times(totalArea.radius);
                    stick.x=angle.x + stick.radius;
                    stick.y=angle.y + stick.radius;
                }
                else
                {
                    stick.x = touchPoints[0].x - stick.radius;
                    stick.y = touchPoints[0].y - stick.radius;
                    angle=angle.times(radius/totalArea.radius);

                    joyStick.move(angle);

                }
            }
        }
        onPressed: {
            if(touchPoints.length===1)
            {
                joyStick.pressed();
            }
        }

        onReleased: {
            if(touchPoints.length===1)
            {
                stick.x = totalArea.width /2 - stick.radius;
                stick.y = totalArea.height/2 - stick.radius;
                joyStick.move(0.0,0.0);
                joyStick.released();
            }
        }
    }
/*
    MouseArea{
        id:mouseArea
        anchors.fill: parent

        onPressed:
        {
            joyStick.pressed();
        }

        onReleased: {
            //snap to center
            stick.x = totalArea.width /2 - stick.radius;
            stick.y = totalArea.height/2 - stick.radius;
            joyStick.move(0.0,0.0);
            joyStick.released();
        }

        onPositionChanged: {

            var vec_x = mouseX - (totalArea.x + totalArea.radius);
            var vec_y = mouseY - (totalArea.y + totalArea.radius);

            var radius = Math.sqrt(vec_x*vec_x+vec_y*vec_y);
            var angle = Qt.vector2d(vec_x,vec_y);
            angle=angle.normalized();

            if(radius>totalArea.radius)
            {
                joyStick.move(angle);
                angle=angle.times(totalArea.radius);
                stick.x=angle.x + stick.radius;
                stick.y=angle.y + stick.radius;
            }
            else
            {
                stick.x = mouseX - stick.radius;
                stick.y = mouseY - stick.radius;
                angle=angle.times(radius/totalArea.radius);

                joyStick.move(angle);

            }
        }
    }*/
}
