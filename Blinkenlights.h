#pragma once

#include <stdint.h>
#include <stdlib.h>

struct LedState {
    uint8_t baseState:1;
    /// time remaining for next toggle
    int16_t timeLeftOn;
    int8_t broadcast;
};

template<size_t ledCount, typename LedImpl>
class Blinkenlights {
public:
    LedImpl p;

    LedState led[ledCount];

    void tick();
    void init() {
        p.init();
        memset(led, 0, sizeof(led));
    }
};

const uint16_t TICK=20;

template<size_t ledCount, typename LedImpl>
void Blinkenlights<ledCount, LedImpl>::tick() {
  if (rand() % 100 == 2) {
    for (uint8_t i=0; i<ledCount; ++i) {
      if (rand() % 3 != 0) {
        led[i].broadcast = 80;
      }
    }
  }
  for (uint8_t i=0; i<ledCount; ++i) {
    led[i].timeLeftOn -= TICK;
    if (led[i].broadcast > 0) {
        led[i].broadcast -= TICK;
    }
    if (led[i].timeLeftOn < 0) {
      if (led[i].baseState) {
        led[i].baseState = 0;
        p.ledOff(i);
        led[i].timeLeftOn = 80;
      } else {
        led[i].baseState = 1;
        p.ledOn(i);
        led[i].timeLeftOn = (
            i==2 ? 70 : // constantly busy
            i==5 ? random16(500,3000) :  // mostly idle
            random16(10,600));
      }
    }
    if (led[i].baseState && led[i].broadcast <= 0) {
      p.ledOn(i);
    } else {
      p.ledOff(i);
    }
  }
  p.refresh();
  delay(TICK);
}
