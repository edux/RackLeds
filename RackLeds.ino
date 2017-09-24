#include "FastLED.h"
#include "Blinkenlights.h"

// How many leds in your strip?
#define NUM_LEDS 150

#define DATA_PIN 3

class FastLedImpl {
private:
    CRGB leds[NUM_LEDS];
public:
    inline void init() {
        FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    }
    inline void ledOn(uint8_t idx) {
        leds[idx] = 0x004000;
    }
    inline void ledOff(uint8_t idx) {
        leds[idx] = 0x000000;
    }
    inline uint8_t ledState(uint8_t idx) const {
        return (uint8_t)(bool)(leds[idx]);
    }
    inline void refresh() {
        FastLED.show();
    }
};

Blinkenlights<NUM_LEDS, FastLedImpl> blinkenlights;

void setup() {
    blinkenlights.init();
}

void loop() {
    blinkenlights.tick();
}
