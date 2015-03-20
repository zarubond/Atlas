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
#ifndef DIALOGENVIRONMENT_H
#define DIALOGENVIRONMENT_H

#include "../lib/environment/environment.h"
#include "../dialog.h"
#include "../lib/worldgraphics.h"
#include "../project.h"

/**
 * @brief The DialogSun class
 */
class EnvironmentDialog : public Dialog
{
    Q_OBJECT
    //sun
    Q_PROPERTY(Project* project READ project WRITE setProject NOTIFY projectChanged)
    Q_PROPERTY(QColor diffuse READ diffuse WRITE setDiffuse NOTIFY diffuseChanged)
    Q_PROPERTY(QColor specular READ specular WRITE setSpecular NOTIFY specularChanged)
    Q_PROPERTY(QColor ambient READ ambient WRITE setAmbient NOTIFY ambientChanged)
    Q_PROPERTY(float shininess READ shininess WRITE setShininess NOTIFY shininessChanged)
    Q_PROPERTY(float sunTheta READ sunTheta WRITE setSunTheta NOTIFY sunThetaChanged)
    Q_PROPERTY(float sunLambda READ sunLambda WRITE setSunLambda NOTIFY sunLambdaChanged)
    //fog
    Q_PROPERTY(QColor fogColor READ fogColor WRITE setFogColor NOTIFY fogColorChanged)
    Q_PROPERTY(float fogDensity READ fogDensity WRITE setFogDensity NOTIFY fogDensityChanged)
    Q_PROPERTY(bool fogEnable READ fogEnable WRITE setFogEnable NOTIFY fogEnableChanged)
    //ocean
    Q_PROPERTY(bool oceanEnable READ oceanEnable WRITE setOceanEnable NOTIFY oceanEnableChanged)
    Q_PROPERTY(float oceanHeight READ oceanHeight WRITE setOceanHeight NOTIFY oceanHeightChanged)
    Q_PROPERTY(float oceanMaxHeight READ oceanMaxHeight WRITE setOceanMaxHeight NOTIFY oceanMaxHeightChanged)
    Q_PROPERTY(QColor oceanColor READ oceanColor WRITE setOceanColor NOTIFY oceanColorChanged)
    Q_PROPERTY(float oceanOpacity READ oceanOpacity WRITE setOceanOpacity NOTIFY oceanOpacityChanged)
    
public:
    explicit EnvironmentDialog(QObject *parent = NULL);
    ~EnvironmentDialog();
    
    Project *project() const;
    QColor diffuse() const;
    QColor specular() const;
    QColor ambient() const;
    float shininess() const;
    float sunTheta() const;
    float sunLambda() const;

    QColor fogColor() const;
    float fogDensity() const;
    bool fogEnable() const;

    bool oceanEnable() const;
    float oceanHeight() const;
    float oceanMaxHeight() const;
    QColor oceanColor() const;

    float oceanOpacity() const;

public slots:
    void setProject(Project* arg);
    void setDiffuse(QColor arg);
    void setSpecular(QColor arg);
    void setAmbient(QColor arg);
    void setShininess(float arg);
    void setSunTheta(float arg);
    void setSunLambda(float arg);

    void setFogColor(QColor arg);
    void setFogDensity(float arg);
    void setFogEnable(bool arg);

    void setOceanEnable(bool arg);
    void setOceanHeight(float arg);
    void setOceanMaxHeight(float arg);
    void setOceanColor(QColor arg);

    void setOceanOpacity(float arg);

signals:
    void projectChanged(Project* arg);
    void diffuseChanged(QColor arg);
    void specularChanged(QColor arg);
    void ambientChanged(QColor arg);
    void shininessChanged(float arg);
    void sunThetaChanged(float arg);
    void sunLambdaChanged(float arg);

    void fogColorChanged(QColor arg);
    void fogDensityChanged(float arg);

    void oceanEnableChanged(float arg);
    void oceanHeightChanged(float arg);
    void oceanMaxHeightChanged(float arg);
    void oceanColorChanged(QColor arg);

    void oceanOpacityChanged(float arg);

    void fogEnableChanged(bool arg);

private slots:
    void accept();
    void reject();
    void opened();

private:
    void setupSun();
    void setupWind();
    void setupFog();
    void saveFog();
    void setupOcean();
    void saveOcean();

    Environment * envi;

    //wind
    //TO BE

    Project* m_project;
    //sun
    QColor m_diffuse;
    QColor m_specular;
    QColor m_ambient;
    float m_shininess;
    //fog
    QColor m_fogColor;
    float m_fogDensity;
    //ocean
    bool m_oceanEnable;
    float m_oceanHeigth;
    float m_oceanMaxHeight;
    QColor m_oceanColor;
    float m_oceanOpacity;
    float m_sunTheta;
    float m_sunLambda;
    bool m_fogEnable;
};

#endif // DIALOGENVIRONMENT_H
