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

#include "optionsdialog.h"

OptionsDialog::OptionsDialog(QObject *parent) :
    Dialog(parent)
{
    /*
    QListWidgetItem *configButton = new QListWidgetItem(ui->listWidget);
    //configButton->setIcon(QIcon(":/images/config.png"));
    configButton->setText(tr("Configuration"));
    configButton->setTextAlignment(Qt::AlignHCenter);
    configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *updateButton = new QListWidgetItem(ui->listWidget);
    //updateButton->setIcon(QIcon(":/images/update.png"));
    updateButton->setText(tr("Update"));
    updateButton->setTextAlignment(Qt::AlignHCenter);
    updateButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    loadAssets();*/
    m_theme=0;
    m_renderBackend=0;
    m_language=0;

    m_gamepadSize=0;

}

OptionsDialog::~OptionsDialog()
{
}

QUrl OptionsDialog::assetsPath() const
{
    return m_assetsPath;
}

void OptionsDialog::opened()
{
    setAssetsPath(GlobalSettings::instance()->assetsPath());
    setLanguage(GlobalSettings::instance()->language());
    setRenderBackend(GlobalSettings::instance()->renderBackend());
    setTheme(GlobalSettings::instance()->theme());

    setKeyForward(ControlSettings::key_forward);
    setKeyBackward(ControlSettings::key_backward);
    setKeyRight(ControlSettings::key_right);
    setKeyLeft(ControlSettings::key_left);
    setKeyUp(ControlSettings::key_up);
    setKeyDown(ControlSettings::key_down);
    setKeyJump(ControlSettings::key_jump);

    setMouseFreq(ControlSettings::mouse_freq);
    setMouseInverse(ControlSettings::mouse_inverse);
    setMouseSensitivity(ControlSettings::mouse_sensitivity);

    setGamepadSensitivity(ControlSettings::gamepad_sensitivity);
    setGamepadSwap(ControlSettings::gamepad_swap);
    setGamepadSize(ControlSettings::gamepad_size);
}

void OptionsDialog::accept()
{
    apply();
    close();
}

int OptionsDialog::language() const
{
    return m_language;
}

int OptionsDialog::renderBackend() const
{
    return m_renderBackend;
}

int OptionsDialog::theme() const
{
    return m_theme;
}

void OptionsDialog::setAssetsPath(const QUrl &arg)
{
    if (m_assetsPath == arg)
        return;

    m_assetsPath = arg;
    emit assetsPathChanged(arg);
}

void OptionsDialog::apply()
{
    GlobalSettings::instance()->setAssetsPath(m_assetsPath);
    GlobalSettings::instance()->setLanguage((GlobalSettings::Language)m_language);
    GlobalSettings::instance()->setRenderBackend((GlobalSettings::RenderBackend)m_renderBackend);
    GlobalSettings::instance()->setTheme((GlobalSettings::Theme)m_theme);

    ControlSettings::key_forward=m_keyForward;
    ControlSettings::key_backward=m_keyBackward;
    ControlSettings::key_right=m_keyRight;
    ControlSettings::key_left=m_keyLeft;
    ControlSettings::key_up=m_keyUp;
    ControlSettings::key_down=m_keyDown;
    ControlSettings::key_jump=m_keyJump;

    ControlSettings::mouse_freq=m_mouseFreq;
    ControlSettings::mouse_inverse=m_mouseInverse;
    ControlSettings::mouse_sensitivity=m_mouseSensitivity;

    ControlSettings::gamepad_sensitivity=m_mouseSensitivity;
    ControlSettings::gamepad_swap=m_gamepadSwap;
    ControlSettings::gamepad_size=m_gamepadSize;

}

void OptionsDialog::setLanguage(int arg)
{
    if (m_language == arg)
        return;

    m_language = arg;
    emit languageChanged(arg);
}

void OptionsDialog::setRenderBackend(int arg)
{
    if (m_renderBackend == arg)
        return;

    m_renderBackend = arg;
    emit renderBackendChanged(arg);
}

void OptionsDialog::setTheme(int arg)
{
    if (m_theme == arg)
        return;

    m_theme = arg;
    emit themeChanged(arg);
}
/*
void OptionsDialog::accepted()
{
    close();
}

void OptionsDialog::rejected()
{
    close();
}
*/
void OptionsDialog::setupEnvironment(QQuickItem *item)
{

}

void OptionsDialog::setupAssets(QQuickItem *item)
{

}
