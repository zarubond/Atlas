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

#include "dialog.h"

Dialog::Dialog(QObject *parent) :
    QObject(parent)
    , m_dialogWindow(0)
    , m_parentWindow(0)
    , m_visible(false)
    , m_modality(Qt::WindowModal)
    , m_contentItem(0)
    , m_windowDecoration(0)
    , m_hasAspiredPosition(false)
{
    /*
     * nativeWindows(QGuiApplication::platformIntegration()->
                                             hasCapability(QPlatformIntegration::MultipleWindows) &&
                                             QGuiApplicationPrivate::platformIntegration()->
                                             hasCapability(QPlatformIntegration::WindowManagement))*/
#ifdef OS_ANDROID
    m_hasNativeWindows=false;
#else
    m_hasNativeWindows=true;
#endif
    m_visible=false;
    m_minimumHeight=0;
    m_minimumWidth=0;
    m_height=0;
    m_width=0;
    m_fillWindow=false;
}

Dialog::~Dialog()
{

}

void Dialog::setTitle(const QString &title)
{
    if(m_dialogWindow!=NULL)
        m_dialogWindow->setTitle(title);
    m_title=title;
}

QString Dialog::title() const
{
    return m_title;
}

void Dialog::open()
{
    setVisible(true);
}

void Dialog::setContentItem(QQuickItem *obj)
{
    m_contentItem = obj;
    if (m_dialogWindow) {
        disconnect(this, SLOT(visibleChanged(bool)));
        // Can't necessarily delete because m_dialogWindow might have been provided by the QML.
        m_dialogWindow = 0;
    }
}

void Dialog::setX(int arg)
{
    if (m_x != arg) {
        m_x = arg;

        m_hasAspiredPosition = true;
        m_sizeAspiration.moveLeft(arg);
        if (m_dialogWindow) {
            if (sender() != m_dialogWindow)
                m_dialogWindow->setX(arg);
        } else if (m_contentItem) {
            m_contentItem->setX(arg);
        }
        emit geometryChanged();
    }
}

void Dialog::setY(int arg)
{
    if (m_y != arg) {
        m_y = arg;

        m_hasAspiredPosition = true;
        m_sizeAspiration.moveTop(arg);
        if (m_dialogWindow) {
            if (sender() != m_dialogWindow)
                m_dialogWindow->setY(arg);
        } else if (m_contentItem) {
            m_contentItem->setY(arg);
        }
        emit geometryChanged();
    }
}

void Dialog::setWidth(int arg)
{
    if (m_width != arg) {
        m_width = arg;
        m_sizeAspiration.setWidth(arg);
        if (m_dialogWindow) {
            if (sender() != m_dialogWindow)
                m_dialogWindow->setWidth(arg);
        } else if (m_contentItem) {
            m_contentItem->setWidth(arg);
        }
        emit geometryChanged();
    }
}

void Dialog::setHeight(int arg)
{
    if (m_height != arg) {
        m_height = arg;

        m_sizeAspiration.setHeight(arg);
        if (m_dialogWindow) {
            if (sender() != m_dialogWindow)
                m_dialogWindow->setHeight(arg);
        } else if (m_contentItem) {
            m_contentItem->setHeight(arg);
        }
        emit geometryChanged();
    }
}

bool Dialog::isVisible() const
{
    return m_visible;
}

Qt::WindowModality Dialog::modality() const
{
    return m_modality;
}

QQuickItem *Dialog::contentItem() const
{
    return m_contentItem;
}

void Dialog::close()
{
    setVisible(false);
}

void Dialog::setVisible(bool arg)
{
    if (m_visible == arg) return;
    m_visible = arg;

    // Pure QML implementation: wrap the contentItem in a window, or fake it
    if (!m_dialogWindow && m_contentItem)
    {
        if (m_hasNativeWindows)
            m_dialogWindow = m_contentItem->window();

        // An Item-based dialog implementation doesn't come with a window, so
        // we have to instantiate one iff the platform allows it.
        if (!m_dialogWindow && m_hasNativeWindows)
        {
            QQuickWindow *win = new QQuickWindow;
            ((QObject *)win)->setParent(this); // memory management only
            m_dialogWindow = win;
            m_contentItem->setParentItem(win->contentItem());
            QSize minSize = QSize(m_contentItem->implicitWidth(), m_contentItem->implicitHeight());

            if (m_minimumHeight > minSize.height())
                minSize.setHeight(m_minimumHeight);

            if (m_minimumWidth > minSize.width())
                minSize.setWidth(m_minimumWidth);

            m_dialogWindow->setMinimumSize(minSize);
            m_dialogWindow->setColor(m_color);
            connect(win, SIGNAL(widthChanged(int)), this, SLOT(windowGeometryChanged()));
            connect(win, SIGNAL(heightChanged(int)), this, SLOT(windowGeometryChanged()));
        }

        QQuickItem *parentItem = qobject_cast<QQuickItem *>(parent());

        // If the platform does not support multiple windows, but the dialog is
        // implemented as an Item, then try to decorate it as a fake window and make it visible.
        if (parentItem && !m_dialogWindow && !m_windowDecoration)
        {
            if(m_decorationComponent==NULL)
            {
                QQmlEngine *engine = qmlEngine(this);
                m_decorationComponent =
                        new QQmlComponent( engine, QUrl(QStringLiteral("qrc:/qml/dialogDecoration.qml")), QQmlComponent::Asynchronous);
            }
            if (m_decorationComponent) {
                    if (m_decorationComponent->isLoading())
                        connect(m_decorationComponent, SIGNAL(statusChanged(QQmlComponent::Status)),
                                this, SLOT(decorationLoaded()));
                    else
                        decorationLoaded();
            }
            // Window decoration wasn't possible, so just reparent it into the scene
            else {
                m_contentItem->setParentItem(parentItem);
                m_contentItem->setZ(10000);
            }
        }
    }

    if (m_dialogWindow)
    {
        // "grow up" to the size and position expected to achieve
        if (!m_sizeAspiration.isNull())
        {
                if (m_hasAspiredPosition)
                    m_dialogWindow->setGeometry(m_sizeAspiration);
                else {
                    if (m_sizeAspiration.width() > 0)
                        m_dialogWindow->setWidth(m_sizeAspiration.width());
                    if (m_sizeAspiration.height() > 0)
                        m_dialogWindow->setHeight(m_sizeAspiration.height());
                }

                connect(m_dialogWindow, SIGNAL(visibleChanged(bool)), this, SLOT(setVisible(bool)));
                connect(m_dialogWindow, SIGNAL(xChanged(int)), this, SLOT(setX(int)));
                connect(m_dialogWindow, SIGNAL(yChanged(int)), this, SLOT(setY(int)));
                connect(m_dialogWindow, SIGNAL(widthChanged(int)), this, SLOT(setWidth(int)));
                connect(m_dialogWindow, SIGNAL(heightChanged(int)), this, SLOT(setHeight(int)));
        }
    }

    if (m_windowDecoration)
    {
        m_windowDecoration->setProperty("dismissOnOuterClick", (m_modality == Qt::NonModal));
        m_windowDecoration->setVisible(m_visible);
    }
    else if (m_dialogWindow)
    {
        if (m_visible)
        {
            m_dialogWindow->setTransientParent(parentWindow());
            m_dialogWindow->setTitle(title());
            m_dialogWindow->setModality(m_modality);
        }
        m_dialogWindow->setVisible(m_visible);
    }

    emit visibleChanged(m_visible);
    if(m_visible)
        opened();
}

void Dialog::visibleChanged(bool v)
{
    m_visible = v;
    emit visibilityChanged();
}

void Dialog::setFillWindow(bool arg)
{
    if (m_fillWindow == arg)
        return;

    m_fillWindow = arg;
    emit fillWindowChanged(arg);
}

void Dialog::decorationLoaded()
{
    bool ok = false;
    QQuickItem *parentItem = qobject_cast<QQuickItem *>(parent());
    while (parentItem->parentItem() && !parentItem->parentItem()->inherits("QQuickRootItem"))
        parentItem = parentItem->parentItem();
    if (m_decorationComponent->isError())
    {
        qWarning() << m_decorationComponent->errors();
    }
    else
    {
        QObject *decoration = m_decorationComponent->create();
        m_windowDecoration = qobject_cast<QQuickItem *>(decoration);
        if (m_windowDecoration)
        {
            m_windowDecoration->setParentItem(parentItem);
            m_windowDecoration->setProperty("backgroundColor",m_color);
            // Give the window decoration its content to manage
            QVariant contentVariant;
            contentVariant.setValue<QQuickItem*>(m_contentItem);
            m_windowDecoration->setProperty("content", contentVariant);
            connect(m_windowDecoration, SIGNAL(dismissed()), this, SLOT(reject()));
            ok = true;
        }
        else
        {
            qWarning() << m_decorationComponent->url() <<
                "cannot be used as a window decoration because it's not an Item";
            delete m_windowDecoration;
            delete m_decorationComponent;
            m_decorationComponent = 0;
        }
    }
    // Window decoration wasn't possible, so just reparent it into the scene
    if (!ok) {
        m_contentItem->setParentItem(parentItem);
        m_contentItem->setZ(10000);
    }
    if(m_fillWindow)
    {
        setWidth(parentItem->width()*0.9);
        setHeight(parentItem->height()*0.9);
    }
}

QQuickWindow *Dialog::parentWindow()
{
    QQuickItem *parentItem = qobject_cast<QQuickItem *>(parent());
    if (parentItem)
        m_parentWindow = parentItem->window();
    return m_parentWindow;
}

int Dialog::x() const
{
    return m_x;
}

int Dialog::y() const
{
    return m_y;
}

int Dialog::width() const
{
    return m_width;
}

int Dialog::height() const
{
    return m_height;
}

int Dialog::minimumHeight() const
{
    return m_minimumHeight;
}

int Dialog::minimumWidth() const
{
    return m_minimumWidth;
}

QColor Dialog::color() const
{
    return m_color;
}

bool Dialog::fillWindow() const
{
    return m_fillWindow;
}

void Dialog::accept()
{
    close();
    emit accepted();
}

void Dialog::reject()
{
    close();
    emit rejected();
}

void Dialog::opened()
{
}

void Dialog::setMinimumHeight(int arg)
{
    if (m_minimumHeight != arg) {
        m_minimumHeight = arg;
        if(m_dialogWindow!=NULL)
            m_dialogWindow->setMinimumHeight(m_minimumHeight);
        emit minimumHeightChanged(arg);
    }
}

void Dialog::setMinimumWidth(int arg)
{
    if (m_minimumWidth != arg) {
        m_minimumWidth = arg;
        if(m_dialogWindow!=NULL)
            m_dialogWindow->setMinimumHeight(m_minimumWidth);
        emit minimumWidthChanged(arg);
    }
}

void Dialog::setColor(QColor arg)
{
    if (m_color != arg) {
        m_color = arg;
        if(m_dialogWindow!=NULL)
            m_dialogWindow->setColor(m_color);
        else if(m_decorationComponent!=NULL)
            m_windowDecoration->setProperty("backgroundColor",m_color);

        emit colorChanged(arg);
    }
}

void Dialog::setModality(Qt::WindowModality arg)
{
    if (m_modality != arg) {
        m_modality = arg;
        emit modalityChanged(arg);
    }
}

void Dialog::windowGeometryChanged()
{
    if (m_dialogWindow && m_contentItem) {
        m_contentItem->setWidth(m_dialogWindow->width());
        m_contentItem->setHeight(m_dialogWindow->height());
    }
}


QQmlComponent * Dialog::m_decorationComponent=NULL;
