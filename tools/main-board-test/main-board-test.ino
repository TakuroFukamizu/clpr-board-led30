#include <FastLED.h>

// M5Atom Lite

#define PIN_LEDS 25
#define NUM_LEDS 30

CRGB leds[NUM_LEDS];

/*
0: all led off
1: RED, odd leds
2: RED, even leds
3: GREEN, odd leds
4: GREEN, even leds
5: BLUE, odd leds
6: BLUE, even leds
7: WHITE, all leds
 */
int modeIndex = 0;

void setup() { 
    FastLED.addLeds<NEOPIXEL, PIN_LEDS>(leds, NUM_LEDS);
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Black;
    }
    FastLED.show();
}

void loop() {
    // Loop RGB LED
    CRGB color;
    bool oddLed = false;
    bool evenLed = false;
    switch (modeIndex)
    {
    case 0:
        oddLed = true;
        evenLed = true;
        color = CRGB::Black;
        break;
    case 1:
        oddLed = true;
        color = CRGB::Red;
        break;
    case 2:
        evenLed = true;
        color = CRGB::Red;
        break;
    case 3:
        oddLed = true;
        color = CRGB::Green;
        break;
    case 4:
        evenLed = true;
        color = CRGB::Green;
        break;
    case 5:
        oddLed = true;
        color = CRGB::Blue;
        break;
    case 6:
        evenLed = true;
        color = CRGB::Blue;
        break;
    case 7:
        oddLed = true;
        evenLed = true;
        color = CRGB::White;
        break;
    }
    for (int i = 0; i < NUM_LEDS; i++)
    {
        if (i % 2 != 0) {
            if (oddLed) {
                leds[i] = color;
            } else {
                leds[i] = CRGB::Black;
            }
        } else {
            if (evenLed) {
                leds[i] = color;
            } else {
                leds[i] = CRGB::Black;
            }
        }
    }
    FastLED.show();
    modeIndex++;
    if (7 < modeIndex) {
        modeIndex = 0;
    }

    delay(500);
}
