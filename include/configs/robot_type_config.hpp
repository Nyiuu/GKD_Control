#ifndef __CONFIG_TYPE__
#define __CONFIG_TYPE__
#include "macro_helpers.hpp"

//
#include MUXDEF(CONFIG_SENTRY, "config_sentry.hpp", MUXDEF(CONFIG_HERO, "config_hero.hpp", MUXDEF(CONFIG_infaNTRY, "config_infantry.hpp", "config_fallback.hpp")))

#endif
