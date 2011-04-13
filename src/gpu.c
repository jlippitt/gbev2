#include <assert.h>
#include <stdbool.h>
#include "gpu.h"
#include "gpu_render.h"

struct GPU gpu = {NULL, HBLANK_MODE, 0, {0, 0, 0, 0, 0}};

void gpu_reset()
{
    gpu.screen = SDL_SetVideoMode(DISPLAY_WIDTH, DISPLAY_HEIGHT, 32,
            SDL_HWSURFACE | SDL_DOUBLEBUF);

    assert(gpu.screen != NULL);

    gpu.mode = HBLANK_MODE;
    gpu.modeclock = 0;

    gpu.regs.control = 0;
    gpu.regs.scrollx = 0;
    gpu.regs.scrolly = 0;
    gpu.regs.line    = 0;
    gpu.regs.palette = 0;

    for (Word i = 0; i < VRAM_SIZE; i++)
    {
        gpu.vram[i] = 0;
    }

    for (Word i = 0; i < OAM_SIZE; i++)
    {
        gpu.oam[i] = 0;
    }

    // Blank the screen
    SDL_Rect rect = {0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT};

    SDL_LockSurface(gpu.screen);

    SDL_FillRect(gpu.screen, &rect,
            SDL_MapRGBA(gpu.screen->format, 0xFF, 0xFF, 0xFF, 0xFF));

    SDL_UnlockSurface(gpu.screen);

    SDL_Flip(gpu.screen);
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
            printf("CTL: %02X\n", value);
            gpu.regs.control = value;

        case 0xFF42:
            printf("SCX: %02X\n", value);
            gpu.regs.scrollx = value;

        case 0xFF43:
            printf("SCY: %02X\n", value);
            gpu.regs.scrolly = value;

        case 0xFF47:
            printf("BGP: %02X\n", value);
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
                gpu.mode = VRAM_MODE;
                gpu.modeclock = 0;
            }
            break;

        // VRAM read mode, scanline active
        // Treat end of mode as end of scanline
        case VRAM_MODE:
            if (gpu.modeclock >= 172)
            {
                render_scanline();

                gpu.mode = HBLANK_MODE;
                gpu.modeclock = 0;
            }
            break;

        // After last hblank, update the display
        case HBLANK_MODE:
            if (gpu.modeclock >= 204)
            {
                if (gpu.regs.line == (DISPLAY_HEIGHT - 1))
                {
                    // Update screen when scanner reaches end of last line
                    SDL_Flip(gpu.screen);

                    gpu.mode = VBLANK_MODE;
                    gpu.modeclock = 0;
                }
                else
                {
                    gpu.mode = OAM_MODE;
                    gpu.modeclock = 0;
                }

                gpu.regs.line++;
            }
            break;

        // Vblank (10 lines)
        case VBLANK_MODE:
            if (gpu.modeclock >= 456)
            {
                if (gpu.regs.line == (DISPLAY_HEIGHT + 9))
                {
                    // Restart scan
                    gpu.regs.line = 0;
                    gpu.mode = OAM_MODE;
                    gpu.modeclock = 0;
                }
                else
                {
                    gpu.regs.line++;
                    gpu.modeclock = 0;
                }
            }
            break;

        default:
            assert(false);
    }
}

