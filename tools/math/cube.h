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
#ifndef CUBE_H
#define CUBE_H

#include "vertex3.h"
#include "math.h"
#include "../opengl.h"
/**
 * @brief The Cube class
 */
class Cube
{
public:
    Cube();
    /**
     * @brief set Set cube geometry.
     * @param pos Lower left cube corner position.
     * @param size Cube edge size;
     */
    void set(Vertex3<float> pos, int size);
    /**
     * @brief collision Ray-cube collision.
     * @param a Ray start position.
     * @param b Ray end position.
     * @return True on collision.
     */
    bool collision(const Vertex3<float> & a,const Vertex3<float> &b);
    /**
     * @brief cubeVertices Create cube vertices. Scale 1.
     * @param array Array of drawable vertices.
     * @param count Number of vertices.
     */
    static void cubeVertices(GLfloat *& array,GLint & count);
    /**
     * @brief verticesLines Create cube vertices for GL_LINES.
     * @param array Array of drawable vertices.
     * @param count Number of vertices.
     */
    static void verticesLines(GLfloat *& array,GLint & count);
    /**
     * @brief cubeElements Cube elements for cubeVertices.
     * @param array Array of vertices.
     * @param count Number of vertices.
     */
    static void cubeElements(GLushort *& array,GLint & count);
private:
    Vertex3<float> corner[2];
};

#endif // CUBE_H
