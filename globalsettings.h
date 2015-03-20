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
#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H

#include <QSettings>
#include <QStringList>
#include <QUrl>
#include "lib/tools/shell.h"

#include "./editor/controlsettings.h"

class GlobalSettings
{
public:
    GlobalSettings();

    enum Theme{
        DEFAULT=0,
        DARK
    };

    enum RenderBackend{
        OPENGL30=0,
        OPENGLES20,
        OPENGLES30
    };

    enum Language{
        English=0,
        Czech
    };

    void load();
    void save();
    static GlobalSettings *instance();

//getter
    const QStringList & recentProjects() const;
    const QUrl & assetsPath() const;
    RenderBackend renderBackend() const;
    Language language() const;
    Theme theme() const;

//setter
    void setRecentProjects(const QStringList & arg);
    void setAssetsPath(const QUrl & arg);
    void setRenderBackend(RenderBackend arg);
    void setLanguage(Language arg);
    void setTheme(Theme arg);


private:
    void setupDefault();
    QSettings settings;
    static GlobalSettings * self_ptr;
    QStringList m_recentProjects;
    QUrl m_assetsPath;
    RenderBackend m_renderBackend;
    Language m_language;
    Theme m_theme;
};

#endif // GLOBALSETTINGS_H
