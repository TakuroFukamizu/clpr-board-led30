#include <FastLED.h>

/* Extra IO Pins
4: EXT IO: G21 / I2C SDA
5: EXT IO: G22 / I2C SCL
6: EXT IO: G25 / DAC1
7: EXT IO: G33 / ADC5 / TOUCH8
8: EXT IO: G18 / SPI SCK
9: EXT IO: G19 / SPI MISO
10: EXT IO: G23 / SPI MOSI
 */

#define PIN_LEDS 27
#define NUM_LEDS 30

#define BRIGHTNESS 255   /* Control the brightness of your leds */
#define SATURATION 128   /* Control the saturation of your leds */

CRGB leds[NUM_LEDS];
int hue;

void setup() { 
    FastLED.addLeds<NEOPIXEL, PIN_LEDS>(leds, NUM_LEDS);
    randomSeed(analogRead(0)); 
}

void loop() {
    for (int i = 0; i < NUM_LEDS; i = i + 2) {
        // 1st
        hue = (int)random(0, 128) * 2;
        leds[i] = CHSV(hue, BRIGHTNESS, SATURATION);

        // 2nd
        hue = (int)random(0, 16) * 16;
        leds[i + 1] = CHSV(hue, BRIGHTNESS, SATURATION);
    }
    FastLED.show();
    delay(100);
}
