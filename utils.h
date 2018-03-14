#pragma once

#ifdef RL_TARGET_FASTLED

#include "libraries/FastLED/lib8tion.h"

inline uint8_t rl_random8(uint8_t lim) {
    return random8(lim);
}

#elif RL_TARGET_ARDUINO

inline uint8_t rl_random8(uint8_t lim) {
    return (uint8_t)random(lim);
}

#endif
