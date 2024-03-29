#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "ili9341.h"
#include "buffer.h"
#include "config.h"

/*
 
 (pin 1) VCC        5V/3.3V power input
 (pin 2) GND        Ground
 (pin 3) CS         LCD chip select signal, low level enable
 (pin 4) RESET      LCD reset signal, low level reset
 (pin 5) DC/RS      LCD register / data selection signal; high level: register, low level: data
 (pin 6) SDI(MOSI)  SPI bus write data signal
 (pin 7) SCK        SPI bus clock signal
 (pin 8) LED        Backlight control; if not controlled, connect 3.3V always bright
 (pin 9) SDO(MISO)  SPI bus read data signal; optional

 */

extern const ili9341_config_t ili9341_config;
extern const display_config_t display_config;

#define DC_CMD 0
#define DC_DATA 1

static inline void cs_select() {
    asm volatile("nop \n nop \n nop");
    gpio_put(ili9341_config.pin_cs, 0);  // Active low
    asm volatile("nop \n nop \n nop");
}

static inline void cs_deselect() {
    asm volatile("nop \n nop \n nop");
    gpio_put(ili9341_config.pin_cs, 1);
    asm volatile("nop \n nop \n nop");
}

void ili9341_set_command(uint8_t cmd) {
    cs_select();
    gpio_put(ili9341_config.pin_dc, DC_CMD);
    spi_write_blocking(ili9341_config.port, &cmd, 1);
    gpio_put(ili9341_config.pin_dc, DC_DATA);
    cs_deselect();
}

void ili9341_command_param(uint8_t data) {
    cs_select();
    spi_write_blocking(ili9341_config.port, &data, 1);
    cs_deselect();
}

inline void ili9341_start_writing() {
    cs_select();
}

void ili9341_write_data(const uint8_t *buffer, size_t bytes) {
    cs_select();
    spi_write_blocking(ili9341_config.port, buffer, bytes);
    cs_deselect();
}

void ili9341_write_data_continuous(const uint8_t *buffer, int bytes) {
    spi_write_blocking(ili9341_config.port, buffer, bytes);
}

inline void ili9341_stop_writing() {
    cs_deselect();
}

static bool spi_master_write_comm_byte(uint8_t cmd)
{
    static uint8_t Byte = 0;
    Byte = cmd;
    //gpio_set_level(dev->_dc, SPI_Command_Mode);
    cs_select();
    gpio_put(ili9341_config.pin_dc, DC_CMD);
    int s = spi_write_blocking(ili9341_config.port, &Byte, 1);
    cs_deselect();
    if (s != 1) {
        printf("spi_master_write_comm_byte return %d. expecting: 1", s);
    }
    return s == 1;
}

static bool spi_master_write_data_byte(uint8_t data)
{
    static uint8_t Byte = 0;
    Byte = data;
    cs_select();
    gpio_put(ili9341_config.pin_dc, DC_DATA);
    int s  = spi_write_blocking(ili9341_config.port, &Byte, 1);
    cs_deselect();
    if (s != 1) {
        printf("spi_master_write_data_byte return %d. expecting: 1", s);
    }
    return s == 1;
}

bool spi_master_write_addr( uint16_t addr1, uint16_t addr2)
{
    static uint8_t Byte[4];
    Byte[0] = (addr1 >> 8) & 0xFF;
    Byte[1] = addr1 & 0xFF;
    Byte[2] = (addr2 >> 8) & 0xFF;
    Byte[3] = addr2 & 0xFF;
    cs_select();
    gpio_put(ili9341_config.pin_dc, DC_DATA);
    int s  = spi_write_blocking(ili9341_config.port, Byte, 4);
    cs_deselect();
    return s;
}

bool spi_master_write_color( uint16_t color, uint16_t size)
{
    uint16_t line[1024];
    int index = 0;
    for (int i = 0; i < size; i++)
    {
        line[i] = color;
    }
    cs_select();
    gpio_put(ili9341_config.pin_dc, DC_DATA);
    int s  = spi_write_blocking(ili9341_config.port, reinterpret_cast<uint8_t*>(line), 2 * size);
    cs_deselect();
    return s;
}


bool spi_master_write_colors( uint16_t *colors, uint16_t size)
{ 
    cs_select();
    gpio_put(ili9341_config.pin_dc, DC_DATA);

    int s  = spi_write_blocking(ili9341_config.port, reinterpret_cast<uint8_t*>(colors), 2 * size);
    cs_deselect();
    return s;
}


// Draw rectangle of filling
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End X coordinate
// y2:End Y coordinate
// color:color
void ili9341_lcdDrawFillRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    const uint16_t width = display_config.width;
    const uint16_t height = display_config.height;

    if (x1 >= width)
        return;
    if (x2 >= width)
        x2 = width - 1;
    if (y1 >= height)
        return;
    if (y2 >= height)
        y2 = height - 1;

    spi_master_write_comm_byte( 0x2A); // set column(x) address
    spi_master_write_addr(x1, x2);
    spi_master_write_comm_byte( 0x2B); // set Page(y) address
    spi_master_write_addr(y1, y2);
    spi_master_write_comm_byte( 0x2C); // Memory Write
    for (int i = y1; i <= y2; i++)
    {
        uint16_t size = x2 - x1 + 1;
        spi_master_write_color(color, size);
    }
     // endif 0x9340/0x9341/0x7796
}


// Draw rectangle of filling
// x1:Start X coordinate
// y1:Start Y coordinate
// tile: ptr to 16x16 tile
void ili9341_lcdDrawTile(uint16_t x1, uint16_t y1, uint16_t *tile)
{
    const uint16_t width = display_config.width;
    const uint16_t height = display_config.height;

    uint16_t x2 = x1 + 16 - 1;
    uint16_t y2 = y1 + 16 - 1;

    if (x1 >= width)
        return;

    if (x2 >= width)
        x2 = width - 1;
    if (y1 >= height)
        return;
    if (y2 >= height)
        y2 = height - 1;

    spi_master_write_comm_byte( 0x2A); // set column(x) address
    spi_master_write_addr(x1, x2);
    spi_master_write_comm_byte( 0x2B); // set Page(y) address
    spi_master_write_addr(y1, y2);
    spi_master_write_comm_byte( 0x2C); // Memory Write
    spi_master_write_colors(tile, 256);
     // endif 0x9340/0x9341/0x7796
}


void ili9341_lcdInit()
{
    spi_master_write_comm_byte(0xC0); // Power Control 1
    spi_master_write_data_byte(0x23);

    spi_master_write_comm_byte(0xC1); // Power Control 2
    spi_master_write_data_byte(0x10);

    spi_master_write_comm_byte(0xC5); // VCOM Control 1
    spi_master_write_data_byte(0x3E);
    spi_master_write_data_byte(0x28);

    spi_master_write_comm_byte(0xC7); // VCOM Control 2
    spi_master_write_data_byte(0x86);

    spi_master_write_comm_byte(0x36); // Memory Access Control
    spi_master_write_data_byte(0x08); // Right top start, BGR color filter panel
    // spi_master_write_data_byte(0x00);//Right top start, RGB color filter panel

    spi_master_write_comm_byte(0x3A); // Pixel Format Set
    spi_master_write_data_byte(0x55); // 65K color: 16-bit/pixel

    spi_master_write_comm_byte(0x20); // Display Inversion OFF

    spi_master_write_comm_byte(0xB1); // Frame Rate Control
    spi_master_write_data_byte(0x00);
    spi_master_write_data_byte(0x18);

    spi_master_write_comm_byte(0xB6); // Display Function Control
    spi_master_write_data_byte(0x08);
    spi_master_write_data_byte(0xA2); // REV:1 GS:0 SS:0 SM:0
    spi_master_write_data_byte(0x27);
    spi_master_write_data_byte(0x00);

    spi_master_write_comm_byte(0x26); // Gamma Set
    spi_master_write_data_byte(0x01);

    spi_master_write_comm_byte(0xE0); // Positive Gamma Correction
    spi_master_write_data_byte(0x0F);
    spi_master_write_data_byte(0x31);
    spi_master_write_data_byte(0x2B);
    spi_master_write_data_byte(0x0C);
    spi_master_write_data_byte(0x0E);
    spi_master_write_data_byte(0x08);
    spi_master_write_data_byte(0x4E);
    spi_master_write_data_byte(0xF1);
    spi_master_write_data_byte(0x37);
    spi_master_write_data_byte(0x07);
    spi_master_write_data_byte(0x10);
    spi_master_write_data_byte(0x03);
    spi_master_write_data_byte(0x0E);
    spi_master_write_data_byte(0x09);
    spi_master_write_data_byte(0x00);

    spi_master_write_comm_byte(0xE1); // Negative Gamma Correction
    spi_master_write_data_byte(0x00);
    spi_master_write_data_byte(0x0E);
    spi_master_write_data_byte(0x14);
    spi_master_write_data_byte(0x03);
    spi_master_write_data_byte(0x11);
    spi_master_write_data_byte(0x07);
    spi_master_write_data_byte(0x31);
    spi_master_write_data_byte(0xC1);
    spi_master_write_data_byte(0x48);
    spi_master_write_data_byte(0x08);
    spi_master_write_data_byte(0x0F);
    spi_master_write_data_byte(0x0C);
    spi_master_write_data_byte(0x31);
    spi_master_write_data_byte(0x36);
    spi_master_write_data_byte(0x0F);

    spi_master_write_comm_byte(0x11); // Sleep Out
    sleep_ms(120);

    spi_master_write_comm_byte(0x29); // Display ON
}

void ili9341_lcdInit0() {
    ili9341_set_command(0x01);//soft reset
    sleep_ms(100);

    ili9341_set_command(ILI9341_GAMMASET);
    ili9341_command_param(0x01);

    // positive gamma correction
    ili9341_set_command(ILI9341_GMCTRP1);
    const uint8_t data0[15] = { 0x0f, 0x31, 0x2b, 0x0c, 0x0e, 0x08, 0x4e, 0xf1, 0x37, 0x07, 0x10, 0x03, 0x0e, 0x09, 0x00 };
    ili9341_write_data(data0, sizeof(data0));

    // negative gamma correction
    ili9341_set_command(ILI9341_GMCTRN1);
    const uint8_t data1[15] ={ 0x00, 0x0e, 0x14, 0x03, 0x11, 0x07, 0x31, 0xc1, 0x48, 0x08, 0x0f, 0x0c, 0x31, 0x36, 0x0f };
    ili9341_write_data(data1, 15);

    // memory access control
    ili9341_set_command(ILI9341_MADCTL);
    ili9341_command_param(0x48);

    // pixel format
    ili9341_set_command(ILI9341_PIXFMT);
    ili9341_command_param(0x55);  // 16-bit

    // frame rate; default, 70 Hz
    ili9341_set_command(ILI9341_FRMCTR1);
    ili9341_command_param(0x00);
    ili9341_command_param(0x1B);

    // exit sleep
    ili9341_set_command(ILI9341_SLPOUT);

    // display on
    ili9341_set_command(ILI9341_DISPON);

    //
/*
    // column address set
    ili9341_set_command(ILI9341_CASET);
    ili9341_command_param(0x00);
    ili9341_command_param(0x00);  // start column
    ili9341_command_param(0x00);
    ili9341_command_param(0xef);  // end column -> 239

    // page address set
    ili9341_set_command(ILI9341_PASET);
    ili9341_command_param(0x00);
    ili9341_command_param(0x00);  // start page
    ili9341_command_param(0x01);
    ili9341_command_param(0x3f);  // end page -> 319

    ili9341_set_command(ILI9341_RAMWR);
*/

}

void ili9341_spi_init()
{
    // This example will use SPI0 at 0.5MHz.
    spi_init(ili9341_config.port, 500 * 1000);
    int baudrate = spi_set_baudrate(ili9341_config.port, 75000 * 1000);

    // init spi pins
    gpio_set_function(ili9341_config.pin_miso, GPIO_FUNC_SPI);
    gpio_set_function(ili9341_config.pin_sck, GPIO_FUNC_SPI);
    gpio_set_function(ili9341_config.pin_mosi, GPIO_FUNC_SPI);
}

void ili9341_init_gpio() 
{
   // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(ili9341_config.pin_cs);
    gpio_set_dir(ili9341_config.pin_cs, GPIO_OUT);
    gpio_put(ili9341_config.pin_cs, 0);

    // Reset is active-low
    gpio_init(ili9341_config.pin_reset);
    gpio_set_dir(ili9341_config.pin_reset, GPIO_OUT);
    gpio_put(ili9341_config.pin_reset, 1);

    // high = command, low = data
    gpio_init(ili9341_config.pin_dc);
    gpio_set_dir(ili9341_config.pin_dc, GPIO_OUT);
    gpio_put(ili9341_config.pin_dc, DC_CMD);

    sleep_ms(50);
    gpio_put(ili9341_config.pin_reset, 0);
    sleep_ms(50);
    gpio_put(ili9341_config.pin_reset, 1);
    sleep_ms(50);

    // LED/BL backlight
    if (ili9341_config.pin_led != -1) {
        gpio_init(ili9341_config.pin_led);
        gpio_set_dir(ili9341_config.pin_led, GPIO_OUT);
        gpio_put(ili9341_config.pin_led, 1);
    }
}

bool ili9341_drawBuffer(uint16_t x1, uint16_t y1, CBuffer &buffer)
{
    const uint16_t width = display_config.width;
    const uint16_t height = display_config.height;

    int len = buffer.len();
    int hei = buffer.hei(); 
    int size = len * hei;

    --len;
    if (x1 + len > width)
        return false;
    if (y1 + hei > height)
        return false;

    uint16_t x2 = x1 + len;
    uint16_t y2 = y1 + hei;

    spi_master_write_comm_byte( 0x2A); // set column(x) address
    spi_master_write_addr(x1, x2);
    spi_master_write_comm_byte( 0x2B); // set Page(y) address
    spi_master_write_addr(y1, y2);
    spi_master_write_comm_byte( 0x2C); // Memory Write
    spi_master_write_colors(buffer.buffer(), size);

    return true;
}

void ili9341_init() {

    // initialize the spi
    ili9341_spi_init();

    // initialize gpio pins
    ili9341_init_gpio();

    // lcd Init
    ili9341_lcdInit();
    //ili9341_lcdInit0();
}