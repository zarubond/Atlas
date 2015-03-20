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
#ifndef MESHDIALOG_H
#define MESHDIALOG_H

#include <QStandardItem>
#include <QStandardItemModel>

#include "assets.h"
#include "../dialog.h"
#include "../project.h"

/**
 * @brief The MeshDialog class
 */
class AssetMeshDialog : public Dialog
{
    Q_OBJECT
    Q_PROPERTY(Project* project READ project WRITE setProject NOTIFY projectChanged)
    Q_PROPERTY(QStandardItemModel* model READ model NOTIFY modelChanged)
    Q_PROPERTY(QUrl meshUrl READ meshUrl WRITE setMeshUrl NOTIFY meshUrlChanged)

public:
    explicit AssetMeshDialog(QObject *parent = 0);
    ~AssetMeshDialog();
    /**
     * @brief isSelected
     * @return If the mesh was selected
     */
    bool isSelected();
    /**
     * @brief file Path file of selected mesh.
     * @return
     */
    QString file();
    /**
     * @brief preview Path to image with preview of selected mesh.
     * @return
     */
    QString preview();
    /**
     * @brief name Name of the selected mesh.
     * @return
     */
    QString name();

    Project* project() const;

    QStandardItemModel* model() const;

    QUrl meshUrl() const;

public slots:
    void setProject(Project* arg);
    void setMeshUrl(QUrl arg);
signals:
    void projectChanged(Project* arg);
    void modelChanged();
    void meshUrlChanged(QUrl arg);

private:
    void readMesh(QXmlStreamReader &xml);
    void opened();
    enum ModelRoles {
        TitleRole = Qt::UserRole + 1,
        PreviewRole,
        UrlRole,
        LicenceRole,
        AuthorRole
    };

    Assets * assets;
    QDir dir;
    QStandardItem * selected;
    Project* m_project;
    QStandardItemModel* m_model;
    QUrl m_meshUrl;
};

#endif // MESHDIALOG_H
