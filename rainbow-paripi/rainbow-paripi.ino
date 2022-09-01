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

#define BRIGHTNESS 128   /* Control the brightness of your leds */
#define SATURATION 255   /* Control the saturation of your leds */
#define MAX_HUE 255
#define MAX_BRIGHTNESS 255
#define MAX_SATURATION 255

CRGB leds[NUM_LEDS];
int hue;
int brightness;
int saturation;

void setup() { 
    FastLED.addLeds<NEOPIXEL, PIN_LEDS>(leds, NUM_LEDS);
    randomSeed(analogRead(0)); 
}

void loop() {
    for (int i = 0; i < NUM_LEDS; i = i + 2) {
        // 1st
        hue = random(MAX_HUE+1);
        brightness = random(MAX_BRIGHTNESS+1);
        saturation = random(MAX_SATURATION+1);
        leds[i] = CHSV(hue, saturation, brightness);

        // 2nd
        hue = MAX_HUE - hue;
        brightness = MAX_BRIGHTNESS - brightness;
        saturation = MAX_SATURATION - saturation;
        leds[i + 1] = CHSV(hue, saturation, brightness);
    }
    FastLED.show();
    delay(100);
}
