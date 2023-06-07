#include <cstring>
#include <stdio.h>
#include "engine.h"
#include "buffer.h"
#include "tilesdata.h"
#include "animzdata.h"
#include "game.h"
#include "ili9341.h"
#include "palette.h"
#include "hardware/adc.h"
#include "joystick.h"

#define TILESIZE 16
#define CONFIG_WIDTH 240
#define CONFIG_HEIGHT 320

extern uint16_t tiles_mcz;
extern uint16_t animz_mcz;
extern uint16_t annie_mcz;
extern uint8_t levels_mapz;

typedef struct
{
    uint8_t srcTile;
    uint8_t startSeq;
    uint8_t count;
    uint8_t index;
} AnimzSeq;

AnimzSeq animzSeq[] = {
    {TILES_DIAMOND, ANIMZ_DIAMOND, 13, 0},
    {TILES_INSECT1, ANIMZ_INSECT1, 2, 0},
    {TILES_SWAMP, ANIMZ_SWAMP, 2, 0},
    {TILES_ALPHA, ANIMZ_ALPHA, 2, 0},
    {TILES_FORCEF94, ANIMZ_FORCEF94, 8, 0},
    {TILES_VAMPLANT, ANIMZ_VAMPLANT, 2, 0},
    {TILES_ORB, ANIMZ_ORB, 4, 0},
    {TILES_TEDDY93, ANIMZ_TEDDY93, 2, 0},
    {TILES_LUTIN, ANIMZ_LUTIN, 2, 0},
    {TILES_OCTOPUS, ANIMZ_OCTOPUS, 2, 0},
    {TILES_TRIFORCE, ANIMZ_TRIFORCE, 4, 0},
    {TILES_YAHOO, ANIMZ_YAHOO, 2, 0},
    {TILES_YIGA, ANIMZ_YIGA, 2, 0},
    {TILES_YELKILLER, ANIMZ_YELKILLER, 2, 0},
    {TILES_MANKA, ANIMZ_MANKA, 2, 0},
   // {TILES_MAXKILLER, ANIMZ_MAXKILLER, 1, 0},
   // {TILES_WHTEWORM, ANIMZ_WHTEWORM, 2, 0},
};

uint8_t tileReplacement[256];
CBuffer buffer(CONFIG_WIDTH, TILESIZE);

CEngine::CEngine(CGame *game)
{
    m_game = game;
    memset(tileReplacement, NO_ANIMZ, sizeof(tileReplacement));
}

CEngine::~CEngine()
{
}

bool CEngine::init()
{
    ili9341_init();
    fill(YELLOW);
    initJoystick();
    return true;
}

void CEngine::fill(uint16_t color) 
{
    ili9341_lcdDrawFillRect(0,0, CONFIG_WIDTH, CONFIG_HEIGHT, color);
}

void CEngine::drawBuffer(int x, int y)
{
    ili9341_drawBuffer(x, y, buffer);
}

void CEngine::drawLevelIntro()
{
    char t[32];
    sprintf(t, "LEVEL %.2d", m_game->m_level + 1);

    int x = (CONFIG_WIDTH - strlen(t) * 8) / 2;
    int y = (CONFIG_HEIGHT - 8) / 2;
//    display.fill(BLACK);
    fill(BLACK);
    buffer.fill(BLACK);
    buffer.drawFont(x, 0, t, WHITE);
    drawBuffer(0, y);
}

void CEngine::drawScreen()
{
    //std::lock_guard<std::mutex> lk(g_mutex);

    CGame & game = *m_game;
    CMap & map = game.getMap();

    const int cols = CONFIG_WIDTH / TILESIZE;
    const int rows = CONFIG_HEIGHT / TILESIZE;
    const int lmx = std::max(0, game.m_player.getX() - cols / 2);
    const int lmy = std::max(0, game.m_player.getY() - rows / 2);
    const int mx = std::min(lmx, map.len() > cols ? map.len() - cols : 0);
    const int my = std::min(lmy, map.hei() > rows ? map.hei() - rows : 0);

    uint16_t *tiles = &tiles_mcz;
    uint16_t *animzTiles = &animz_mcz;
    uint16_t *playerTiles = &annie_mcz;

    uint16_t *tiledata;
    for (int y = 0; y < rows; ++y)
    {
        if (y + my >= map.hei())
        {
            break;
        }

        for (int x = 0; x < cols; ++x)
        {
            int i = map.at(x + mx, y + my);
            if (i == TILES_ANNIE2)
            {
                tiledata = playerTiles + 256 * (game.m_player.getAim() * 4 + x % 3);
            }
            else
            {
                if (i == TILES_STOP)
                {
                    i = TILES_BLANK;
                }
                int j = tileReplacement[i];
                tiledata = j == NO_ANIMZ ? tiles+ i * 256 : animzTiles + j * 256;
            }
            buffer.drawTile(x * TILESIZE, 0, tiledata);
        }

        char tmp[32];
        if (y == 0)
        {
            int bx = 0;
            sprintf(tmp, "SCORE %.8d ", game.m_score);
            buffer.drawFont(0, 0, tmp, WHITE);
            bx += strlen(tmp);
            sprintf(tmp, "DIAMONDS %.2d", game.m_diamonds);
            buffer.drawFont(bx * 8, 0, tmp, YELLOW);
        }
        else if (y ==1) {
            uint16_t joy = readJoystick();
        }
        else if (y == rows - 1)
        {
            buffer.drawRect(
                Rect{.x = 4, .y = 4, .width = std::min(game.m_health / 2, CONFIG_WIDTH - 4), .height = 8}, LIME);
        }
        //display.drawBuffer(0, y * TILESIZE, buffer);
        ili9341_drawBuffer(0, y * TILESIZE, buffer);
    }
}

void CEngine::animate()
{
    for (int i = 0; i < sizeof(animzSeq) / sizeof(AnimzSeq); ++i)
    {
        AnimzSeq &seq = animzSeq[i];
        int j = seq.srcTile;
        tileReplacement[j] = seq.startSeq + seq.index;
        seq.index = seq.index < seq.count - 1 ? seq.index + 1 : 0;
    }
}

void CEngine::drawFont(int x, int y, const char *text)
{
    buffer.fill(BLACK);
    buffer.drawFont(0,4,text, WHITE);
    drawBuffer(x,y);
}

void CEngine::initJoystick()
{
    adc_init();
    adc_gpio_init(26);
    adc_gpio_init(27);
}

uint16_t CEngine::readJoystick()
{
    adc_select_input(0);
    uint adc_x_raw = adc_read();
    adc_select_input(1);
    uint adc_y_raw = adc_read();
    printf("x=%u y=%u\n", adc_x_raw, adc_y_raw);

    uint16_t joy = JOY_NONE;

    if (adc_y_raw < 50)
    {
        joy |= JOY_UP;
    }
    else if (adc_y_raw > 3000)
    {
        joy |= JOY_DOWN;
    }

    if (adc_x_raw < 50)
    {
        joy |= JOY_LEFT;
    }
    else if (adc_x_raw > 3000)
    {
        joy |= JOY_RIGHT;
    }

    //printf("x=%u y=%u joy=%x\n", adc_x_raw, adc_y_raw, joy);


    return joy;
}