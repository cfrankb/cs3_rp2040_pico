#ifndef _engine__h
#define _engine__h

#include <cinttypes>

class CGame;

class CEngine
{
public:
    CEngine(CGame *game);
    ~CEngine();

    bool init();
    void drawLevelIntro();
    void drawScreen();
    void drawBuffer(int x, int y);
    void animate();
    void fill(uint16_t color);
    void drawFont(int x, int y, const char *text);
    void initJoystick();
    uint16_t readJoystick();

protected:
    CGame *m_game;
    enum{
        NO_ANIMZ = 255,
    };
};
#endif