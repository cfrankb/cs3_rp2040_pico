#ifndef __PALETTE_H
#define __PALETTE_H

#define SWAP_BYTES(color) ((uint16_t)(color >> 8) | (uint16_t)(color << 8))
#define RED SWAP_BYTES(0xf800)
#define GREEN SWAP_BYTES(0x07e0)
#define BLUE SWAP_BYTES(0x001f)
#define BLACK SWAP_BYTES(0x0000)
#define WHITE SWAP_BYTES(0xffff)
#define GRAY SWAP_BYTES(0x8c51)
#define YELLOW SWAP_BYTES(0xFFE0)
#define CYAN SWAP_BYTES(0x07FF)
#define PURPLE SWAP_BYTES(0xF81F)
#define LIME SWAP_BYTES(0x0f0f)
#define PINK SWAP_BYTES(0xfefb)
#define ORANGE SWAP_BYTES(0xfc80)
#define VIOLET SWAP_BYTES(0xa81f)

#endif