#include <FastLED.h>

#define PIN_LEDS 27
#define NUM_LEDS 30

#define BRIGHTNESS 128   /* Control the brightness of your leds */
#define SATURATION 255   /* Control the saturation of your leds */


CRGB leds[NUM_LEDS];
int hue;
uint8_t gamingHueIndex = 0;

void setup() { 
    FastLED.addLeds<NEOPIXEL, PIN_LEDS>(leds, NUM_LEDS);
    randomSeed(analogRead(0)); 
}

void loop() {
    hue = gamingHueIndex;
    for (int i = 0; i < NUM_LEDS; i++) {
        hue += 10;
        if (255 < hue) {
            hue - 0;
        }
        leds[i] = CHSV(hue, SATURATION, BRIGHTNESS);
    }
    FastLED.show();
    gamingHueIndex+=20;
    if (255 < gamingHueIndex) {
        gamingHueIndex = 0;
    }
    delay(100);
}
