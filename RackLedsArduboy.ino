#include <Arduboy2.h>

uint16_t random16(uint16_t from, uint16_t to) {
    return from + (rand()%(to-from));
}

#include "Blinkenlights.h"

#define NUM_LEDS 3

class ArduLedImpl {
private:
    uint8_t leds[NUM_LEDS];
public:
    inline void init() {
        memset(leds, 0, sizeof(leds));
        pinMode(30, OUTPUT);
        pinMode(17, OUTPUT);
    }
    inline void ledOn(uint8_t idx) {
        leds[idx] = 1;
    }
    inline void ledOff(uint8_t idx) {
        leds[idx] = 0;
    }
    inline uint8_t ledState(uint8_t idx) const {
        return (uint8_t)(leds[idx] != 0);
    }
    inline void refresh() {
        digitalWrite(17, leds[0] ? HIGH : LOW);
        digitalWrite(30, leds[1] ? HIGH : LOW);
        //digitalWrite(GREEN_LED, leds[2] ? HIGH : LOW);
    }
};

Arduboy2Base arduboy;

Blinkenlights<NUM_LEDS, ArduLedImpl> blinkenlights;

void setup() {
    arduboy.begin();
    arduboy.setFrameRate(60);
    blinkenlights.init();
}

void loop() {
    blinkenlights.tick();
}
