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


extern "C" int main() {
    stdio_init_all();
    printf("Hello, world!\n");

    CGame game;
    CEngine * engine = game.getEngine();

    game.init();
    game.loadLevel(false);

    uint32_t ticks = 0;

    while (1)
    {
        switch (game.mode())
        {
        case CGame::MODE_INTRO:
        case CGame::MODE_RESTART:
        case CGame::MODE_GAMEOVER:
            engine->drawLevelIntro();
            sleep_ms(2000);
            game.setMode(CGame::MODE_LEVEL);
            if (game.mode()== CGame::MODE_GAMEOVER) {
                game.restartGane();
            }
            break;
        case CGame::MODE_LEVEL:
            engine->drawScreen();
        }

  //      if (game.mode() != CGame::MODE_LEVEL)
   //     {
     //       continue;
      //  }

        if (ticks % 3 == 0 && !game.isPlayerDead())
        {
            game.managePlayer();
        }

        if (ticks % 3 == 0)
        {
            engine->animate();
        }

        if (ticks % 4 == 0)
        {
            game.manageMonsters();
        }

        if (game.isPlayerDead() && !game.isGameOver()) {
            sleep_ms(500);
            game.killPlayer();
            game.restartLevel();
        }

        ++ticks;
       
        uint16_t joy =engine->readJoystick();
        if (!game.isGameOver()) {
            if (game.goalCount() == 0 || joy & JOY_A_BUTTON)
            {
                game.nextLevel();
            }
        } else {
            if (joy & JOY_BUTTON) {
            }
        }
    }

    return 0;
}