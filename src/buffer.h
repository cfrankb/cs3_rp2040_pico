#ifndef __BUFFER__H
#define __BUFFER__H

#include <stdint.h>

typedef struct
{
    int x;
    int y;
    int width;
    int height;
} Rect;


class CBuffer
{
public:
    CBuffer(int len, int hei);
    ~CBuffer();

    int len();
    int hei();
    void drawTile(const int x, const int y, uint16_t *tile);
    void drawFont(int x, int y, const char *s, uint16_t color = 0xffff);
    void drawRect(const Rect &rect, uint16_t color = 0xffff);
    uint16_t *buffer();

    void fill(const uint16_t color);
    void forget();

protected:
    uint16_t *m_buffer;
    int m_len;
    int m_hei;
    uint16_t m_limit;
    int m_oY;
};

#endif