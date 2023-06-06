#include <stdio.h>
#include "pico/stdlib.h"
#include "ili9341.h"
#include "buffer.h"

#define SWAP_BYTES(color) ((uint16_t)(color >> 8) | (uint16_t)(color << 8))

void led_test() {
    stdio_init_all();
    const uint LED_PIN = 14;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    while (true) {
        printf("Blink %d\n", LED_PIN);
        gpio_put(LED_PIN, 1);
        sleep_ms(1000);
        printf("No Blink\n");
        gpio_put(LED_PIN, 0);
        sleep_ms(1000);
    }
}

int main1() {
    //setup_default_uart();
    stdio_init_all();
    printf("Hello, world!\n");

    const uint BTN1_PIN = 29;
    const uint BTN2_PIN = 28;
    const uint BTN3_PIN = 27;
    const uint BTN4_PIN = 26;

    gpio_init(BTN1_PIN);
    gpio_init(BTN2_PIN);
    gpio_init(BTN3_PIN);
    gpio_init(BTN4_PIN);

    gpio_set_dir(BTN1_PIN, GPIO_IN);
    gpio_set_dir(BTN2_PIN, GPIO_IN);
    gpio_set_dir(BTN3_PIN, GPIO_IN);
    gpio_set_dir(BTN4_PIN, GPIO_IN);
    //gpio_set_pulls(BTN1_PIN, false, true);
    //gpio_set_pulls(BTN2_PIN, false, true);
    //gpio_set_pulls(BTN3_PIN, false, false);
    //gpio_set_pulls(BTN4_PIN, true, false);

    while (true) {
      bool btn1 =  gpio_get(BTN1_PIN);
      bool btn2 =  gpio_get(BTN2_PIN);
      bool btn3 =  gpio_get(BTN3_PIN);
      bool btn4 =  gpio_get(BTN4_PIN);
      printf("btn: %d %d %d %d\n", btn1, btn2, btn3,btn4);
        sleep_ms(50);
    }
    return 0;
}

#define width  240
#define height 320
#define bufferSize  width * height
/*static uint16_t frameBuffer[bufferSize];

void fillBuffer(const uint16_t color) {
    for (int i=0; i < bufferSize; ++i) {
        frameBuffer[i] = color;
    }
}*/

const uint16_t palette[16] = {
    SWAP_BYTES(0x0000),
    SWAP_BYTES(0x49E5),
    SWAP_BYTES(0xB926),
    SWAP_BYTES(0xE371),
    SWAP_BYTES(0x9CF3),
    SWAP_BYTES(0xA324),
    SWAP_BYTES(0xEC46),
    SWAP_BYTES(0xF70D),
    SWAP_BYTES(0xffff),
    SWAP_BYTES(0x1926),
    SWAP_BYTES(0x2A49),
    SWAP_BYTES(0x4443),
    SWAP_BYTES(0xA664),
    SWAP_BYTES(0x02B0),
    SWAP_BYTES(0x351E),
    SWAP_BYTES(0xB6FD)
};

void setupPins() {
    const uint LED_PIN = 3;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, true);
}

extern uint16_t tiles_mcz;

extern "C" int main() {
    stdio_init_all();
    printf("Hello, world!\n");
    ili9341_init();
    ili9341_lcdDrawFillRect(0,0, width, height, 0);
    int i = 0;
    CBuffer buffer(240, 16);
    while (true) {
        //fillBuffer(palette[i]);
        //ili9341_write_data(frameBuffer, width * height * 2);
        //ili9341_lcdDrawFillRect(0,0, 128, 128,palette[i] );
        uint16_t *tiles = &tiles_mcz;
        int j =0;
        int cols = width / 16;
        int rows = height / 16;

        for (int y=0; y < rows; ++y) {
            for (int x=0; x < cols; ++x) {
                //ili9341_lcdDrawTile(x*16, y*16, tiles + 256 * j);
                buffer.drawTile(x*16, 0,tiles + 256 * j );
                ++j;
            }
            char txt[32];
            if (y == 0) {
                sprintf(txt, "tiles 0x%p", tiles);
                buffer.drawFont(0,4, txt, 0xffff);
            } else if (y == 1) {
                sprintf(txt, "buffer 0x%p", buffer.buffer());
                buffer.drawFont(0,4, txt, 0xffff);
            }
            ili9341_drawBuffer(0,y*16, buffer);
        }

        ++i;
        if (i > 15) {
            i = 0;
        }

//        buffer.drawFont(0,0, "hello world", 0xffff);
  //      ili9341_drawBuffer(0,0, buffer);

        sleep_ms(512);
        printf("i = %d\n", i);
    }
    return 0;
}