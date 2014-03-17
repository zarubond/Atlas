/**
 *  Atlas - Volumetric terrain editor
 *  Copyright (C) 2012-2013  Ondřej Záruba
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
#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include "tools/opengl.h"
#include "tools/math/math.h"
#include "tools/shader.h"
#include "tools/camera.h"
#include "world/map.h"
#include "shape.h"

#define DTOR 0.01745329252

/**
 * @brief The Navigator class
 */
class Navigator: public Shape
{
public:

    Navigator();
    bool init();

    void render(const Matrix4f & mvp);
    void setDistance(float distance);
    void lockX();
    void lockY();
    void lockZ();
    Mode getMode() const;
    void setMode(Mode mode);
    void show();
    void hide();
    bool isVisible();

    void updatePosition(Camera *camera, Map *map);
    void setPosition(float x, float y, float z);
    void setPosition(const Vertex3f &pos);
    void setMouse(int x,int y);
private:
    void CreateSphere();

    Shader program;
    GLint uniform_mvp,uniform_pos,uniform_scale;
    GLint attribute_coord3d;
    GLuint vbo_vertices,vbo_cube;
    int triangle_num,cube_num;
    bool lock_x,lock_y,lock_z;
    float distance;
    bool visible;
    int mouse_x,mouse_y;

    Navigator::Mode mode;
};

#endif // NAVIGATOR_H
