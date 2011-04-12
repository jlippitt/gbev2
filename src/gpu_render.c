#include <stdbool.h>
#include <stdio.h>
#include "gpu.h"
#include "gpu_render.h"

// Registers
#define CONTROL      (gpu.regs.control)
#define SCROLLX      (gpu.regs.scrollx)
#define SCROLLY      (gpu.regs.scrolly)
#define LINE         (gpu.regs.line)
#define PALETTE      (gpu.regs.palette)

// Flags

#define BACKGROUND   0x80
#define SPRITE       0x40
#define SPRITE_SIZE  0x20
#define BG_TILE_MAP  0x10
#define BG_TILE_SET  0x08
#define WINDOW       0x04
#define WND_TILE_MAP 0x02
#define DISPLAY      0x01

static Byte *get_tile(Word map_offset);
static void draw_pixel(const Byte *tile, Byte x, Byte y, uint32_t *pixel);

static inline bool isset_flag(Byte flag)
{
    return CONTROL & flag;
}

void render_scanline()
{
    // Get starting offset for current tile map
    Word map_offset = isset_flag(BG_TILE_MAP) ? 0x1C00 : 0x1800;
    
    // Determine line of tiles we're currently using
    map_offset += (gpu.regs.line + gpu.regs.scrolly) >> 3;

    // Determine which tile in the line to start with
    Byte line_offset = gpu.regs.scrollx >> 3;

    // Determine tile pixel offsets
    Byte tile_x = SCROLLX & 0xF;
    Byte tile_y = (LINE + SCROLLY) & 0xF;

    Byte *tile = get_tile(map_offset + line_offset);

    SDL_LockSurface(gpu.screen);

    uint32_t *pixel = ((uint32_t *)gpu.screen->pixels) + LINE * 160;

    for (Byte i = 0; i < 160; i++)
    {
        draw_pixel(tile, tile_x, tile_y, pixel);

        if (++tile_x == 8)
        {
            tile_x = 0;
            line_offset = (line_offset + 1) & 0x1F;
            tile = get_tile(map_offset + line_offset);
        }
    }

    SDL_UnlockSurface(gpu.screen);
}

Byte *get_tile(Word map_offset)
{
    if (isset_flag(BG_TILE_SET))
    {
        return gpu.vram + gpu.vram[map_offset] * 16;
    }
    else
    {
        return gpu.vram + 0x1000 + ((int8_t)gpu.vram[map_offset]) * 16;
    }
}

void draw_pixel(const Byte *tile, Byte x, Byte y, uint32_t *pixel)
{
    const Byte *row = tile + (2 * y);

    Byte colour = ((row[0] & (0x80 >> x)) ? 0x04 : 0) +
                  ((row[1] & (0x80 >> x)) ? 0x02 : 0);

    // Translate via background palette
    colour = (PALETTE & (0x02 << colour)) >> colour;

    switch (colour)
    {
        case 0:
            *pixel = 0xFFFFFFFF;
            break;

        case 1:
            *pixel = 0xC0C0C0C0;
            break;

        case 2:
            *pixel = 0x60606060;
            break;

        case 3:
            *pixel = 0x00000000;
            break;
    }
}

