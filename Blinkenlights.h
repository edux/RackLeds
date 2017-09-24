#pragma once

#include <stdint.h>
#include <stdlib.h>

struct LedState {
    uint8_t baseState:1;
    uint8_t isIdle:1;
    uint8_t isFrequent:1;
    /// time remaining for next toggle
    int16_t timeLeftOn;
    /// how much longer this LED will be off to simulate a broadcast
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
        for (uint8_t i=0; i<ledCount/5; ++i) {
            led[random16(0, ledCount)].isIdle = 1;
        }
        for (uint8_t i=0; i<ledCount/10; ++i) {
            led[random16(0, ledCount)].isFrequent = 1;
        }
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
            led[i].isFrequent ? random(50,80) : // constantly busy
            led[i].isIdle ? random16(500,5000) :  // mostly idle
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
