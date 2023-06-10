#include "config.h"

display_config_t display_config = {
    .width = 240,
    .height = 320
};

ili9341_config_t ili9341_config = {
	.port = spi0,
	.pin_miso = 4,
	.pin_cs = 5,        //spi0 csn
	.pin_sck = 6,       //spi0 sck
	.pin_mosi = 7,      //spi0 tx
	.pin_reset = 8,     //spi1 rx
	.pin_dc = 9,         //spi1 csn
    .pin_led = 1
};

joystick_module_config_t joystick_module_config {
    .pin_vrx = 26,
    .pin_vry = 27,
    .pin_sw = 28
};

