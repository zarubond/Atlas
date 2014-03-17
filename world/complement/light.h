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
#ifndef LIGHT_H
#define LIGHT_H

#include "../../tools/math/math.h"
#include "../../tools/opengl.h"

struct LightUniform
{
    GLint position;
    GLint direction;
    GLint radius;
    GLint attenuation;
    GLint exponent;
    GLint diffuse;
    GLint specular;
    GLint shininess;
    GLint type;
};

class Light
{
public:
    enum LightType{
        DIRECTIONAL=0,
        POINT,
        SPOT
    };

    Light();
    /**
     * @brief Light position on map;
     * @param x
     * @param y
     * @param z
     */
    void setPosition(GLfloat x, GLfloat y,GLfloat z);
    /**
     * @brief Direction of light.
     * @param x
     * @param y
     * @param z
     */
    void setDirection(GLfloat x,GLfloat y,GLfloat z);
    void setDirection(const Vertex3f & direction);
    /**
     * @brief Diffuse light color.
     * @param r
     * @param g
     * @param b
     */
    void setDiffuse(GLfloat r,GLfloat g,GLfloat b);
    /**
     * @brief Specular light color.
     * @param r
     * @param g
     * @param b
     */
    void setSpecular(GLfloat r,GLfloat g,GLfloat b);

    /**
     * @brief setAttenuation
     * @param constant
     * @param linear
     * @param quadratic
     */
    void setAttenuation(GLfloat constant,GLfloat linear,GLfloat quadratic);
    /**
     * @brief Radius of spottlight.
     * @param radius
     */
    void setRadius(GLfloat radius);
    /**
     * @brief Range of light
     * @param range
     */
    void setExponent(GLfloat exponent);

    /**
     * @brief setShininess
     * @param shininess Specular exponent
     */
    void setShininess(GLfloat shininess);
    /**
     * @brief setType Sets light type.
     * @param type
     */
    void setType(LightType type);

    const ColorRGB & getPosition() const;
    const ColorRGB & getDirection() const;
    const ColorRGB & getDiffuse() const;
    const ColorRGB & getAttenuation() const;
    const ColorRGB & getSpecular() const;

    GLfloat getRadius()    const;
    GLfloat getExponent()  const;
    GLfloat getShininess() const;
    GLint getType() const;

private:
    Vertex3f position;
    Vertex3f direction;
    Vertex3f diffuse;
    Vertex3f specular;
    Vertex3f attenuation;
    float radius;
    float exponent;
    float shininess;
    LightType type;
    Matrix4f depth_bias;

    //to be removed
    /*
    void CreateSphere ();
    Shader program;
    GLint uniform_mvp,uniform_pos,uniform_scale,uniform_color;
    GLint attribute_coord3d;
    GLuint vbo_vertices;
    int triangle_num;*/

};

#endif // LIGHT_H
