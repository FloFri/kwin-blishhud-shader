#include "blishhud_shader.h"
Q_NAMESPACE

KWIN_EFFECT_FACTORY_ENABLED(KWin::BlishHUDShader,
                            "metadata.json",
                            return KWin::BlishHUDShader::supported();)
#include "plugin.moc"
