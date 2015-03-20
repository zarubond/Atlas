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
#ifndef PROJECTDIALOG_H
#define PROJECTDIALOG_H

#include "../dialog.h"
#include "../project.h"

class ProjectDialog : public Dialog
{
    Q_OBJECT

    Q_PROPERTY(Project* project READ project WRITE setProject NOTIFY projectChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString author READ author WRITE setAuthor NOTIFY authorChanged)
    Q_PROPERTY(QString info READ info WRITE setInfo NOTIFY infoChanged)
    Q_PROPERTY(QUrl snapshot READ snapshot WRITE setSnapshot NOTIFY snapshotChanged)
public:
    explicit ProjectDialog(QObject *parent = NULL);

    Project *project() const;
    QString author() const;
    QString info() const;
    QUrl snapshot() const;
    QString name() const;

signals:

    void projectChanged(Project* arg);
    void authorChanged(QString arg);
    void infoChanged(QString arg);
    void snapshotChanged(QUrl arg);

    void nameChanged(QString arg);

private slots:
    void accept();
    void reject();
public slots:
    void setProject(Project* arg);
    void setAuthor(QString arg);
    void setInfo(QString arg);
    void setSnapshot(QUrl arg);
    void setName(QString arg);

private:
    void opened();

    Project* m_project;
    QString m_author;
    QString m_info;
    QUrl m_snapshot;
    QString m_name;
};

#endif // PROJECTDIALOG_H
