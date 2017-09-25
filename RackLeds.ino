#include "FastLED.h"
#include "Blinkenlights.h"

// How many leds in your strip?
#define NUM_LEDS 150

#define DATA_PIN 3

class FastLedImpl {
public:
    CRGB leds[NUM_LEDS];
    inline void init() {
        FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    }
    inline void ledOn(uint8_t idx, CRGB color) {
        leds[idx] = color;
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
    random16_set_seed(42);
    blinkenlights.init();
}

int n=0;

void loop() {
    //blinkenlights.tick();
    memset(blinkenlights.p.leds, 0, sizeof(blinkenlights.p.leds));
    for(auto i=0; i<10; ++i) {
        blinkenlights.p.leds[disks[0][n%10]] = 0x00ff00;
        blinkenlights.p.leds[disks[1][n%10]] = 0x00ff00;
        blinkenlights.p.leds[disks[2][n%10]] = 0x00ff00;
    }
    //blinkenlights.p.leds[n%118] = 0xff0000;
    n++;
    FastLED.show();
    delay(400);
}
