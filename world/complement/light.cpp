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
#include "light.h"

Light::Light()
{
    radius=0;
    shininess=0;
}

void Light::setPosition(GLfloat x, GLfloat y, GLfloat z)
{
    position.set(x,y,z);
}

void Light::setDirection(GLfloat x, GLfloat y, GLfloat z)
{
    direction.set(x,y,z);
    direction.normalize();
}

void Light::setDirection(const Vertex3f &direction)
{
    this->direction=direction;
    this->direction.normalize();
}

void Light::setDiffuse(GLfloat r, GLfloat g, GLfloat b)
{
    diffuse.set(r,g,b);
}

void Light::setSpecular(GLfloat r, GLfloat g, GLfloat b)
{
    specular.set(r,g,b);
}

void Light::setAttenuation(GLfloat constant, GLfloat linear, GLfloat quadratic)
{
    attenuation.set(constant,linear,quadratic);
}

void Light::setRadius(GLfloat radius)
{
    this->radius=radius;
}

void Light::setExponent(GLfloat exponent)
{
    this->exponent=exponent;
}

void Light::setShininess(GLfloat shininess)
{
    this->shininess=shininess;
}

void Light::setType(Light::LightType type)
{
    this->type=type;
}

const ColorRGB &Light::getPosition() const
{
    return position;
}

const ColorRGB &Light::getDirection() const
{
    return direction;
}

const ColorRGB &Light::getDiffuse() const
{
    return diffuse;
}

const ColorRGB &Light::getAttenuation() const
{
    return attenuation;
}

const ColorRGB &Light::getSpecular() const
{
    return specular;
}

GLfloat Light::getRadius() const
{
    return radius;
}

GLfloat Light::getExponent() const
{
    return exponent;
}

GLfloat Light::getShininess() const
{
    return shininess;
}

GLint Light::getType() const
{
    return type;
}
