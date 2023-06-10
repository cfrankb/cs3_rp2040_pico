#ifndef __CONFIG
#define __CONFIG

#include <stdint.h>
#include "hardware/spi.h"

typedef struct {
    spi_inst_t *port;
    uint pin_miso;
    uint pin_cs;
    uint pin_sck;
    uint pin_mosi;
    uint pin_reset;
    uint pin_dc;
    uint pin_led; // BL 
} ili9341_config_t;

typedef struct {
    uint pin_vrx;
    uint pin_vry;
    uint pin_sw;
} joystick_module_config_t;

#endif