#pragma once

#include <stdint.h>
#include <stdlib.h>

template<size_t ledCount, typename LedImpl>
class Blinkenlights {
public:
    LedImpl p;

    uint8_t baseState[ledCount] = {0};
    /// time remaining for next toggle of each LED
    int16_t timeLeftOn[ledCount] = {0};
    int8_t broadcast[ledCount] = {0};

    void tick();
    void init() { p.init(); }
};

const uint16_t TICK=20;

template<size_t ledCount, typename LedImpl>
void Blinkenlights<ledCount, LedImpl>::tick() {
  if (rand() % 100 == 2) {
    for (uint8_t i=0; i<ledCount; ++i) {
      if (rand() % 3 != 0) {
        broadcast[i] = 80;
      }
    }
  }
  for (uint8_t i=0; i<ledCount; ++i) {
    timeLeftOn[i] -= TICK;
    if (broadcast[i] > 0) {
        broadcast[i] -= TICK;
    }
    if (timeLeftOn[i] < 0) {
      if (baseState[i]) {
        baseState[i] = 0;
        p.ledOff(i);
        timeLeftOn[i] = 80;
      } else {
        baseState[i] = 1;
        p.ledOn(i);
        timeLeftOn[i] = (i==2 ? 70 : // constantly busy
                         i==5 ? random16(500,3000) :  // mostly idle
                         random16(10,600));
      }
    }
    if (baseState[i] && broadcast[i] <= 0) {
      p.ledOn(i);
    } else {
      p.ledOff(i);
    }
  }
  p.refresh();
  delay(TICK);
}
