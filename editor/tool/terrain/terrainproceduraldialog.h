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
#ifndef DIALOGPROCEDURAL_H
#define DIALOGPROCEDURAL_H

#include "../../../project.h"
#include "../../../lib/module/terrain/terrainio.h"

#include "../../../lib/tools/perlinnoise.h"
#include "../../../lib/tools/diamondsquare.h"

#include "../../../dialog.h"

#include "../../imageprovider.h"

#include <QQuickImageProvider>
#include <QStandardItemModel>


class TerrainControl;


class TerrainProceduralDialog : public Dialog
{
    Q_OBJECT
    Q_PROPERTY(QStandardItemModel* model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(Project* project READ project WRITE setProject NOTIFY projectChanged)
    Q_PROPERTY(bool working READ working NOTIFY workingChanged)
    Q_PROPERTY(float minHeight READ minHeight WRITE setMinHeight NOTIFY minHeightChanged)
    Q_PROPERTY(float maxHeight READ maxHeight WRITE setMaxHeight NOTIFY maxHeightChanged)
    Q_PROPERTY(float heightLimit READ heightLimit NOTIFY heightLimitChanged)
    Q_PROPERTY(int mapWidth READ mapWidth WRITE setMapWidth NOTIFY mapWidthChanged)
    Q_PROPERTY(int mapDepth READ mapDepth WRITE setMapDepth NOTIFY mapDepthChanged)
    Q_PROPERTY(ImageProvider* imageProvider READ imageProvider WRITE setImageProvider NOTIFY imageProviderChanged)
    Q_PROPERTY(int verticalTexture READ verticalTexture WRITE setVerticalTexture NOTIFY verticalTextureChanged)
    Q_PROPERTY(int horizontalTexture READ horizontalTexture WRITE setHorizontalTexture NOTIFY horizontalTextureChanged)

public:
    explicit TerrainProceduralDialog(QObject*parent = 0);
    ~TerrainProceduralDialog();

    QStandardItemModel* model() const;

    bool working() const;
    void accept();

    float minHeight() const;
    float maxHeight() const;

    int mapWidth() const;
    int mapDepth() const;

    Project* project() const;
    ImageProvider* imageProvider() const;

    int verticalTexture() const;
    int horizontalTexture() const;

    float heightLimit() const;

public slots:
    void setModel(QStandardItemModel* arg);
    void makePerlinNoise(float seed,float frequency);
    void makeDiamondNoise(float seed,float frequency);
    void makePertrub(float frequency,float distance);
    void makeSmooth(float step);
    void makeErrode(float smoothness);

    void setMinHeight(float arg);
    void setMaxHeight(float arg);

    void setMapWidth(int arg);
    void setMapDepth(int arg);

    void setProject(Project* arg);
    void setImageProvider(ImageProvider* arg);

    void setVerticalTexture(int arg);
    void setHorizontalTexture(int arg);

    void setHeightLimit(float arg);

signals:
    void modelChanged();

    void workingChanged(bool arg);

    void minHeightChanged(float arg);
    void maxHeightChanged(float arg);

    void mapWidthChanged(int arg);
    void mapDepthChanged(int arg);

    void projectChanged(Project* arg);
    void imageProviderChanged(ImageProvider*arg);
    void verticalTextureChanged(int arg);
    void horizontalTextureChanged(int arg);

    void heightLimitChanged(float arg);

private:
    void setWorking(bool arg);
    void opened();
    void updateImage();
    void perturb(float f, float d);

    enum ModelRoles {
        TextRole = Qt::UserRole + 1,
        MaterialIdRole
    };

    TerrainGraphics * terrain;
    unsigned char texture_vertical,texture_horizontal;
    double * data;

    QStandardItemModel* m_model;
    bool m_working;
    float m_minHeight;
    float m_maxHeight;
    int m_mapWidth;
    int m_mapDepth;

    Project* m_project;
    ImageProvider* m_imageProvider;
    int m_verticalTexture;
    int m_horizontalTexture;
    float m_heightLimit;
};

#endif // DIALOGPROCEDURAL_H
