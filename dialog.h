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
#ifndef DIALOG_H
#define DIALOG_H

#include <QObject>
#include <QWindow>
#include <QQuickView>
#include <QEventLoop>
#include <QQuickItem>
#include "lib/tools/envi.h"
#include "lib/tools/shell.h"

//mostly taken from qt lib
class Dialog : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible NOTIFY visibilityChanged)
    Q_PROPERTY(Qt::WindowModality modality READ modality WRITE setModality NOTIFY modalityChanged)
    Q_PROPERTY(int minimumHeight READ minimumHeight WRITE setMinimumHeight NOTIFY minimumHeightChanged)
    Q_PROPERTY(int minimumWidth READ minimumWidth WRITE setMinimumWidth NOTIFY minimumWidthChanged)
    Q_PROPERTY(int x READ x WRITE setX NOTIFY geometryChanged)
    Q_PROPERTY(int y READ y WRITE setY NOTIFY geometryChanged)
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY geometryChanged)
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY geometryChanged)
    Q_PROPERTY(QQuickItem* contentItem READ contentItem WRITE setContentItem DESIGNABLE false)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(bool fillWindow READ fillWindow WRITE setFillWindow NOTIFY fillWindowChanged)
    Q_CLASSINFO("DefaultProperty", "contentItem")    // Dialog in QML can have only one child

public:
    explicit Dialog(QObject *parent = NULL);
    ~Dialog();

    void setTitle(const QString &title);
    QString title() const;

    bool isVisible() const;
    Qt::WindowModality modality() const;
    QQuickItem* contentItem() const;

    QQuickWindow *parentWindow();
    int x() const;
    int y() const;
    int width() const;
    int height() const;


    int minimumHeight() const;
    int minimumWidth() const;

    QColor color() const;

    bool fillWindow() const;

signals:

    void titleChanged(QString arg);
    void visibilityChanged();

    void modalityChanged(Qt::WindowModality arg);
    void geometryChanged();
    void rejected();
    void accepted();

    void minimumHeightChanged(int arg);
    void minimumWidthChanged(int arg);

    void colorChanged(QColor arg);
    /**
     * @brief fillWindowChanged This fills 90% of the window with your dialog. This works only if you the host platform does not supports windows.
     * @param arg
     */
    void fillWindowChanged(bool arg);

public slots:
    void open();
    void close();
    void setVisible(bool arg);
    void setModality(Qt::WindowModality arg);
    void setContentItem(QQuickItem *obj);

    void setX(int arg);
    void setY(int arg);
    void setWidth(int arg);
    void setHeight(int arg);

    virtual void accept();
    virtual void reject();
    virtual void opened();
    void setMinimumHeight(int arg);
    void setMinimumWidth(int arg);

    void setColor(QColor arg);

    void visibleChanged(bool v);
    void setFillWindow(bool arg);

private slots:
    void decorationLoaded();
    void windowGeometryChanged();
private:
    QQuickWindow * m_dialogWindow;
    QQuickWindow * m_parentWindow;
    bool m_hasNativeWindows;

    QString m_title;
    bool m_visible;
    Qt::WindowModality m_modality;
    QQuickItem* m_contentItem;
    static QQmlComponent * m_decorationComponent;
    QQuickItem *m_windowDecoration;
    QRect m_sizeAspiration;
    bool m_hasAspiredPosition;
    int m_x;
    int m_y;
    int m_width;
    int m_height;
    int m_minimumHeight;
    int m_minimumWidth;
    QColor m_color;
    bool m_fillWindow;
};

#endif // DIALOG_H
