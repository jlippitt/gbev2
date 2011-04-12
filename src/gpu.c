#include "gpu.h"
#include "gpu_render.h"

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

                render_scanline();
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
                    printf("SDL_FLIP\n");
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

