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

#include "blishhud_shader.h"

#include <kwinglutils.h>

namespace KWin
{

  BlishHUDShader::BlishHUDShader()
    :   m_inited(false),
        m_valid(true),
        m_shader(nullptr),
        m_allWindows(true)
  {
    connect(effects, &EffectsHandler::windowAdded, this, &BlishHUDShader::slotWindowAdded);
    connect(effects, &EffectsHandler::windowClosed, this, &BlishHUDShader::slotWindowClosed);
  }

  BlishHUDShader::~BlishHUDShader() = default;

  bool BlishHUDShader::supported()
  {
    return effects->compositingType() == OpenGLCompositing;
  }
  
  bool BlishHUDShader::isInvertable(EffectWindow *window) const
{
    return m_allWindows != m_windows.contains(window);
}
  
  void BlishHUDShader::invert(EffectWindow *window)
  {
      if (m_valid && !m_inited) {
          m_valid = loadData();
      }

      redirect(window);
      setShader(window, m_shader.get());
  }

  void BlishHUDShader::uninvert(EffectWindow *window)
  {
      unredirect(window);
  }

  bool BlishHUDShader::loadData()
  {
    m_inited = true;

    m_shader = ShaderManager::instance()->generateShaderFromFile(ShaderTrait::MapTexture, QString(), QStringLiteral("/usr/share/kwin/shaders/transparent_black.frag"));
    if (!m_shader->isValid()) {
      //qCCritical(KWIN_INVERT) << "The shader failed to load!";
      return false;
    }

    return true;
  }

  void BlishHUDShader::slotWindowAdded(KWin::EffectWindow *w)
  {
      QString caption = w->caption();
      
      if (caption.contains(QStringLiteral("Blish"))) {
          invert(w);
      }
  }
  
  void BlishHUDShader::slotWindowClosed(EffectWindow* w)
  {
    m_windows.removeOne(w);
  }

  bool BlishHUDShader::isActive() const
  {
    return m_valid && (m_allWindows || !m_windows.isEmpty());
  }

  bool BlishHUDShader::provides(Feature f)
  {
    return f == ScreenInversion;
  }

} // namespace
