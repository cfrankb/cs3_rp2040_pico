#ifndef __ACTOR__H
#define __ACTOR__H
#include <stdint.h>
#include "map.h"

class CActor
{

public:
    CActor(uint8_t x = 0, uint8_t y = 0, uint8_t type = 0, uint8_t aim = 0);
    CActor(const Pos &pos, uint8_t type = 0, uint8_t aim = 0);
    ~CActor();

    bool canMove(int aim);
    void move(const int aim);
    uint8_t getX() const;
    uint8_t getY() const;
    uint8_t getPU() const;
    void setPU(const uint8_t c);
    void setXY(const Pos pos);
    uint8_t getAim() const;
    void setAim(const uint8_t aim);
    int findNextDir();
    bool isPlayerThere(uint8_t aim);
    uint8_t tileAt(uint8_t aim);
    void setType(const uint8_t type);

protected:
    uint8_t m_x;
    uint8_t m_y;
    uint8_t m_type;
    uint8_t m_aim;
    uint8_t m_pu;

    friend class CGame;
};

#endif