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
#ifndef TERRAINIMPORTDIALOG_H
#define TERRAINIMPORTDIALOG_H

#include <QStandardItemModel>

#include "../../../project.h"
#include "../../../lib/module/terrain/terrainio.h"
#include "../../../dialog.h"
#include "../../imageprovider.h"

class TerrainControl;

class TerrainImportDialog : public Dialog
{
    Q_OBJECT
    Q_PROPERTY(QStandardItemModel* model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(Project* project READ project WRITE setProject NOTIFY projectChanged)
    Q_PROPERTY(bool working READ working NOTIFY workingChanged)
    Q_PROPERTY(float minHeight READ minHeight WRITE setMinHeight NOTIFY minHeightChanged)
    Q_PROPERTY(float maxHeight READ maxHeight WRITE setMaxHeight NOTIFY maxHeightChanged)
    Q_PROPERTY(float heightLimit READ heightLimit NOTIFY heightLimitChanged)
    Q_PROPERTY(ImageProvider* imageProvider READ imageProvider WRITE setImageProvider NOTIFY imageProviderChanged)
    Q_PROPERTY(int verticalTexture READ verticalTexture WRITE setVerticalTexture NOTIFY verticalTextureChanged)
    Q_PROPERTY(int horizontalTexture READ horizontalTexture WRITE setHorizontalTexture NOTIFY horizontalTextureChanged)
    Q_PROPERTY(QString supportedFormats READ supportedFormats NOTIFY supportedFormatsChanged)
    
public:
    explicit TerrainImportDialog(QObject *parent = 0);
    ~TerrainImportDialog();

    QStandardItemModel* model() const;
    Project* project() const;
    bool working() const;
    float minHeight() const;
    float maxHeight() const;
    ImageProvider* imageProvider() const;
    int verticalTexture() const;
    int horizontalTexture() const;
    QString supportedFormats() const;

    float heightLimit() const;

public slots:
    void importImage(const QUrl & path);
    void setModel(QStandardItemModel* arg);
    void setProject(Project* arg);
    void setMinHeight(float arg);
    void setMaxHeight(float arg);
    void setImageProvider(ImageProvider* arg);
    void setVerticalTexture(int arg);
    void setHorizontalTexture(int arg);

    void setHeightLimit(float arg);

signals:
    void reloadImage();
    void guiEnable(QVariant);
    void modelChanged();

    void projectChanged(Project* arg);
    void workingChanged(bool arg);
    void minHeightChanged(float arg);
    void maxHeightChanged(float arg);
    void imageProviderChanged(ImageProvider* arg);
    void verticalTextureChanged(int arg);
    void horizontalTextureChanged(int arg);
    void supportedFormatsChanged();

    void heightLimitChanged(float arg);

private:
    void accept();
    void opened();

    enum ModelRoles {
        TextRole = Qt::UserRole + 1,
        MaterialIdRole
    };


    TerrainGraphics * terrain;
    QImage image;

    QStandardItemModel* m_model;
    Project* m_project;
    bool m_working;
    float m_minHeight;
    float m_maxHeight;
    ImageProvider* m_imageProvider;
    int m_verticalTexture;
    int m_horizontalTexture;
    QString m_supportedFormats;
    float m_heightLimit;
};

#endif // TERRAINIMPORTDIALOG_H
