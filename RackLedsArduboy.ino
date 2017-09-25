#include <Arduboy2.h>

uint16_t random16(uint16_t from, uint16_t to) {
    return from + (rand()%(to-from));
}

#include "Blinkenlights.h"

#define NUM_LEDS 30

class ArduLedImpl {
private:
    uint8_t leds[NUM_LEDS];
public:
    inline void init() {
        memset(leds, 0, sizeof(leds));
    }
    inline void ledOn(uint8_t idx, uint32_t color) {
        leds[idx] = 1;
    }
    inline void ledOff(uint8_t idx) {
        leds[idx] = 0;
    }
    inline uint8_t ledState(uint8_t idx) const {
        return (uint8_t)(leds[idx] != 0);
    }
    inline void refresh();
};

Arduboy2Base arduboy;

Blinkenlights<NUM_LEDS, ArduLedImpl> blinkenlights;

#define RADIUS 3
#define SPACING 6
#define gridposx(n) ((RADIUS+1) + SPACING*2*(n))

void ArduLedImpl::refresh() {
    arduboy.clear();

    for (uint_fast8_t i=0; i<NUM_LEDS; ++i) {
        if (leds[i]) {
            arduboy.fillCircle(gridposx(i%10), gridposx(i/10), RADIUS, WHITE);
        }
    }

    arduboy.display();
}

void setup() {
    arduboy.begin();
    arduboy.setFrameRate(60);
    blinkenlights.init();
}

void loop() {
    blinkenlights.tick();
}
