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
#ifndef TOOLPANEL_H
#define TOOLPANEL_H

#include "../shape.h"
#include "../../project.h"
#include "../../lib/worldio.h"
#include "../../canvas.h"
#include "../../assets/assets.h"

#include "../player.h"

#include <QQuickItem>
/**
 * @brief The EditorTool class is a tempelate for tools in editor.
 */

class ToolControl : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QQmlComponent* controlPanel READ controlPanel WRITE setControlPanel NOTIFY controlPanelChanged)

public:
    ToolControl(QQuickItem *parent = 0);
    /**
     * @brief setup Setup the editor tool.
     * @param space
     * @param navig
     * @param assets
     */
    virtual void setup(Instrument * navig)=0;

    /**
     * @brief uiSetup
     */
    virtual void uiSetup(QQuickItem * ui);
    /**
     * @brief uiTabSetup
     * @param tab
     */
    virtual void uiTabSetup(QQuickItem * tab);

    /**
     * @brief undo Undo one step back.
     */
    virtual void undo();
    /**
     * @brief redo Redo one step.
     */
    virtual void redo();
    /**
     * @brief mousePress Mouse has been pressed.
     * @param event
     */
    virtual void mousePress(QMouseEvent *event);
    /**
     * @brief mouseRelease Mouse has been released.
     * @param event
     */
    virtual void mouseRelease(QMouseEvent *event);
    /**
     * @brief mouseMove Mouse has mooved.
     * @param event
     */
    virtual void mouseMove(QMouseEvent * event);
    /**
     * @brief mouseDoubleClick
     * @param event
     */
    virtual void mouseDoubleClick(QMouseEvent * event);
    /**
     * @brief wheel Mouse wheel has turned.
     * @param event
     */
    virtual void wheel(QWheelEvent * event);
    /**
     * @brief keyPress Keyboard key has been pressed.
     * @param event
     */
    virtual void keyPress(QKeyEvent * event);
    /**
     * @brief keyRelease Keyboard key has been released.
     * @param event
     */
    virtual void keyRelease(QKeyEvent * event);
    /**
     * @brief onEnter Call when the tool is activated.
     */
    virtual void onEnter();
    /**
     * @brief onLeave Call when the tool is deactivated.
     */
    virtual void onLeave();
    /**
     * @brief loadProject Setup for the new project.
     * @param project
     */
    virtual void loadProject(Project * project);
    /**
     * @brief unloadProject Unload project.
     */
    virtual void unloadProject();
    /**
     * @brief update Ticks.
     * @param elapsed_time
     */
    virtual void update(int elapsed_time, const Player & player);

    QString title() const;

    QQmlComponent* controlPanel() const;

public slots:
    void setTitle(const QString &arg);

    void setControlPanel(QQmlComponent* arg);

signals:
    void titleChanged(QString arg);

    void controlPanelChanged(QQmlComponent* arg);

protected:

private:
    QString m_title;
    QQmlComponent* m_controlPanel;
};

#endif // TOOLPANEL_H
