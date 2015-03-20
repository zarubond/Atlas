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

#include "lightdirectional.h"

LightDirectional::LightDirectional()
{
}

const ColorRGB &LightDirectional::getDiffuse() const
{
    return diffuse;
}

void LightDirectional::setDiffuse(const ColorRGB &color)
{
    this->diffuse=color;
}

void LightDirectional::setDirection(GLfloat x, GLfloat y, GLfloat z)
{
    direction.set(x,y,z);
    direction.normalize();
}

void LightDirectional::setDirection(const Vector3f &direction)
{
    this->direction=direction;
    this->direction.normalize();
}

const Vector3f &LightDirectional::getDirection() const
{
    return direction;
}

void LightDirectional::setSpecular(const ColorRGB &color)
{
    specular=color;
}

void LightDirectional::setSpecular(GLfloat r, GLfloat g, GLfloat b)
{
    specular.set(r,g,b);
}

const ColorRGB &LightDirectional::getSpecular() const
{
    return specular;
}

void LightDirectional::setShininess(float shininess)
{
    this->shininess=shininess;
}

float LightDirectional::getShininess() const
{
    return shininess;
}

Matrix4f LightDirectional::shadowMVP(float width, float height, float depth) const
{
    Vector3f dir=direction;
    dir.inverse();

    Matrix4f view;
    view.lookAt(dir,Vector3f(0,0,0),Vector3f(0,1,0));

    Vector4f vec[8];

    vec[0].set(0,0,0,1.0f);
    vec[1].set(width,0,0,1.0f);
    vec[2].set(0,height,0,1.0f);
    vec[3].set(0,0,depth,1.0f);

    vec[4].set(width,height,0,1.0f);
    vec[5].set(width,0,depth,1.0f);
    vec[6].set(0,height,depth,1.0f);
    vec[7].set(width,height,depth,1.0f);

    /*
    vec[0].set(current_camera.ntl[0],current_camera.ntl[1],current_camera.ntl[2],1.0f);
    vec[1].set(current_camera.ntr[0],current_camera.ntr[1],current_camera.ntr[2],1.0f);
    vec[2].set(current_camera.nbl[0],current_camera.nbl[1],current_camera.nbl[2],1.0f);
    vec[3].set(current_camera.nbr[0],current_camera.nbr[1],current_camera.nbr[2],1.0f);

    vec[4].set(current_camera.ftl[0],current_camera.ftl[1],current_camera.ftl[2],1.0f);
    vec[5].set(current_camera.ftr[0],current_camera.ftr[1],current_camera.ftr[2],1.0f);
    vec[6].set(current_camera.fbl[0],current_camera.fbl[1],current_camera.fbl[2],1.0f);
    vec[7].set(current_camera.fbr[0],current_camera.fbr[1],current_camera.fbr[2],1.0f);
*/

    Vector4f tmp=view*vec[0];
    Vector3f min(tmp[0],tmp[1],tmp[2]),max(tmp[0],tmp[1],tmp[2]);

    for(int i=1;i<8;i++)
    {
        tmp=view*vec[i];

        if (tmp[0] > max[0])
            max[0] = tmp[0];
        else if (tmp[0] < min[0])
            min[0] = tmp[0];

        if (tmp[1] > max[1])
            max[1] = tmp[1];
        else if (tmp[1] < min[1])
            min[1] = tmp[1];

        if (tmp[2] > max[2])
            max[2] = tmp[2];
        else if (tmp[2] < min[2])
            min[2] = tmp[2];
    }

    Matrix4f proj;
    proj.ortho(min[0],max[0],min[1],max[1], -max[2], -min[2]);

    proj.transpose();
    view.transpose();

    return view*proj;
}
