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
#define NUM_EXT_IOS 7

#define PIN_LEDS 27
#define NUM_LEDS 30

#define BRIGHTNESS 128   /* Control the brightness of your leds */
#define SATURATION 255   /* Control the saturation of your leds */

CRGB leds[NUM_LEDS];
int ledIndex = 0;
int ledColorIndex = 0;

int extIoIndex = 0;


int extIoPins[] = {18, 19, 21, 22, 23, 25, 33};

void setup() { 
    FastLED.addLeds<NEOPIXEL, PIN_LEDS>(leds, NUM_LEDS);
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Black;
    }
    FastLED.show();

    for (int i = 0; i < NUM_EXT_IOS; i++) {
        pinMode(extIoPins[i], OUTPUT);
    }
}

void loop() {
    // Loop RGB LED
    switch(ledColorIndex) {
        case 0: leds[ledIndex] = CRGB::Red; break;
        case 1: leds[ledIndex] = CRGB::Green; break;
        case 2: leds[ledIndex] = CRGB::Blue; break;
    }
    FastLED.show();
    ledIndex++;
    if (NUM_LEDS <= ledIndex) {
        ledIndex = 0;
        for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = CRGB::Black;
        }
        ledColorIndex++;
    }
    if (3 <= ledColorIndex) {
        ledColorIndex = 0;
    }

    // Check Ext IOs
    for (int j = 0; j < NUM_EXT_IOS; j++) {
        digitalWrite(extIoPins[j], LOW);
    }
    digitalWrite(extIoPins[extIoIndex], HIGH);
    extIoIndex++;
    if (NUM_EXT_IOS <= extIoIndex) {
        extIoIndex = 0;
    }

    delay(500);
}
