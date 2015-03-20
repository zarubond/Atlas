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
#ifndef NEWPROJECTDIALOG_H
#define NEWPROJECTDIALOG_H

#include "../dialog.h"
#include <QDir>

class Window;

class NewProjectDialog : public Dialog
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString author READ author WRITE setAuthor NOTIFY authorChanged)
    Q_PROPERTY(QString path READ path NOTIFY pathChanged)
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(int depth READ depth WRITE setDepth NOTIFY depthChanged)
    Q_PROPERTY(int step READ step WRITE setStep NOTIFY stepChanged)
    Q_PROPERTY(Window* atlasWindow READ atlasWindow WRITE setAtlasWindow NOTIFY atlasWindowChanged)
public:
    explicit NewProjectDialog(QObject *parent = NULL);
    QString name() const;
    QString author() const;
    QString path() const;
    int width() const;
    int height() const;
    int depth() const;

    Window* atlasWindow() const;
    void opened();
    QUrl url() const;
    int step() const;

private slots:
    void accept();
    void reject();
signals:

    void nameChanged(QString arg);
    void authorChanged(QString arg);
    void pathChanged(QString arg);
    void widthChanged(int arg);
    void heightChanged(int arg);
    void depthChanged(int arg);
    void atlasWindowChanged(Window* arg);
    void urlChanged(QUrl arg);
    void stepChanged(int arg);

public slots:
    void setName(const QString &arg);
    void setAuthor(const QString &arg);
    void setWidth(int arg);
    void setHeight(int arg);
    void setDepth(int arg);
    void setAtlasWindow(Window* arg);
    void setUrl(const QUrl &arg);
    void setStep(int arg);

private:
    void setPath(const QString &arg);
    QString m_name;
    QString m_author;
    QString m_path;
    int m_width;
    int m_height;
    int m_depth;
    Window* m_atlasWindow;
    QUrl m_url;
    int m_step;
};

#endif // NEWPROJECTDIALOG_H
