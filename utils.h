#pragma once

#if RL_TARGET_FASTLED

#include "libraries/FastLED/lib8tion.h"

#elif RL_TARGET_ARDUINO

inline uint8_t random8(uint8_t lim) {
    return (uint8_t)random(lim);
}
inline uint8_t random8(uint8_t min, uint8_t lim) {
    return (uint8_t)random(lim, min);
}

#endif
