# About

This project is a game port to the RP2040 / Raspberry Pico microcontroller of an MS-DOS Game.

Current status is alpha. 90% of the core mechanic has been implemented.

## Original MS-DOS

![Image](images/Screenshot_2022-10-19_21-01-20.png 'icon')

## RP2040 / Raspberry Pico

![Image](images/20230606_231307_crop50.jpg 'icon')

I am also releasing the source code for the map editor:
https://github.com/cfrankb/cs3-map-edit

# Software required

RP2040 SDK Pico

# Build and Installation

```Shell
git clone git@github.com:cfrankb/cs3_rp2040_pico.git
cd cs3_rp2040_pico
mkdir -p build
cd build
cmake ..
make
```
Copy the UF2 file to your Pico.

# Tested with this hardware

| Driver  | Resolution | Max SPI Freq | Tested |
| ------- | ---------- | ------------ | ----------- |
| ST7789  | 240x320    | 20M          | Not tested      |
| ILI9341 | 240x320    | 40M          | Works      |

## ILI9341

https://www.aliexpress.com/item/33040270372.html

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


