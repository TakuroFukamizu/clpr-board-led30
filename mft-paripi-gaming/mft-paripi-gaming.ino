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

#define CM_INTERVAL 10000 /* LEDの演出を切り替える時間(10秒) */

//----------------------------------------------------------------

CRGB leds[NUM_LEDS];
int hue;
int brightness;
int saturation;
uint8_t gamingHueIndex = 0;
unsigned long previousMillis = 0;
uint8_t cmFlag = 0; // 0: paripi, 1: gaming

//----------------------------------------------------------------

void paripi();
void gaming();

//----------------------------------------------------------------

void setup() { 
    FastLED.addLeds<NEOPIXEL, PIN_LEDS>(leds, NUM_LEDS);
    randomSeed(analogRead(0)); 
}

void loop() {
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis >= CM_INTERVAL) {
        previousMillis = currentMillis;
        cmFlag = cmFlag == 0 ? 1 : 0; // モードフラグの切り替え(0 or 1)
    }
    switch (cmFlag)
    {
    case 0: // paripi
        paripi();
        break;
    case 1: // gaming
        gaming();
        break;
    default:
        paripi();
        break;
    }
    delay(100);
}

//----------------------------------------------------------------

void paripi() {
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
}

void gaming() {
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
}
