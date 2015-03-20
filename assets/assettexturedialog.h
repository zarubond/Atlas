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
#ifndef TEXTUREDIALOG_H
#define TEXTUREDIALOG_H

#include <QStandardItemModel>
#include <QQuickItem>

#include "../dialog.h"
#include "assets.h"
#include "../project.h"

/**
 * @brief The TextureDialog class
 */
class AssetTextureDialog : public Dialog
{
    Q_OBJECT
    Q_PROPERTY(QStandardItemModel* model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(QUrl materialDiffuse READ materialDiffuse WRITE setMaterialDiffuse NOTIFY materialDiffuseChanged)
    Q_PROPERTY(QUrl materialNormal READ materialNormal WRITE setMaterialNormal NOTIFY materialNormalChanged)
    Q_PROPERTY(QUrl materialSpecular READ materialSpecular WRITE setMaterialSpecular NOTIFY materialSpecularChanged)
    Q_PROPERTY(QString materialName READ materialName WRITE setMaterialName NOTIFY materialNameChanged)
    Q_PROPERTY(Project* project READ project WRITE setProject NOTIFY projectChanged)
    
public:
    explicit AssetTextureDialog(QObject *parent = 0);
    ~AssetTextureDialog();

    QStandardItemModel* model() const;

    Project* project() const;

    QUrl materialDiffuse() const;
    QUrl materialNormal() const;
    QUrl materialSpecular() const;
    QString materialName() const;

public slots:
    void setModel(QStandardItemModel* arg);
    void setProject(Project* arg);

    void setMaterialDiffuse(QUrl arg);
    void setMaterialNormal(QUrl arg);
    void setMaterialSpecular(QUrl arg);
    void setMaterialName(QString arg);

signals:
    void modelChanged();
    void projectChanged(Project* arg);

    void materialDiffuseChanged(QUrl arg);
    void materialNormalChanged(QUrl arg);
    void materialSpecularChanged(QUrl arg);
    void materialNameChanged(QString arg);

private:
    void opened();
    enum ModelRoles {
        TitleRole = Qt::UserRole + 1,
        SpecularRole,
        DiffuseRole,
        NormalRole,
        LicenceRole,
        AuthorRole
    };

    void readTexture(QXmlStreamReader& xml);

    QDir dir;

    QStandardItem * selected;
    QStandardItemModel* m_model;
    Assets* assets;
    Project* m_project;
    QUrl m_materialDiffuse;
    QUrl m_materialNormal;
    QUrl m_materialSpecular;
    QString m_materialName;
};

#endif // TEXTUREDIALOG_H
