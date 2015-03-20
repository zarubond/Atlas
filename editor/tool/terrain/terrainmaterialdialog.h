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
#ifndef TERRAINTEXTUREDIALOG_H
#define TERRAINTEXTUREDIALOG_H

#include <QImageReader>
#include <QStandardItemModel>

#include "../../../project.h"
#include "../../../lib/module/terrain/terrainio.h"
#include "../../../assets/assettexturedialog.h"

class TerrainControl;

class TerrainMaterialDialog : public Dialog
{
    Q_OBJECT
    Q_PROPERTY(QStandardItemModel* model READ model NOTIFY modelChanged)
    Q_PROPERTY(QString supportedFormats READ supportedFormats NOTIFY supportedFormatsChanged)
    Q_PROPERTY(int maximumMaterials READ maximumMaterials NOTIFY maximumMaterialsChanged)
    Q_PROPERTY(Project* project READ project WRITE setProject NOTIFY projectChanged)
    Q_PROPERTY(TerrainControl* tool READ tool WRITE setTool NOTIFY toolChanged)
    
public:
    //TerrainTool * tool, TerrainGraphics *terrain, TerrainIO *terrain_io,Assets * assets,
    explicit TerrainMaterialDialog(QObject *parent = 0);
    ~TerrainMaterialDialog();

    QStandardItemModel* model() const;
    QString supportedFormats() const;
    int maximumMaterials() const;

    Project* project() const;

    TerrainControl* tool() const;

public slots:
    void setProject(Project* arg);
    void setTool(TerrainControl* arg);
    void setMaterial(int index, const QString &name, const QUrl &diffuse, const QUrl &normal, const QUrl &specular);
    void setMaterialName(int index, const QString &name);
    void addMaterial();

signals:
    void modelChanged();
    void supportedFormatsChanged(const QString & arg);
    void maximumMaterialsChanged(int arg);
    void projectChanged(Project* arg);

    void toolChanged(TerrainControl* arg);

private slots:
    void replaceTexture(int index, QUrl url);
    void replaceNormalMap(int index, QUrl url);
    void changeName(int index, const QString &name);

    void accept();
    void reject();

private:
    void setMaximumMaterials(int count);
    void opened();

    enum ModelRoles {
        TitleRole = Qt::UserRole + 1,
        TextureRole,
        NormalRole,
        MaterialIdRole,
        ChangedRole
    };

    TerrainGraphics * terrain;
    TerrainIO * terrain_io;
    Assets * assets;

    bool changed;

    QStandardItemModel* m_model;
    QString m_supportedFormats;
    int m_maximumMaterials;
    Project* m_project;
    TerrainControl* m_tool;
};

#endif // TERRAINTEXTUREDIALOG_H
