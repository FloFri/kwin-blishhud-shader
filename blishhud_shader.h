/********************************************************************
 KWin - the KDE window manager
 This file is part of the KDE project.

Copyright (C) 2007 Rivo Laks <rivolaks@hot.ee>
Copyright (C) 2008 Lucas Murray <lmurray@undefinedfire.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*********************************************************************/

#ifndef KWIN_INVERT_H
#define KWIN_INVERT_H

#include <QtGlobal>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <kwinoffscreeneffect.h>
#else
#include <effect/offscreeneffect.h>
#endif

namespace KWin
{

class GLShader;

/**
 * Inverts desktop's colors
 */
class BlishHUDShader
    : public OffscreenEffect
{
    Q_OBJECT
public:
    BlishHUDShader();
    ~BlishHUDShader() override;

    bool isActive() const override;
    bool provides(Feature) override;

    int requestedEffectChainPosition() const override;

    static bool supported();

public Q_SLOTS:
    
    void slotWindowAdded(KWin::EffectWindow *w);
    void slotWindowClosed(KWin::EffectWindow *w);

protected:
    bool loadData();

private:
    bool isInvertable(EffectWindow *window) const;
    void invert(EffectWindow *window);
    void uninvert(EffectWindow *window);
    
    bool m_inited;
    bool m_valid;
    std::unique_ptr<KWin::GLShader> m_shader;
    bool m_allWindows;
    QList<EffectWindow*> m_windows;
};

inline int BlishHUDShader::requestedEffectChainPosition() const
{
    return 99;
}

} // namespace

#endif
