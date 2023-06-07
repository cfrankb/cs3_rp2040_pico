# About

This project is a game port to RP2040 / Pico microcontroller of an MS-DOS Game.

Current status is alpha. 90% of the core mechanic has been implemented.

## Original MS-DOS

![Image](images/Screenshot_2022-10-19_21-01-20.png 'icon')

## RP2040 / Pico

![Image](images/20230606_224428_crop50.jpg 'icon')

I am also releasing the source code for the map editor:
https://github.com/cfrankb/cs3-map-edit

# Software required

RP2040 SDK Pico

# Installation

This project is based in part on: https://github.com/nopnop2002/esp-idf-st7789

```Shell
git clone https://github.com/cfrankb/esp-idf-cs3.git
cd esp-idf-cs3/
idf.py set-target {esp32/esp32s2/esp32s3/esp32c3}
idf.py menuconfig
idf.py flash
```

# Tested with this hardware

| Driver  | Resolution | Max SPI Freq | Display Inv |
| ------- | ---------- | ------------ | ----------- |
| ST7789  | 240x320    | 20M          | True        |
| ILI9341 | 240x320    | 40M          | False       |

## ST7789

https://www.aliexpress.com/item/1005004343881408.html

| ST7789   | wirecolor | GPIO Pin   |
| -------- | --------- | ---------- |
| SDK/MISO | purple    | 4          |
| LED / BL | grey      | 1          |
| SCK      | yellow    | 6          |
| SDI/MOSI | orange    | 7          |
| DC       | green     | 9          |
| RESET    | brown     | 8          |
| CS       | white     | 5          |
| GRN      | black     | GRN        |
| VCC      | red       | 3.3v or 5v |

## Joystick

https://www.aliexpress.com/item/32345829014.html

| JOYSTICK | wirecolor | GPIO Pin |
| -------- | --------- | -------- |
| VRX      | brown     | 26       |
| VRY      | white     | 27       |
| SW       | blue      | 28       |
| GRN      | black     | GRN      |
| VCC      | red       | 3.3v     |

## Button Module (optional)

| Module | wirecolor | GPIO Pin  |
| ------ | --------- | --------- |
| GRN    | black     | GRN       |
| OUT    | green     | 4         |
| VCC    | red       | 3.3v / 5v |

## Further reading

https://github.com/DiegoPaezA/ESP32-freeRTOS
