#include "buffer.h"
//#include <esp_heap_caps_init.h>
#include <algorithm>
#include <stdio.h>
//#include "font.h"
//#include "colors.h"

extern uint8_t bitfont_bin;

CBuffer::CBuffer(int len, int hei)
{
    m_buffer = reinterpret_cast<uint16_t *>(malloc(len * hei * sizeof(m_buffer[0])));
    m_len = len;
    m_hei = hei;
    printf("DMA: %p  len: %d, hei:%d\n", m_buffer, len, hei);
}

CBuffer::~CBuffer()
{
    if (m_buffer)
    {
        free(m_buffer);
    }
}

int CBuffer::len()
{
    return m_len;
}
int CBuffer::hei()
{
    return m_hei;
}

void CBuffer::drawTile(const int x, const int y, uint16_t *tile)
{
    uint16_t *o = m_buffer + x + y * m_len;
    uint32_t *p32 = reinterpret_cast<uint32_t *>(tile);
    for (int yy = 0; yy < 16; ++yy)
    {
        uint32_t *d32 = reinterpret_cast<uint32_t *>(o);
        d32[0] = p32[0];
        d32[1] = p32[1];
        d32[2] = p32[2];
        d32[3] = p32[3];
        d32[4] = p32[4];
        d32[5] = p32[5];
        d32[6] = p32[6];
        d32[7] = p32[7];
        o += m_len;
        p32 += 8;
    }
}

uint16_t *CBuffer::buffer()
{
    return m_buffer;
}

void CBuffer::fill(const uint16_t color)
{
   // const uint16_t fcolor = flipColor(color);
    if (m_buffer)
    {
        for (int i = 0; i < m_len * m_hei; ++i)
        {
            m_buffer[i] = color;
        }
    }
}

void CBuffer::forget()
{
    if (m_buffer)
    {
        free(m_buffer);
        m_buffer = nullptr;
    }
    m_hei = m_len = 0;
}

void CBuffer::drawFont(int x, int y, const char *s, uint16_t color)
{
   // const uint16_t fcolor = flipColor(color);
    uint8_t *font = &bitfont_bin;
    for (int j = 0; s[j]; ++j)
    {
        int u = s[j] < 127 ? std::max(s[j] - 32,0) : 0;
        uint16_t *o = m_buffer + x + y * m_len + 8 * j;
        for (int yy = 0; yy < 8; ++yy)
        {
            uint8_t data = font[8 * u  + yy];
            for (int xx = 0; xx < 8; ++xx)
            {
                o[xx] = data & 1 ? color : 0;
                data = data >> 1;
            }
            o += m_len;
        }
    }
}

void CBuffer::drawRect(const Rect &rect, uint16_t color)
{
    //const uint16_t fcolor = flipColor(color);
    uint16_t *buf = m_buffer + rect.x + rect.y * m_len;
    for (int y = 0; y < rect.height; ++y)
    {
        for (int x = 0; x < rect.width; ++x)
        {
            buf[x] = color;
        }
        buf += m_len;
    }
}