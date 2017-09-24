#pragma once

#include <stdint.h>
#include <stdlib.h>

template<size_t ledCount, typename LedImpl>
class Blinkenlights {
public:
    LedImpl p;

    /// time remaining for next toggle of each lED
    int16_t timeLeftOn[ledCount] = {0};

    void tick();
    void init() { p.init(); }
};

const uint16_t TICK=20;

template<size_t ledCount, typename LedImpl>
void Blinkenlights<ledCount, LedImpl>::tick() {
  for (uint8_t i=0; i<ledCount; ++i) {
    timeLeftOn[i] -= TICK;
    if (timeLeftOn[i] < 0) {
      if (p.ledState(i)) {
        p.ledOff(i);
        timeLeftOn[i] = 80;
      } else {
        p.ledOn(i);
        timeLeftOn[i] = (i==2 ? 70 :
                         i==5 ? random16(500,3000) :
                         random16(50,300));
      }
    }
  }
  p.refresh();
  delay(TICK);
}
