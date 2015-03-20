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
#ifndef WELCOME_H
#define WELCOME_H

#include <QObject>
#include <QStringList>
#include  <QStandardItemModel>
#include <QQuickItem>

class Window;
class Welcome : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QStandardItemModel* projectModel READ projectModel WRITE setProjectModel NOTIFY projectModelChanged)
    
public:
    explicit Welcome(QQuickItem *parent = 0);
    ~Welcome();
    /**
     * @brief load Load welcom screen.
     * @param atlas
     */
    void load(Window * atlas);
    QStandardItemModel* projectModel() const;

public slots:
    void setProjectModel(QStandardItemModel* arg);
    void openProject(int index);
signals:
    void projectModelChanged();

private slots:

    void reloadRecentProjects();
private:

    enum ModelRoles {
        TitleRole = Qt::UserRole + 1,
        PathRole
    };

    Window * atlas;
    QStandardItemModel* m_projectModel;
};

#endif // WELCOME_H
