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

#ifndef EDITORGRAPHICS_H
#define EDITORGRAPHICS_H

#include "editorbase.h"
#include "../../graphics/graphics.h"

class EditorGraphics: public EditorBase
{
public:
    EditorGraphics();
    ~EditorGraphics();

    enum Axis{
        X,Y,Z,NONE
    };

    bool init();

    VertexBuffer vbo_position, vbo_scale, vbo_box;
    VertexArray vao_position, vao_scale, vao_box;

    VertexArray  vao_sphere, vao_cube;
    VertexBuffer vbo_cube, vbo_sphere;
    int sphere_num, cube_num;
    int position_num, scale_num, box_num;
    float * position_data, * scale_data;
    Vector3f axis_switch;
    /**
     * @brief collision
     * @param start start of ray
     * @param end end of ray, if the collision was detected then the position will be written into it
     * @return -1: no collistion 0:x axis 1: y axis 2: z axis
     */
    Axis collision(const Vector3<double> & start, Vector3<double> & end);
private:
    Axis positionCollision(const Vector3<double> &start, Vector3<double> &end);
    Axis scaleCollision(const Vector3<double> & start, Vector3<double> &end);

    void loadAxis();
    void loadBox();
    void loadPointer();    
    void loadScale();

};

#endif // EDITORGRAPHICS_H
