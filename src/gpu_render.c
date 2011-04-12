#include <stdbool.h>
#include "gpu.h"
#include "gpu_render.h"

#define BACKGROUND   0x80
#define SPRITE       0x40
#define SPRITE_SIZE  0x20
#define BG_TILE_MAP  0x10
#define BG_TILE_SET  0x08
#define WINDOW       0x04
#define WND_TILE_MAP 0x02
#define DISPLAY      0x01

static inline bool isset_flag(Byte flag)
{
    return gpu.regs.control & flag;
}

void render_scanline()
{
    // Get offset for current tile map
    Byte *map = gpu.vram + (isset_flag(BG_TILE_MAP) ? 0x1C00 : 0x1800);
    
    // Determine line of tiles we're currently using
    map += (gpu.regs.line + gpu.regs.scrolly) >> 3;

    // Determine which tile in the line to start with
    map += gpu.regs.scrollx >> 3;

}

