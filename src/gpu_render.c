#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "gpu.h"
#include "gpu_render.h"

// Registers
#define CONTROL      (gpu.regs.control)
#define SCROLLX      (gpu.regs.scrollx)
#define SCROLLY      (gpu.regs.scrolly)
#define WNDPOSX      (gpu.regs.wndposx)
#define WNDPOSY      (gpu.regs.wndposy)
#define LINE         (gpu.regs.line)
#define RASTER       (gpu.regs.raster)
#define PAL_BG       (gpu.pal.bg)
#define PAL_OBJ0     (gpu.pal.obj0)
#define PAL_OBJ1     (gpu.pal.obj1)

// Flags

#define BACKGROUND   0x01
#define OBJECT       0x02
#define OBJECT_SIZE  0x04
#define BG_TILE_MAP  0x08
#define BG_TILE_SET  0x10
#define WINDOW       0x20
#define WND_TILE_MAP 0x40
#define DISPLAY      0x80

// Object flags
#define OBJ_BELOW_BG 0x80
#define OBJ_YFLIP    0x40
#define OBJ_XFLIP    0x20
#define OBJ_PALETTE  0x10

// Other constants
#define MAP_SIZE     32
#define TILE_SIZE    8

typedef struct Sprite
{
    Byte id;
    Byte y;
    Byte x;
    Byte tile;
    Byte flags;
}
Sprite;

static Byte *get_tile(Byte tile_no, Byte tileset);

static void draw_pixel(uint32_t *pixel, Byte colour, Byte palette);

int sprite_compare(const void *sprite1, const void *sprite2);

static inline bool isset_flag(Byte flag)
{
    return CONTROL & flag;
}

void render_scanline()
{
    if (!isset_flag(DISPLAY))
    {
        return;
    }

    Byte bgcolour[DISPLAY_WIDTH];

    SDL_LockSurface(gpu.screen);

    if (isset_flag(BACKGROUND))
    {
        // Get starting offset for current tile map
        Word map_offset = isset_flag(BG_TILE_MAP) ? 0x1C00 : 0x1800;

        // Determine position to start from on background map
        Byte bg_x = SCROLLX;
        Byte bg_y = SCROLLY + LINE;
        
        // Determine line of tiles we're currently using
        map_offset += bg_y / TILE_SIZE * MAP_SIZE;

        // Determine which tile in the line to start with
        Byte line_offset = bg_x / TILE_SIZE;

        // Determine tile pixel offsets
        Byte tile_x = bg_x % TILE_SIZE;
        Byte tile_y = bg_y % TILE_SIZE;

        Byte *tile_row = get_tile(gpu.vram[map_offset + line_offset],
                isset_flag(BG_TILE_SET)) + tile_y * 2;

        uint32_t *pixel = ((uint32_t *)gpu.screen->pixels) + LINE * DISPLAY_WIDTH;

        for (Byte i = 0; i < DISPLAY_WIDTH; i++)
        {
            Byte colour = ((tile_row[0] & (1 << (7 - tile_x))) ? 0x02 : 0) +
                          ((tile_row[1] & (1 << (7 - tile_x))) ? 0x01 : 0);

            bgcolour[i] = colour;

            draw_pixel(pixel, colour, PAL_BG);

            tile_x++;
            pixel++;

            if (tile_x == TILE_SIZE)
            {
                tile_x = 0;
                line_offset = (line_offset + 1) % MAP_SIZE;
                tile_row = get_tile(gpu.vram[map_offset + line_offset],
                        isset_flag(BG_TILE_SET)) + tile_y * 2;
            }
        }

        if (isset_flag(WINDOW))
        {
            // Check if window falls on this scanline
            if (WNDPOSY <= LINE)
            {
                // Get starting offset for current tile map
                Word map_offset = isset_flag(WND_TILE_MAP) ? 0x1C00 : 0x1800;
    
                // Determine position to start from on background map
                Byte bg_x = 0;
                Byte bg_y = LINE - WNDPOSY;
    
                // Determine line of tiles we're currently using
                map_offset += bg_y / TILE_SIZE * MAP_SIZE;
    
                // Determine which tile in the line to start with
                Byte line_offset = 0; // Always first tile
    
                // Determine tile pixel offsets
                Byte tile_x = bg_x % TILE_SIZE;
                Byte tile_y = bg_y % TILE_SIZE;
    
                Byte *tile_row = get_tile(gpu.vram[map_offset + line_offset],
                        isset_flag(BG_TILE_SET)) + tile_y * 2;
    
                Byte start_x = WNDPOSX > 7 ? WNDPOSX - 7 : 0;
    
                uint32_t *pixel = ((uint32_t *)gpu.screen->pixels) + LINE * DISPLAY_WIDTH + start_x;

                for (Byte i = start_x; i < DISPLAY_WIDTH; i++)
                {
                    Byte colour = ((tile_row[0] & (1 << (7 - tile_x))) ? 0x02 : 0) +
                                  ((tile_row[1] & (1 << (7 - tile_x))) ? 0x01 : 0);
    
                    bgcolour[i] = colour;
    
                    draw_pixel(pixel, colour, PAL_BG);
    
                    tile_x++;
                    pixel++;
    
                    if (tile_x == TILE_SIZE)
                    {
                        tile_x = 0;
                        line_offset++; // Cannot wrap - no need for modulus
                        tile_row = get_tile(gpu.vram[map_offset + line_offset],
                                isset_flag(BG_TILE_SET)) + tile_y * 2;
                    }
                }
            }
        }
    }

    if (isset_flag(OBJECT))
    {
        Sprite sprites[40];

        /*
         * Determine sprite priority
         */

        Byte *obj = gpu.oam;

        for (int i = 0; i < 40; i++)
        {
            sprites[i].id    = i;
            sprites[i].y     = *obj++;
            sprites[i].x     = *obj++;
            sprites[i].tile  = *obj++;
            sprites[i].flags = *obj++;
        }

        qsort(&sprites[0], 40, sizeof(Sprite), &sprite_compare);

        /*
         * Draw sprites
         */

        Byte obj_height = isset_flag(OBJECT_SIZE) ? 16 : 8;

        for (int i = 0; i < 40; i++)
        {
            Byte obj_y     = sprites[i].y - 16;
            Byte obj_x     = sprites[i].x - 8;
            Byte obj_tile  = sprites[i].tile;
            Byte obj_flags = sprites[i].flags;

            // Check if sprite falls on this scanline
            if (obj_y <= LINE && (obj_y + obj_height) > LINE)
            {
                // Determine tile row offset
                Byte tile_y = LINE - obj_y;

                if (isset_flag(OBJECT_SIZE))
                {
                    if (tile_y < TILE_SIZE)
                    {
                        obj_tile &= ~0x1;
                    }
                    else
                    {
                        obj_tile |= 0x1;
                    }
                }

                if (obj_flags & OBJ_YFLIP)
                {
                    tile_y = obj_height - tile_y - 1;
                }

                Byte *tile_row = get_tile(obj_tile, 1) + tile_y * 2;

                uint32_t *pixel = ((uint32_t *)gpu.screen->pixels) +
                    LINE * DISPLAY_WIDTH + obj_x;

                for (Byte i = 0; i < TILE_SIZE; i++)
                {
                    Byte tile_x = (obj_flags & OBJ_XFLIP) ? i : (7 - i);

                    Byte colour = ((tile_row[0] & (1 << tile_x)) ? 0x02 : 0) +
                                  ((tile_row[1] & (1 << tile_x)) ? 0x01 : 0);

                    // - Make sure pixel is on screen
                    // - Pixel not be transparent
                    // - If displaying under background,
                    //   background must be transparent
                    if ((obj_x + i) >= 0 && (obj_x + i) < DISPLAY_WIDTH &&
                        colour != 0 &&
                        (!isset_flag(BACKGROUND) ||
                         !(obj_flags & OBJ_BELOW_BG) ||
                         bgcolour[obj_x + i] == 0))
                    {
                        draw_pixel(pixel, colour, (obj_flags & OBJ_PALETTE) ?
                                PAL_OBJ1 : PAL_OBJ0);
                    }

                    ++pixel;
                }
            }
        }
    }

    SDL_UnlockSurface(gpu.screen);
}

Byte *get_tile(Byte tile_no, Byte tileset)
{
    if (tileset)
    {
        return gpu.vram + tile_no * 2 * TILE_SIZE;
    }
    else
    {
        return gpu.vram + 0x1000 + ((int8_t)tile_no) * 2 * TILE_SIZE;
    }
}

void draw_pixel(uint32_t *pixel, Byte colour, Byte palette)
{
    switch ((palette & (0x03 << (colour * 2))) >> (colour * 2))
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

int sprite_compare(const void *sprite1, const void *sprite2)
{
    if (((Sprite *)sprite1)->x == ((Sprite *)sprite2)->x)
    {
        return ((Sprite *)sprite2)->id - ((Sprite *)sprite1)->id;
    }
    else
    {
        return ((Sprite *)sprite2)->x - ((Sprite *)sprite1)->x;
    }
}

