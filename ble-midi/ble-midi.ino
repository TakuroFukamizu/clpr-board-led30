#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <Arduino.h>
#include <BLEMidi.h>
#include <FastLED.h>

/**
 * 必要なArduinoライブラリ
 * ESP32-BLE_MIDI
 * https://github.com/max22-/ESP32-BLE-MIDI
 * */

#define PIN_LEDS 27
#define NUM_LEDS 30
#define MAX_BRIGHTNESS 255   /* Control the brightness of your leds */
#define SATURATION 128   /* Control the saturation of your leds */
#define MAX_HUE 360
#define MAX_PERFORMANCE_STEP 10

#define MIDI_CC_DURATION 2
#define MIDI_CC_COLOR_HUE 3
#define MIDI_VALUE_MAX 127

//-------------------------------
uint8_t brightness = MAX_BRIGHTNESS;
CRGB leds[NUM_LEDS];
int hue;
uint8_t duration = 100;
uint16_t latestNoteOnTimestamp = 0;
uint16_t currentNoteOnTimestamp = 0;
bool isPlaying = false;
uint8_t playIndex = 0;

//-------------------------------

void connected();
void onNoteOn(uint8_t channel, uint8_t note, uint8_t velocity, uint16_t timestamp);
void onNoteOff(uint8_t channel, uint8_t note, uint8_t velocity, uint16_t timestamp);
void onControlChange(uint8_t channel, uint8_t controller, uint8_t value, uint16_t timestamp);

void performanceTask(void *pvParameters);

//-------------------------------

void setup() {
    Serial.begin(115200);
    BLEMidiServer.begin("CLPR-LED32-BLEMIDI");
    BLEMidiServer.setOnConnectCallback(connected);
    BLEMidiServer.setOnDisconnectCallback([](){     // To show how to make a callback with a lambda function
        Serial.println("Disconnected"); 
    });
    BLEMidiServer.setNoteOnCallback(onNoteOn);
    BLEMidiServer.setNoteOffCallback(onNoteOff);
    BLEMidiServer.setControlChangeCallback(onControlChange);
    //BLEMidiServer.enableDebugging();
    
    xTaskCreatePinnedToCore(
        performanceTask,
        "performanceTask",
        8192,
        NULL,
        1,
        NULL,
        0
    );
}

void loop() {
    if (BLEMidiServer.isConnected()) {
        // BLEMidiServer.noteOn(0, 69, 127);
        // delay(1000);
        // BLEMidiServer.noteOff(0, 69, 127);
        // delay(1000);
        Serial.println("connected");
    }
    delay(1000);
    isPlaying = true;
    latestNoteOnTimestamp = 10;
    brightness = 128;
    delay(10000);
}

//-------------------------------

void connected()
{
  Serial.println("Connected");
}

void onNoteOn(uint8_t channel, uint8_t note, uint8_t velocity, uint16_t timestamp) {
    Serial.printf("Received note on : channel %d, note %d, velocity %d (timestamp %dms)\n", channel, note, velocity, timestamp);
    brightness = (uint8_t)velocity * MAX_BRIGHTNESS / MIDI_VALUE_MAX;
    latestNoteOnTimestamp = timestamp;
    isPlaying = true;
}

void onNoteOff(uint8_t channel, uint8_t note, uint8_t velocity, uint16_t timestamp)
{
    Serial.printf("Received note off : channel %d, note %d, velocity %d (timestamp %dms)\n", channel, note, velocity, timestamp);
    if (isPlaying) {
        isPlaying = false;
        playIndex = 0;
    }
}

void onControlChange(uint8_t channel, uint8_t controller, uint8_t value, uint16_t timestamp)
{
    Serial.printf("Received control change : channel %d, controller %d, value %d (timestamp %dms)\n", channel, controller, value, timestamp);
    switch (controller) {
        case MIDI_CC_DURATION:
            duration = ((uint8_t)value * 100 / MIDI_VALUE_MAX) + 10;
            break;
        case MIDI_CC_COLOR_HUE:
            hue = (uint8_t)value * MAX_HUE / MIDI_VALUE_MAX;
            break;
    }
}

void performanceTask(void *pvParameters) {
    while(1){
        if (isPlaying) {
            if (currentNoteOnTimestamp != latestNoteOnTimestamp) { // 新着noteOn有り = 今の演出をキャンセル
                playIndex = 0;
                currentNoteOnTimestamp = latestNoteOnTimestamp;
            }
            for (int i = 0; i < NUM_LEDS; i++)
            {
                // TODO: playIndexに応じた処理
                leds[i] = CHSV(hue, brightness, SATURATION);
            }
            FastLED.show();

            playIndex++;
            if (MAX_PERFORMANCE_STEP <= playIndex) { // 最後のstep = 演出終了
                isPlaying = false; 
            }
            vTaskDelay(duration);
        } else {
            vTaskDelay(10);
        }
    }
}
