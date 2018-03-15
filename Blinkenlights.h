#pragma once

#include <stdint.h>
#include <stdlib.h>

#include "utils.h"

struct LedState {
    uint32_t color;
    uint8_t baseState:1;
    uint8_t isIdle:1;
    uint8_t isFrequent:1;
    /// time remaining for next toggle
    int16_t timeLeftOn;
    /// how much longer this LED will be off to simulate a broadcast
    int8_t broadcast;
};

uint32_t colors[] = {
    0x008800,
    0x008800,
    0x009900,
    0x008800,
    0x009900,
    0x009900,
    0x999900,
    0xff0000,
    0x000088,
};

uint8_t disks[][10] = {
    {12,11,10,9,8,7,6,5,4,3},
    {28,27,26,25,24,23,22,21,20,19},
    {102,103,104,105,106,107,108,109,110,111}
};

// This array specifies the number of time units the LED will be on.
// Negative numbers mean LED off. There will be 1 unit of LED off between each item anyway,
// thus using 1,1,1 is 1 on, 1 off, 1 on, 1 off, 1 on
// and 1,-1,1 means 1 on, 3 off, 1 on.

char morseString[] = "PUTO EL QUE LEE";

int8_t morse[] = {
    1,3,3,1,-1,     // P
    1,1,3,-1,       // U
    3,-1,           // T
    3,3,3,-1,       // O
    -5,
    1,-1,           // E
    1,3,1,1,-1,     // L
    -5,
    3,3,1,3,-1,     // Q
    1,1,3,-1,       // U
    1,-1,           // E
    -5,
    1,3,1,1,-1,     // L
    1,-1,           // E
    1,-1,           // E
    -9
};
const uint8_t morseLed = 60;
const int16_t morseUnitLength = 130;
const uint32_t morseColor = 0x00ffff;
const uint16_t morseDataLength = sizeof(morse) / sizeof(*morse);
uint8_t morseIdx = 0;
int morseChrIdx = 0;
int16_t morseRemaining = morseUnitLength;

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
            led[i].color = colors[random8(sizeof(colors)/sizeof(*colors))];
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
        Serial.begin(9600);
    }
    void updateMorse();
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
        if (i == morseLed) continue;
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
                    led[i].isFrequent ? random16(50,80) :  // constantly busy
                    led[i].isIdle ? random16(500,5000) : // mostly idle
                    random16(10,600)
                );
            }
        }
        if (led[i].baseState && led[i].broadcast <= 0) {
            p.ledOn(i, led[i].color);
        } else {
            p.ledOff(i);
        }
    }

    updateMorse();

    p.refresh();
    delay(TICK);
}

#ifdef MORSE_DEBUGGING_CLOCK
int16_t clock = morseUnitLength/2;
bool clockBlink = true;
#endif
template<size_t ledCount, typename LedImpl>
void Blinkenlights<ledCount, LedImpl>::updateMorse() {
    morseRemaining -= TICK;
#ifdef MORSE_DEBUGGING_CLOCK
    clock -= TICK;
    if (clock <= 0) {
        if (shit)
        p.ledOff(0);
        else p.ledOn(0,0xff0000);
        shit = !shit;
        clock = morseUnitLength / 2;
    }
#endif
    if (morseRemaining <= morseUnitLength) {
        p.ledOff(morseLed);
    }
    if (morseRemaining <= 0) {
        if (morse[morseIdx] < 0) {
            // next interval is off
            p.ledOff(morseLed);
            Serial.write(morseString[morseChrIdx]);
            morseChrIdx++;
        } else {
            p.ledOn(morseLed, morseColor);
        }
        morseRemaining = morseUnitLength * (1+abs(morse[morseIdx]));
        morseIdx++;
        if (morseIdx >= morseDataLength) {
            morseIdx = 0;
            morseChrIdx = 0;
            Serial.write('\r');
            Serial.write('\n');
        }
    }
}
