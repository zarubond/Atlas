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

#include "environmentdialog.h"

EnvironmentDialog::EnvironmentDialog(QObject *parent) :
    Dialog(parent), m_project(NULL)
{
    m_project=NULL;
    m_sunLambda=0;
    m_sunTheta=0;
}

EnvironmentDialog::~EnvironmentDialog()
{
}

Project *EnvironmentDialog::project() const
{
    return m_project;
}

QColor EnvironmentDialog::diffuse() const
{
    return m_diffuse;
}

QColor EnvironmentDialog::specular() const
{
    return m_specular;
}

QColor EnvironmentDialog::ambient() const
{
    return m_ambient;
}

float EnvironmentDialog::shininess() const
{
    return m_shininess;
}

float EnvironmentDialog::sunTheta() const
{
    return m_sunTheta;
}

float EnvironmentDialog::sunLambda() const
{
    return m_sunLambda;
}

QColor EnvironmentDialog::fogColor() const
{
    return m_fogColor;
}

float EnvironmentDialog::fogDensity() const
{
    return m_fogDensity;
}

bool EnvironmentDialog::fogEnable() const
{
    return m_fogEnable;
}

bool EnvironmentDialog::oceanEnable() const
{
    return m_oceanEnable;
}

float EnvironmentDialog::oceanHeight() const
{
    return m_oceanHeigth;
}

float EnvironmentDialog::oceanMaxHeight() const
{
    return m_oceanMaxHeight;
}

QColor EnvironmentDialog::oceanColor() const
{
    return m_oceanColor;
}

float EnvironmentDialog::oceanOpacity() const
{
    return m_oceanOpacity;
}

void EnvironmentDialog::setProject(Project *arg)
{
    if (m_project != arg) {
        m_project = arg;
        envi=&m_project->world.environment;
        emit projectChanged(arg);
    }
}

void EnvironmentDialog::setDiffuse(QColor arg)
{
    if (m_diffuse != arg) {
        m_diffuse = arg;
        envi->sun.setDiffuse(ColorRGB(float(arg.red())/255.0,float(arg.green())/255.0,float(arg.blue())/255.0));
        envi->changed=true;
        emit diffuseChanged(arg);
    }
}

void EnvironmentDialog::setSpecular(QColor arg)
{
    if (m_specular != arg) {
        m_specular = arg;
        emit specularChanged(arg);
    }
}

void EnvironmentDialog::setAmbient(QColor arg)
{
    if (m_ambient != arg) {
        m_ambient = arg;
        emit ambientChanged(arg);
    }
}

void EnvironmentDialog::setShininess(float arg)
{
    if (m_shininess != arg) {
        m_shininess = arg;
        emit shininessChanged(arg);
    }
}

void EnvironmentDialog::setSunTheta(float arg)
{
    if (m_sunTheta == arg)
        return;

    m_sunTheta = arg;
    emit sunThetaChanged(arg);
}

void EnvironmentDialog::setSunLambda(float arg)
{
    if (m_sunLambda == arg)
        return;

    m_sunLambda = arg;
    emit sunLambdaChanged(arg);
}

void EnvironmentDialog::setFogColor(QColor arg)
{
    if (m_fogColor != arg) {
        m_fogColor = arg;
        emit fogColorChanged(arg);
    }
}

void EnvironmentDialog::setFogDensity(float arg)
{
    if (m_fogDensity != arg) {
        m_fogDensity = arg;
        emit fogDensityChanged(arg);
    }
}

void EnvironmentDialog::setFogEnable(bool arg)
{
    if (m_fogEnable == arg)
        return;

    m_fogEnable = arg;
    emit fogEnableChanged(arg);
}

void EnvironmentDialog::setOceanEnable(bool arg)
{
    if (m_oceanEnable != arg) {
        m_oceanEnable = arg;
        emit oceanEnableChanged(arg);
    }
}

void EnvironmentDialog::setOceanHeight(float arg)
{
    if (m_oceanHeigth != arg) {
        m_oceanHeigth = arg;
        emit oceanHeightChanged(arg);
    }
}

void EnvironmentDialog::setOceanMaxHeight(float arg)
{
    if (m_oceanMaxHeight != arg) {
        m_oceanMaxHeight = arg;
        emit oceanMaxHeightChanged(arg);
    }
}

void EnvironmentDialog::setOceanColor(QColor arg)
{
    if (m_oceanColor != arg) {
        m_oceanColor = arg;
        emit oceanColorChanged(arg);
    }
}

void EnvironmentDialog::setOceanOpacity(float arg)
{
    if (m_oceanOpacity != arg) {
        m_oceanOpacity = arg;
        emit oceanOpacityChanged(arg);
    }
}

void EnvironmentDialog::accept()
{
    ColorRGB color;
    //save sun
    color.set(float(m_diffuse.red())/255.0,float(m_diffuse.green())/255.0,float(m_diffuse.blue())/255.0);
    envi->sun.setDiffuse(color);

    color.set(float(m_specular.red())/255.0,float(m_specular.green())/255.0,float(m_specular.blue())/255.0);
    envi->sun.setSpecular(color);

    color.set(float(m_ambient.red())/255.0,float(m_ambient.green())/255.0,float(m_ambient.blue())/255.0);
    envi->ambient.setColor(color);

    Vector3f dir;
    dir[0]=sin(m_sunTheta)*cos(m_sunLambda);
    dir[2]=sin(m_sunTheta)*sin(m_sunLambda);
    dir[1]=cos(m_sunTheta);
    envi->sun.setDirection(dir);

    //save fog
    saveFog();

    //save ocean
    saveOcean();

    ///
    envi->changed=true;

    close();
}

void EnvironmentDialog::reject()
{
    /* RESET??
    //sun
    envi->sun.setDiffuse(diffuse[0],diffuse[1],diffuse[2]);
    envi->sun.setSpecular(specular[0],specular[1],specular[2]);
    envi->sun.setShininess(shininess);
    envi->sun.setDirection(sun_direction);
    envi->setAmbient(ambient[0],ambient[1],ambient[2]);
    //fog
    envi->fog_color=fog_color;
    envi->fog_density=fog_density;
    //ocean
    envi->ocean_enabled=ocean_enabled;
    envi->ocean_height=ocean_height;
    world->ocean.setSeaLevel(ocean_height);
    world->ocean.enableOcean(ocean_enabled);
*/


    close();
}

void EnvironmentDialog::opened()
{
    ColorRGB color;
    setupSun();
    setupFog();
    setupOcean();
}

void EnvironmentDialog::setupSun()
{
    ColorRGB color;
     //sun
    color=envi->ambient.getColor();
    setAmbient(QColor(color[0]*255.0, color[1]*255.0, color[2]*255.0));
    color=envi->sun.getDiffuse();
    setDiffuse(QColor(color[0]*255.0, color[1]*255.0, color[2]*255.0));
    color=envi->sun.getSpecular();
    setSpecular(QColor(color[0]*255.0, color[1]*255.0, color[2]*255.0));
    float s=envi->sun.getShininess();
    setShininess(s);

    Vector3f sun_direction=envi->sun.getDirection();

    float lambda=atan(sun_direction[0]/sun_direction[2]);
    float theta=acos(sun_direction[1]/sqrt(sun_direction[0]*sun_direction[0]+sun_direction[1]*sun_direction[1]+
            sun_direction[2]*sun_direction[2]));

    setSunLambda(lambda);
    setSunTheta(theta);
}


void EnvironmentDialog::setupWind()
{

}


void EnvironmentDialog::setupFog()
{
    ColorRGB color;
    color=envi->fog.getColor();
    setFogColor(QColor(color[0]*255.0, color[1]*255.0, color[2]*255.0,255));
    setFogDensity(envi->fog.getDensity());
    setFogEnable(envi->fog.isEnabled());
}

void EnvironmentDialog::saveFog()
{
    envi->fog.setColor(ColorRGB(float(m_fogColor.red())/255.0,float(m_fogColor.green())/255.0,float(m_fogColor.blue())/255.0));
    envi->fog.setDensity(m_fogDensity);
    envi->fog.setEnable(m_fogEnable);
}

void EnvironmentDialog::setupOcean()
{
    setOceanEnable(m_project->world.ocean.isEnabled());
    setOceanHeight(m_project->world.ocean.getSeaLevel());
    setOceanMaxHeight(m_project->world.map.height());
    QColor color;
    color.setRed(m_project->world.ocean.color()[0]*255.0);
    color.setGreen(m_project->world.ocean.color()[1]*255.0);
    color.setBlue(m_project->world.ocean.color()[2]*255.0);
    setOceanColor(color);
    setOceanOpacity(m_project->world.ocean.color()[3]);
}

void EnvironmentDialog::saveOcean()
{
    OceanGraphics * ocean=&m_project->world.ocean;
    ocean->setEnabled(m_oceanEnable);
    ocean->setSeaLevel(m_oceanHeigth);
    ocean->setColor(m_oceanColor.red()/255.0f,m_oceanColor.green()/255.0f,m_oceanColor.blue()/255.0f,m_oceanOpacity);
}
