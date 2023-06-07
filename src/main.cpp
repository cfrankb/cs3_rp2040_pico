#include <stdio.h>
#include <vector>
#include <cstring>
#include "pico/stdlib.h"
#include "hardware/flash.h"
#include "ili9341.h"
#include "buffer.h"
#include "levelarch.h"
#include "map.h"
#include "game.h"
#include "engine.h"
#include "palette.h"
#include "joystick.h"

#define width  240
#define height 320
#define bufferSize  width * height

extern uint16_t tiles_mcz;
extern uint8_t levels_mapz;

uint storage_get_flash_capacity() {
    #define FLASH_RUID_CMD 0x4b
    #define FLASH_RUID_DUMMY_BYTES 4
    #define FLASH_RUID_DATA_BYTES 8
    #define FLASH_RUID_TOTAL_BYTES (1 + FLASH_RUID_DUMMY_BYTES + FLASH_RUID_DATA_BYTES)

    uint8_t txbuf[FLASH_RUID_TOTAL_BYTES] = {0x9f};
    uint8_t rxbuf[FLASH_RUID_TOTAL_BYTES] = {0};
    flash_do_cmd(txbuf, rxbuf, FLASH_RUID_TOTAL_BYTES);

    return 1 << rxbuf[3];
}

extern "C" int main1() {
    stdio_init_all();
    printf("Hello, world!\n");
    ili9341_init();
    ili9341_lcdDrawFillRect(0,0, width, height, 0);
    int i = 0;
    CBuffer buffer(240, 16);
    uint8_t *ptr = &levels_mapz;
    CLevelArch arch;
    arch.fromMemory(ptr);

    CMap map;
    map.fromMemory(ptr + arch.at(3));

    while (true) {
        //ili9341_write_data(frameBuffer, width * height * 2);
        //ili9341_lcdDrawFillRect(0,0, 128, 128,palette[i] );
        uint16_t *tiles = &tiles_mcz;
        int cols = width / 16;
        int rows = height / 16;

        for (int y=0; y < rows; ++y) {
            for (int x=0; x < cols; ++x) {
                //ili9341_lcdDrawTile(x*16, y*16, tiles + 256 * j);
                buffer.drawTile(x*16, 0,tiles + 256 * map.at(x,y));
                //++j;
            }
            char txt[32];
            if (y == 0) {
                sprintf(txt, "tiles 0x%p", tiles);
                buffer.drawFont(0,4, txt, 0xffff);
            } else if (y == 1) {
                sprintf(txt, "buffer 0x%p", buffer.buffer());
                buffer.drawFont(0,4, txt, 0xffff);
            } else if (y==2) {
                sprintf(txt, "0x%x", storage_get_flash_capacity());
                buffer.drawFont(0,4, txt, 0xffff);
            }
            ili9341_drawBuffer(0,y*16, buffer);
        }

//        buffer.drawFont(0,0, "hello world", 0xffff);
  //      ili9341_drawBuffer(0,0, buffer);
        sleep_ms(512);
    }
    return 0;
}

CGame game;

extern "C" int main() {
    stdio_init_all();
    printf("Hello, world!\n");

    CEngine * engine = game.getEngine();

    game.init();
    game.loadLevel();
    game.nextLevel();

    engine->fill(LIME);
    game.setMode(CGame::MODE_LEVEL);
    uint32_t ticks = 0;

    while (1)
    {
//        vTaskDelay(40 / portTICK_PERIOD_MS);
        engine->drawScreen();

        if (game.mode() != CGame::MODE_LEVEL)
        {
            continue;
        }

        if (ticks % 3 == 0)
        {
            game.managePlayer();
        }

        if (ticks % 3 == 0)
        {
            //game.animate();
            engine->animate();
        }

        if (ticks % 4 == 0)
        {
            game.manageMonsters();
        }
        ++ticks;
       
        uint16_t joy =engine->readJoystick();
        if (game.goalCount() == 0 || joy & JOY_A_BUTTON)
        {
            game.nextLevel();
        }
        
    }    

    return 0;
}