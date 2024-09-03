#include "blishhud_shader.h"
KWIN_EFFECT_FACTORY_ENABLED(KWin::BlishHUDShader,
                            "metadata.json",
                            return KWin::BlishHUDShader::supported();)
#include "plugin.moc"
