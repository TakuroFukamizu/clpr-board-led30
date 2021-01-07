# Hardware Specs

## Extra IO Pin Assigns

拡張用のオプションIOピンのピンアサインについて

| Ref |     Function      |
| --- | ----------------- |
| G21 | G21, I2C SDA      |
| G22 | G22, I2C SCL      |
| G25 | G25, DAC1         |
| G33 | G33, ADC5, TOUCH8 |
| G18 | G18, SPI SCK      |
| G19 | G19, SPI MISO     |
| G23 | G23, SPI MOSI     |

## Schematics

各基板の回路図について

- [Main board](./main-board-sch.pdf)

## Board layout

各基板のレイアウトについて

|       Kind       |           Top view           |           Bottom View           |
| ---------------- | ---------------------------- | ------------------------------- |
| Main board       | ![](./res/main-top.png)      | ![](./res/main-bottom.png)      |
| MCU board(ESP32) | ![](./res/mcu-esp32-top.png) | ![](./res/mcu-esp32-bottom.png) |