#include <stdbool.h>
#include "gpu.h"

#define BACKGROUND   0x80
#define SPRITE       0x40
#define SPRITE_SIZE  0x20
#define BG_TILE_MAP  0x10
#define BG_TILE_SET  0x08
#define WINDOW       0x04
#define WND_TILE_MAP 0x02
#define DISPLAY      0x01

static inline bool isset_flag(Byte flag);

static void renderscan();

struct GPU gpu = {NULL, 0, 0, {0, 0, 0, 0, 0}};

void gpu_reset()
{
    for (Word i = 0; i < VRAM_SIZE; i++)
    {
        gpu.vram[i] = 0;
    }

    for (Word i = 0; i < OAM_SIZE; i++)
    {
        gpu.oam[i] = 0;
    }

    gpu.screen = SDL_SetVideoMode(160, 144, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

    SDL_Rect rect = {0, 0, 160, 144};

    SDL_FillRect(gpu.screen, &rect, 0xFFFFFFFF);

    SDL_Flip(gpu.screen);

    gpu.mode = 0;
    gpu.modeclock = 0;

    gpu.regs.control = 0;
    gpu.regs.scrollx = 0;
    gpu.regs.scrolly = 0;
    gpu.regs.line = 0;
    gpu.regs.palette = 0;
}

Byte gpu_getbyte(Word addr)
{
    switch (addr)
    {
        case 0xFF40:
            return gpu.regs.control;

        case 0xFF42:
            return gpu.regs.scrollx;

        case 0xFF43:
            return gpu.regs.scrolly;

        case 0xFF44:
            return gpu.regs.line;

        default:
            return 0;
    }
}

void gpu_putbyte(Word addr, Byte value)
{
    switch (addr)
    {
        case 0xFF40:
            gpu.regs.control = value;

        case 0xFF42:
            gpu.regs.scrollx = value;

        case 0xFF43:
            gpu.regs.scrolly = value;

        case 0xFF47:
            gpu.regs.palette = value;
    }
}

void gpu_step(Word ticks)
{
    gpu.modeclock += ticks;

    switch (gpu.mode)
    {
        // OAM read mode, scanline active
        case OAM_MODE:
            if (gpu.modeclock >= 80)
            {
                gpu.modeclock = 0;
                gpu.mode = VRAM_MODE;
            }
            break;

        // VRAM read mode, scanline active
        // Treat end of mode as end of scanline
        case VRAM_MODE:
            if (gpu.modeclock >= 172)
            {
                gpu.modeclock = 0;
                gpu.mode = HBLANK_MODE;

                renderscan();
            }
            break;

        // After last hblank, update the display
        case HBLANK_MODE:
            if (gpu.modeclock >= 204)
            {
                gpu.modeclock = 0;
                gpu.regs.line++;

                if (gpu.regs.line == 143)
                {
                    gpu.mode = VBLANK_MODE;
                    SDL_Flip(gpu.screen);
                }
                else
                {
                    gpu.mode = OAM_MODE;
                }
            }
            break;

        // Vblank (10 lines)
        case VBLANK_MODE:
            if (gpu.modeclock >= 456)
            {
                gpu.modeclock = 0;
                gpu.regs.line++;

                if (gpu.regs.line > 153)
                {
                    // Restart scanning modes
                    gpu.mode = OAM_MODE;
                    gpu.regs.line = 0;
                }
            }
            break;

        default:
            // Nothing
            break;
    }
}

bool isset_flag(Byte flag)
{
    return gpu.regs.control & flag;
}

void renderscan()
{
    // Get offset for current tile map
    Byte *map = gpu.vram + (isset_flag(BG_TILE_MAP) ? 0x1C00 : 0x1800);
    
    // Determine line of tiles we're currently using
    map += (gpu.regs.line + gpu.regs.scrolly) >> 3;

    // Determine which tile in the line to start with
    map += gpu.regs.scrollx >> 3;

}

