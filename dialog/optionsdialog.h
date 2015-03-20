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
#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QSettings>

#include "../globalsettings.h"
#include "../editor/controlsettings.h"
#include "../dialog.h"
#include <QStandardItemModel>


//http://grip.espace-win.org/doc/apps/qt4/html/dialogs-configdialog.html
/**
 * @brief The ConfigDialog class
 * @todo This class is under construction.
 */
class OptionsDialog : public Dialog
{
    Q_OBJECT

    Q_PROPERTY(QStandardItemModel * keyboardShortcuts READ keyboardShortcuts WRITE setKeyboardShortcuts NOTIFY keyboardShortcutsChanged)
    Q_PROPERTY(QUrl assetsPath READ assetsPath WRITE setAssetsPath NOTIFY assetsPathChanged)
    Q_PROPERTY(int language READ language WRITE setLanguage NOTIFY languageChanged)
    Q_PROPERTY(int renderBackend READ renderBackend WRITE setRenderBackend NOTIFY renderBackendChanged)
    Q_PROPERTY(int theme READ theme WRITE setTheme NOTIFY themeChanged)

    Q_PROPERTY(int keyForward READ keyForward WRITE setKeyForward NOTIFY keyForwardChanged)
    Q_PROPERTY(int keyBackward READ keyBackward WRITE setKeyBackward NOTIFY keyBackwardChanged)
    Q_PROPERTY(int keyRight READ keyRight WRITE setKeyRight NOTIFY keyRightChanged)
    Q_PROPERTY(int keyLeft READ keyLeft WRITE setKeyLeft NOTIFY keyLeftChanged)
    Q_PROPERTY(int keyUp READ keyUp WRITE setKeyUp NOTIFY keyUpChanged)
    Q_PROPERTY(int keyDown READ keyDown WRITE setKeyDown NOTIFY keyDownChanged)
    Q_PROPERTY(int keyJump READ keyJump WRITE setKeyJump NOTIFY keyJumpChanged)

    Q_PROPERTY(bool mouseInverse READ mouseInverse WRITE setMouseInverse NOTIFY mouseInverseChanged)
    Q_PROPERTY(float mouseSensitivity READ mouseSensitivity WRITE setMouseSensitivity NOTIFY mouseSensitivityChanged)
    Q_PROPERTY(int mouseFreq READ mouseFreq WRITE setMouseFreq NOTIFY mouseFreqChanged)

    Q_PROPERTY(bool gamepadSwap READ gamepadSwap WRITE setGamepadSwap NOTIFY gamepadSwapChanged)
    Q_PROPERTY(float gamepadSensitivity READ gamepadSensitivity WRITE setGamepadSensitivity NOTIFY gamepadSensitivityChanged)
    Q_PROPERTY(float gamepadSize READ gamepadSize WRITE setGamepadSize NOTIFY gamepadSizeChanged)
public:
    explicit OptionsDialog(QObject *parent = NULL);
    ~OptionsDialog();
    
    void opened();
    void accept();


    QStandardItemModel * keyboardShortcuts() const
    {
        return m_keyboardShortcuts;
    }

    QUrl assetsPath() const;
    int language() const;
    int renderBackend() const;
    int theme() const;

    int keyForward() const
    {
        return m_keyForward;
    }

    int keyBackward() const
    {
        return m_keyBackward;
    }

    int keyRight() const
    {
        return m_keyRight;
    }

    int keyLeft() const
    {
        return m_keyLeft;
    }

    int keyUp() const
    {
        return m_keyUp;
    }

    int keyDown() const
    {
        return m_keyDown;
    }

    int keyJump() const
    {
        return m_keyJump;
    }

    bool mouseInverse() const
    {
        return m_mouseInverse;
    }

    bool gamepadSwap() const
    {
        return m_gamepadSwap;
    }

    float gamepadSensitivity() const
    {
        return m_gamepadSensitivity;
    }

    int mouseFreq() const
    {
        return m_mouseFreq;
    }

    float mouseSensitivity() const
    {
        return m_mouseSensitivity;
    }

    float gamepadSize() const
    {
        return m_gamepadSize;
    }

public slots:
    void apply();

    void setKeyboardShortcuts(QStandardItemModel * arg)
    {
        if (m_keyboardShortcuts == arg)
            return;

        m_keyboardShortcuts = arg;
        emit keyboardShortcutsChanged(arg);
    }

    void setAssetsPath(const QUrl& arg);
    void setLanguage(int arg);
    void setRenderBackend(int arg);
    void setTheme(int arg);

    void setKeyForward(int arg)
    {
        if (m_keyForward == arg)
            return;

        m_keyForward = arg;
        emit keyForwardChanged(arg);
    }

    void setKeyBackward(int arg)
    {
        if (m_keyBackward == arg)
            return;

        m_keyBackward = arg;
        emit keyBackwardChanged(arg);
    }

    void setKeyRight(int arg)
    {
        if (m_keyRight == arg)
            return;

        m_keyRight = arg;
        emit keyRightChanged(arg);
    }

    void setKeyLeft(int arg)
    {
        if (m_keyLeft == arg)
            return;

        m_keyLeft = arg;
        emit keyLeftChanged(arg);
    }

    void setKeyUp(int arg)
    {
        if (m_keyUp == arg)
            return;

        m_keyUp = arg;
        emit keyUpChanged(arg);
    }

    void setKeyDown(int arg)
    {
        if (m_keyDown == arg)
            return;

        m_keyDown = arg;
        emit keyDownChanged(arg);
    }

    void setKeyJump(int arg)
    {
        if (m_keyJump == arg)
            return;

        m_keyJump = arg;
        emit keyJumpChanged(arg);
    }

    void setMouseInverse(bool arg)
    {
        if (m_mouseInverse == arg)
            return;

        m_mouseInverse = arg;
        emit mouseInverseChanged(arg);
    }

    void setGamepadSwap(bool arg)
    {
        if (m_gamepadSwap == arg)
            return;

        m_gamepadSwap = arg;
        emit gamepadSwapChanged(arg);
    }

    void setGamepadSensitivity(float arg)
    {
        if (m_gamepadSensitivity == arg)
            return;

        m_gamepadSensitivity = arg;
        emit gamepadSensitivityChanged(arg);
    }

    void setMouseFreq(int arg)
    {
        if (m_mouseFreq == arg)
            return;

        m_mouseFreq = arg;
        emit mouseFreqChanged(arg);
    }

    void setMouseSensitivity(float arg)
    {
        if (m_mouseSensitivity == arg)
            return;

        m_mouseSensitivity = arg;
        emit mouseSensitivityChanged(arg);
    }

    void setGamepadSize(float arg)
    {
        if (m_gamepadSize == arg)
            return;
        m_gamepadSize = arg;
        emit gamepadSizeChanged(arg);
    }

signals:
    void keyboardShortcutsChanged(QStandardItemModel * arg);

    void assetsPathChanged(QUrl arg);
/*
private slots:

    void accepted();
    void rejected();
*/
    void languageChanged(int arg);

    void renderBackendChanged(int arg);

    void themeChanged(int arg);

    void keyForwardChanged(int arg);

    void keyBackwardChanged(int arg);

    void keyRightChanged(int arg);

    void keyLeftChanged(int arg);

    void keyUpChanged(int arg);

    void keyDownChanged(int arg);

    void keyJumpChanged(int arg);

    void mouseInverseChanged(bool arg);

    void gamepadSwapChanged(bool arg);

    void gamepadSensitivityChanged(float arg);

    void mouseFreqChanged(int arg);

    void mouseSensitivityChanged(float arg);

    void gamepadSizeChanged(float arg);

private:
    void setupEnvironment(QQuickItem * item);
    void setupAssets(QQuickItem * item);

    QQuickItem * ui;
    QStandardItemModel * m_keyboardShortcuts;
    QUrl m_assetsPath;
    int m_language;
    int m_renderBackend;
    int m_theme;
    int m_keyForward;
    int m_keyBackward;
    int m_keyRight;
    int m_keyLeft;
    int m_keyUp;
    int m_keyDown;
    int m_keyJump;
    bool m_mouseInverse;
    bool m_gamepadSwap;
    float m_gamepadSensitivity;
    int m_mouseFreq;
    float m_mouseSensitivity;
    float m_gamepadSize;
};

#endif // CONFIGDIALOG_H
