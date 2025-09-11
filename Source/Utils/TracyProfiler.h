#pragma once

#ifdef TRACY_ENABLE
#include <tracy/Tracy.hpp>
#define SPECTRAL_PROFILE_ZONE(name) ZoneScopedN(name)
#define SPECTRAL_PROFILE_FRAME() FrameMark
#else
#define SPECTRAL_PROFILE_ZONE(name)
#define SPECTRAL_PROFILE_FRAME()
#endif