#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 8

#define DATA_PIN 3

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() {
  // Uncomment/edit one of the following lines for your leds arrangement.
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

int16_t timeLeftOn[NUM_LEDS] = {0};

const uint16_t TICK=100;

void loop() {
  for (uint8_t i=0; i<NUM_LEDS; ++i) {
    timeLeftOn[i] -= TICK;
    if (timeLeftOn[i] < 0) {
      if ((int)leds[i] != 0) {
        leds[i] = 0;
        timeLeftOn[i] = 80;
      } else {
        leds[i] = 0x000400;
        timeLeftOn[i] = (i==2 ? 70 :
                         i==5 ? random16(500,3000) :
                         random16(50,300));
      }
    }
  }
  FastLED.show();
  delay(TICK);
}
