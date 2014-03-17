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
#ifndef CAMERA_H
#define CAMERA_H

#include "math/math.h"
#include "opengl.h"
#include "frustum.h"
#include "moveable.h"
#include <iostream>


/*
 *			  Traditional                   OpenGL
 *					
 *			| 0  1  2  3  |             | 0  4  8  12 |
 *			| 4  5  6  7  |             | 1  5  9  13 |
 *			| 8  9  10 11 |             | 2  6  10 14 |
 *			| 12 13 14 15 |             | 3  7  11 15 |
 *
 */

/**
  * @brief A camera class.
  * This class controls a player's camera.
  * @author Ondrej Zaruba
  */

class Camera: public Frustum, public Moveable
{
public:
    /**
      * @brief Sets everything to NULL and loads identity matrices.
      */
    Camera();
	/**
	 * @brief Calculates camera's perspective
     * @param view_angle Specifies the field of view angle, in degrees, in the y direction.
	 * @param aspect Aspect ration of view. Usually screen width/height
	 * @param near Distance from near clipping plane. near > 0
	 * @param far Distance form far clipping plane. far > near
	 */
    void perspective(GLfloat view_angle, GLfloat width, GLfloat height, GLfloat near, GLfloat far);
	/**
	 * @return Camera view matrix.
     */
    const Matrix4f & getLook() const;

    /**
     * @brief setPitch Set pitch of the camera.
     * @param angle Angle of the pitch.
     */
    void setPitch(float angle);
    /**
     * @brief setYaw Set yaw of the camera.
     * @param angle Angle of the yaw
     */
    void setYaw(float angle);

    /**
     * @brief getPitch
     * @return Camera's pitch.
     */
    float getPitch() const;
    /**
     * @brief getYaw
     * @return Camera's yaw.
     */
    float getYaw() const;

    /**
      * @brief Getter for the x coordinate.
      * @return X coordiante.
      */
    float getX() const;
    /**
      * @brief Getter for the y coordinate.
      * @return Y coordiante.
      */
    float getY() const;
    /**
      * @brief Getter for the z coordinate.
      * @return Z coordiante.
      */
    float getZ() const;

    /**
      * @brief Getter for the Projection matrix.
      * @return Projection matrix.
      */
    const Matrix4f &getProjectionMatrix() const;
    /**
     * @brief height Screen height.
     */
    unsigned height() const;
    /**
     * @brief width Screen width.
     */
    unsigned width() const;
    /**
     * @brief unproject Unproject from screen to space.
     * @param x Screen x position.
     * @param y Screen y position.
     * @param depth Depth on given point.
     * @return Unprojected point.
     */
    Vertex3f unproject(int x, int y, GLfloat depth) const;

    Vertex3f getDirection() const;
    float getSightDistance() const;
    void setSightDistance(float value);

private:
    void onUpdate();
    void recal();

    Matrix4f mvp;

    Matrix4f projection;
    float yaw,pitch;
    float sight_distance;

    unsigned screen_height,screen_width;
};

#endif // CAMERA_H
