#pragma once

#include <stdint.h>
#include <stdlib.h>

struct LedState {
    CRGB color;
    uint8_t baseState:1;
    uint8_t isIdle:1;
    uint8_t isFrequent:1;
    /// time remaining for next toggle
    int16_t timeLeftOn;
    /// how much longer this LED will be off to simulate a broadcast
    int8_t broadcast;
};

CRGB colors[] = {
    0x008800,
    0x008800,
    0x009900,
    0x999900,
    0xff0000,
    0x66cc00,
};

uint8_t disks[][10] = {
    {12,11,10,9,8,7,6,5,4,3},
    {28,27,26,25,24,23,22,21,20,19},
    {102,103,104,105,106,107,108,109,110,111}
};

template<size_t ledCount, typename LedImpl>
class Blinkenlights {
public:
    LedImpl p;

    LedState led[ledCount];
    int16_t timeToNextRecolor;

    void tick();
    void randomColors() {
        for (uint8_t i=0; i<ledCount; ++i) {
//            led[i].color = FL_PGM_READ_DWORD_NEAR(RainbowColors_p + random8(16));
            led[i].color = colors[random8(6)];
            //led[i].color = 0x00ff00;
        }
    }
    void init() {
        p.init();
        memset(led, 0, sizeof(led));
        for (uint8_t i=0; i<ledCount/5; ++i) {
            led[random16(0, ledCount)].isIdle = 1;
        }
        for (uint8_t i=0; i<ledCount/10; ++i) {
            led[random16(0, ledCount)].isFrequent = 1;
        }
        timeToNextRecolor = 0;
        randomColors();
    }
};

const uint16_t TICK=20;

template<size_t ledCount, typename LedImpl>
void Blinkenlights<ledCount, LedImpl>::tick() {
#if 0
  if (rand() % 100 == 2) {
    for (uint8_t i=0; i<ledCount; ++i) {
      if (rand() % 3 != 0) {
        led[i].broadcast = 80;
      }
    }
  }
#endif
  timeToNextRecolor -= TICK;
  if (timeToNextRecolor <= 0) {
      randomColors();
      timeToNextRecolor = 10000;
  }

  for (uint8_t i=0; i<ledCount; ++i) {
    led[i].timeLeftOn -= TICK;
    if (led[i].broadcast > 0) {
        led[i].broadcast -= TICK;
    }
    if (led[i].timeLeftOn < 0) {
      if (led[i].baseState) {
        led[i].baseState = 0;
        led[i].timeLeftOn = 80;
      } else {
        led[i].baseState = 1;
        led[i].timeLeftOn = (
            led[i].isFrequent ? random(50,80) : // constantly busy
            led[i].isIdle ? random16(500,5000) :  // mostly idle
            random16(10,600));
      }
    }
    if (led[i].baseState && led[i].broadcast <= 0) {
      p.ledOn(i, led[i].color);
    } else {
      p.ledOff(i);
    }
  }
  p.refresh();
  delay(TICK);
}
