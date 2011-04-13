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

#define BACKGROUND   0x01
#define SPRITE       0x02
#define SPRITE_SIZE  0x04
#define BG_TILE_MAP  0x08
#define BG_TILE_SET  0x10
#define WINDOW       0x20
#define WND_TILE_MAP 0x40
#define DISPLAY      0x80

// Other constants
#define MAP_SIZE     32
#define TILE_SIZE    8

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

    // Determine position to start from on background map
    Byte bg_x = SCROLLX;
    Byte bg_y = LINE + SCROLLY;
    
    // Determine line of tiles we're currently using
    map_offset += bg_y / TILE_SIZE * MAP_SIZE;

    // Determine which tile in the line to start with
    Byte line_offset = bg_x / TILE_SIZE;

    // Determine tile pixel offsets
    Byte tile_x = bg_x % TILE_SIZE;
    Byte tile_y = bg_y % TILE_SIZE;

    Byte *tile = get_tile(map_offset + line_offset);

    SDL_LockSurface(gpu.screen);

    uint32_t *pixel = ((uint32_t *)gpu.screen->pixels) + LINE * DISPLAY_WIDTH;

    for (Byte i = 0; i < DISPLAY_WIDTH; i++)
    {
        draw_pixel(tile, tile_x, tile_y, pixel);

        if (++tile_x == TILE_SIZE)
        {
            tile_x = 0;
            line_offset = (line_offset + 1) % MAP_SIZE;
            tile = get_tile(map_offset + line_offset);
        }

        ++pixel;
    }

    SDL_UnlockSurface(gpu.screen);
}

Byte *get_tile(Word map_offset)
{
    if (isset_flag(BG_TILE_SET))
    {
        return gpu.vram + gpu.vram[map_offset] * 2 * TILE_SIZE;
    }
    else
    {
        return gpu.vram + 0x1000 + ((int8_t)gpu.vram[map_offset]) * 2 * TILE_SIZE;
    }
}

void draw_pixel(const Byte *tile, Byte x, Byte y, uint32_t *pixel)
{
    tile += 2 * y;

    Byte colour = ((tile[0] & (0x80 >> x)) ? 0x02 : 0) +
                  ((tile[1] & (0x80 >> x)) ? 0x01 : 0);

    // Translate via background palette
    colour = (PALETTE & (0x03 << (colour * 2))) >> (colour * 2);

    switch (colour)
    {
        case 0:
            *pixel = SDL_MapRGBA(gpu.screen->format, 0xFF, 0xFF, 0xFF, 0xFF);
            break;

        case 1:
            *pixel = SDL_MapRGBA(gpu.screen->format, 0xC0, 0xC0, 0xC0, 0xFF);
            break;

        case 2:
            *pixel = SDL_MapRGBA(gpu.screen->format, 0x60, 0x60, 0x60, 0xFF);
            break;

        case 3:
            *pixel = SDL_MapRGBA(gpu.screen->format, 0x00, 0x00, 0x00, 0xFF);
            break;
    }
}

