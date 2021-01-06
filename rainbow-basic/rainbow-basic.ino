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

CRGB leds[NUM_LEDS];

void setup() { 
    FastLED.addLeds<NEOPIXEL, PIN_LEDS>(leds, NUM_LEDS); 
}

void loop() {
    for (int j = 0; j < 255; j++) {
        for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = CHSV(i - (j * 2), BRIGHTNESS, SATURATION); /* The higher the value 4 the less fade there is and vice versa */ 
        }
        FastLED.show();
        delay(50); /* Change this to your hearts desire, the lower the value the faster your colors move (and vice versa) */
    }
}