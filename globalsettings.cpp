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

#include "globalsettings.h"

GlobalSettings::GlobalSettings()
{
   self_ptr=this;
#ifdef OS_ANDROID
   m_renderBackend=OPENGLES20;//HACK
   m_assetsPath="assets:/assets/assets.aas";
#else
    m_renderBackend=OPENGL30;
#endif
}

void GlobalSettings::load()
{
    m_recentProjects    = settings.value("recentProjects").toStringList();
    if(settings.contains("assetsPath")) m_assetsPath = settings.value("assetsPath").toUrl();
    if(settings.contains("renderBackend")) m_renderBackend = (RenderBackend)settings.value("renderBackend").toInt();
    m_language          = (Language) settings.value("language").toInt();
    m_theme             = (Theme) settings.value("theme").toInt();

    settings.beginGroup("peripherals");
        if(settings.contains("keyForward"))     ControlSettings::key_forward=settings.value("keyForward").toInt();
        if(settings.contains("keyBackward"))    ControlSettings::key_backward=settings.value("keyBackward").toInt();
        if(settings.contains("keyRight"))       ControlSettings::key_right=settings.value("keyRight").toInt();
        if(settings.contains("keyLeft"))        ControlSettings::key_left=settings.value("keyLeft").toInt();
        if(settings.contains("keyUp"))          ControlSettings::key_up=settings.value("keyUp").toInt();
        if(settings.contains("keyDown"))        ControlSettings::key_down=settings.value("keyDown").toInt();
        if(settings.contains("keyJump"))        ControlSettings::key_jump=settings.value("keyJump").toInt();

        if(settings.contains("mouseFreq"))        ControlSettings::mouse_freq=settings.value("mouseFreq").toInt();
        if(settings.contains("mouseInverse"))     ControlSettings::mouse_inverse=settings.value("mouseInverse").toBool();
        if(settings.contains("mouseSensitivity")) ControlSettings::mouse_sensitivity=settings.value("mouseSensitivity").toFloat();

        if(settings.contains("gamepadSwap"))        ControlSettings::gamepad_swap=settings.value("gamepadSwap").toBool();
        if(settings.contains("gamepadSensitivity")) ControlSettings::gamepad_sensitivity=settings.value("gamepadSensitivity").toFloat();
        if(settings.contains("gamepadSize"))        ControlSettings::gamepad_size=settings.value("gamepadSize").toFloat();
    settings.endGroup();
}

void GlobalSettings::save()
{
    settings.setValue("recentProjects",m_recentProjects);
    settings.setValue("assetsPath",m_assetsPath);
    settings.setValue("renderBackend",m_renderBackend);
    settings.setValue("language",m_language);
    settings.setValue("theme", m_theme);

    settings.beginGroup("peripherals");
    settings.setValue("keyForward",     ControlSettings::key_forward);
    settings.setValue("keyBackward",    ControlSettings::key_backward);
    settings.setValue("keyRight",       ControlSettings::key_right);
    settings.setValue("keyLeft",        ControlSettings::key_left);
    settings.setValue("keyUp",          ControlSettings::key_up);
    settings.setValue("keyDown",        ControlSettings::key_down);
    settings.setValue("keyJump",        ControlSettings::key_jump);

    settings.setValue("mouseFreq",          ControlSettings::mouse_freq);
    settings.setValue("mouseInverse",       ControlSettings::mouse_inverse);
    settings.setValue("mouseSensitivity",   ControlSettings::mouse_sensitivity);

    settings.setValue("gamepadSwap",        ControlSettings::gamepad_swap);
    settings.setValue("gamepadSensitivity", ControlSettings::gamepad_sensitivity);
    settings.setValue("gamepadSize",        ControlSettings::gamepad_size);

    settings.endGroup();

    settings.sync();
}

GlobalSettings *GlobalSettings::instance()
{
    return self_ptr;
}

void GlobalSettings::setupDefault()
{
    settings.setValue("recentProjects",m_recentProjects);
    settings.setValue("assetsPath",m_assetsPath);
    settings.setValue("renderBackend",m_renderBackend);
    settings.setValue("language",m_language);
    settings.setValue("theme", m_theme);
    settings.sync();
}

GlobalSettings * GlobalSettings::self_ptr=NULL;

//properties

const QStringList &GlobalSettings::recentProjects() const
{
    return m_recentProjects;
}

const QUrl &GlobalSettings::assetsPath() const
{
    return m_assetsPath;
}

GlobalSettings::RenderBackend GlobalSettings::renderBackend() const
{
    return m_renderBackend;
}

GlobalSettings::Language GlobalSettings::language() const
{
    return m_language;
}

GlobalSettings::Theme GlobalSettings::theme() const
{
    return m_theme;
}

void GlobalSettings::setRecentProjects(const QStringList &arg)
{
    if (m_recentProjects != arg) {
        m_recentProjects = arg;
    }
}

void GlobalSettings::setAssetsPath(const QUrl &arg)
{
    if (m_assetsPath != arg) {
        m_assetsPath = arg;
    }
}

void GlobalSettings::setRenderBackend(RenderBackend arg)
{
    if (m_renderBackend == arg)
        return;

    m_renderBackend = arg;
}

void GlobalSettings::setLanguage(Language arg)
{
    if (m_language == arg)
        return;

    m_language = arg;
}

void GlobalSettings::setTheme(Theme arg)
{
    if (m_theme == arg)
        return;

    m_theme = arg;
}

