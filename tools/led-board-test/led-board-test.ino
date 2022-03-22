#include <M5Unified.h>
#include <FastLED.h>

#define PIN_3V3 36 
#define PIN_LEDS 26
#define NUM_LEDS 30

CRGB leds[NUM_LEDS];
int ledIndex = 0;
int ledColorIndex = 0;

//------
static constexpr int MAX_VOLT = 5; // 5.0v
static constexpr float deg_to_rad = 0.017453292519943295769236907684886;
static constexpr int TFT_GREY = 0x5AEB;
static constexpr int LOOP_PERIOD = 35; // Display updates every 35 ms
static constexpr float A = 0.8f;

int value = 0;

int meter_height;
int needle_x;
int needle_y;
int needle_r;
int osx, osy; // Saved x & y coords
float rc = 0.0f;

void plotNeedle(int value);
void analogMeter();
//------

void setup() {
  // M5Stack Setup
  auto cfg = M5.config();
  cfg.internal_rtc  = false;
  cfg.internal_imu  = false;
  cfg.internal_mic  = false;
  cfg.led_brightness = 128;   // default= 0. system LED brightness (0=off / 255=max)
  M5.begin(cfg);

  delay(500);

  FastLED.addLeds<NEOPIXEL, PIN_LEDS>(leds, NUM_LEDS);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();

  // analogSetAttenuation(ADC_6db);  //ATT -6dB
  pinMode(PIN_3V3, ANALOG);

  // init display
  M5.Display.setEpdMode(epd_mode_t::epd_fastest); // fastest but very-low quality.

  if (M5.Display.width() < M5.Display.height())
  { /// Landscape mode.
    M5.Display.setRotation(M5.Display.getRotation() ^ 1);
  }
  
  M5.Display.startWrite();
  M5.Display.setTextSize(1);
  M5.Display.fillScreen(TFT_BLACK);

//  if (M5.Display.width() == 0 || M5.Display.height() == 0) {
//    Serial.println("ERROR: invalid display size");
//    return;
//  }
//  Serial.print("size: ");
//  Serial.print(M5.Display.width());
//  Serial.print(", ");
//  Serial.println(M5.Display.height());

//  meter_height = M5.Display.height() * 2 / 5;
  meter_height = M5.Display.height();
  needle_x = M5.Display.width() / 2;
  needle_y = (meter_height*2 + M5.Display.width()) / 3;
  osx = needle_x;
  osy = needle_y;

  analogMeter(); // Draw analogue meter
  M5.Display.endWrite();
  Serial.println("initialized.");
}

void loop() {
  
  // Loop RGB LED
  switch(ledColorIndex) {
    case 0: leds[ledIndex] = CRGB::Red; break;
    case 1: leds[ledIndex] = CRGB::Green; break;
    case 2: leds[ledIndex] = CRGB::Blue; break;
    case 3: leds[ledIndex] = CRGB::White; break;
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
  if (4 <= ledColorIndex) {
    ledColorIndex = 0;
  }

  //-----------

  // 3.3v LDO check
  int volt = analogReadMilliVolts(PIN_3V3);
  volt = volt * 2; // 分圧抵抗で下げた分
  rc = A * rc + (1-A) * (float)volt;

  Serial.printf("%d,%f\n", volt, rc);

  M5.Display.startWrite();
  plotNeedle(roundf(rc)); // min:0, max:MAX_VOLT
  M5.Display.endWrite();
  
  M5.update();
  delay(100);
}

//------
void analogMeter()
{
  M5.Display.fillRect(0, 0, M5.Display.width()   , meter_height  , TFT_WHITE);
  M5.Display.drawRect(1, 1, M5.Display.width()-2 , meter_height-2, TFT_BLACK);

  int r3 = needle_y * 13 / 15;
  int r2 = needle_y * 12 / 15;
  int r1 = needle_y * 11 / 15;
  needle_r = r1 - 3;
  M5.Display.fillArc(needle_x, needle_y, r1, r3, 270, 292, TFT_GREEN);
  M5.Display.fillArc(needle_x, needle_y, r1, r3, 292, 315, TFT_ORANGE);
  M5.Display.fillArc(needle_x, needle_y, r1, r1, 225, 315, TFT_BLACK);

  M5.Display.setTextColor(TFT_BLACK);
  M5.Display.setFont(&fonts::Font2);
  M5.Display.setTextDatum(textdatum_t::bottom_center);
  for (int i = 0; i <= 20; i++)
  {
    float v = map(i, 0, 20, 0, MAX_VOLT); // Map value to angle
    M5.Display.fillArc(needle_x, needle_y, r1, (i % 5) ? r2 : r3, 225 + i * 4.5f, 225 + i * 4.5f, TFT_BLACK);
    if (0 == (i % 5) && M5.Display.width() > 100)
    {
      M5.Display.drawNumber(
        v, 
        needle_x - cosf((45+i*4.5) * deg_to_rad) * r3, 
        needle_y - sinf((45+i*4.5) * deg_to_rad) * r3 - 2);
    }
  }
}

void plotNeedle(int value)
{
  float sdeg = map(value, 0, (MAX_VOLT * 1000), -135, -45); // Map value to angle
  // Calcualte tip of needle coords
  float sx = cosf(sdeg * deg_to_rad);
  float sy = sinf(sdeg * deg_to_rad);

  M5.Display.setClipRect(0, 0, M5.Display.width(), meter_height - 5);
  // Erase old needle image
  M5.Display.drawLine(needle_x - 1, needle_y, osx - 1, osy, TFT_WHITE);
  M5.Display.drawLine(needle_x + 1, needle_y, osx + 1, osy, TFT_WHITE);
  M5.Display.drawLine(needle_x    , needle_y, osx    , osy, TFT_WHITE);

  // Re-plot text under needle
  M5.Display.setTextColor(TFT_BLACK);
  if (M5.Display.width() > 100)
  {
    M5.Display.setFont(&fonts::Font4);
    M5.Display.setTextDatum(textdatum_t::middle_center);
    M5.Display.drawString("V", needle_x, needle_y>>1);
  }

//  M5.Display.setFont(&fonts::Font2);
//  M5.Display.setTextDatum(textdatum_t::bottom_left);
//  M5.Display.drawString("%RH", M5.Display.width() - 36, meter_height - 8);

  M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
  M5.Display.setTextDatum(textdatum_t::bottom_right);
  float v = (float)value/1000.0f;
  char buf1[5];
  char buf2[10];
  dtostrf(v, 1, 2, buf2);
  sprintf(buf1, "%sV", buf2);
  M5.Display.drawString(buf1, 68, meter_height - 8);
//  M5.Display.drawNumber(value, 36, meter_height - 8);


  // Store new needle end coords for next erase
  osx = roundf(sx * needle_r) + needle_x;
  osy = roundf(sy * needle_r) + needle_y;

  // Draw the needle in the new postion, magenta makes needle a bit bolder
  // draws 3 lines to thicken needle
  M5.Display.drawLine(needle_x - 1, needle_y, osx - 1, osy, TFT_RED);
  M5.Display.drawLine(needle_x + 1, needle_y, osx + 1, osy, TFT_RED);
  M5.Display.drawLine(needle_x    , needle_y, osx    , osy, TFT_MAGENTA);

  M5.Display.clearClipRect();
}
