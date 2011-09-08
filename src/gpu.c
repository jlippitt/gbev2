#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include "gpu.h"
#include "gpu_render.h"
#include "mmu.h"

#define STAT_HBLANK      0x08
#define STAT_VBLANK      0x10
#define STAT_OAM         0x20
#define STAT_COINCIDENCE 0x40

static void dma_transfer(Word addr);

Byte get_lcd_status();

void set_mode(Byte mode);

void next_line();

struct GPU gpu = {NULL, HBLANK_MODE, 0, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0}};

void gpu_reset()
{
    gpu.screen = SDL_SetVideoMode(DISPLAY_WIDTH, DISPLAY_HEIGHT, 32,
            SDL_HWSURFACE | SDL_DOUBLEBUF);

    assert(gpu.screen != NULL);

    gpu.mode = HBLANK_MODE;
    gpu.modeclock = 0;

    gpu.regs.control     = 0;
    gpu.regs.scrollx     = 0;
    gpu.regs.scrolly     = 0;
    gpu.regs.wndposx     = 0;
    gpu.regs.wndposy     = 0;
    gpu.regs.line        = 0;
    gpu.regs.coincidence = 0;
    gpu.regs.status      = 0;

    gpu.pal.bg   = 0;
    gpu.pal.obj0 = 0;
    gpu.pal.obj1 = 0;

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

        case 0xFF41:
            return get_lcd_status();

        case 0xFF42:
            return gpu.regs.scrolly;

        case 0xFF43:
            return gpu.regs.scrollx;

        case 0xFF44:
            return gpu.regs.line;

        case 0xFF45:
            return gpu.regs.coincidence;

        case 0xFF47:
            return gpu.pal.bg;

        case 0xFF48:
            return gpu.pal.obj0;

        case 0xFF49:
            return gpu.pal.obj1;

        case 0xFF4A:
            return gpu.regs.wndposy;

        case 0xFF4B:
            return gpu.regs.wndposx;

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
            break;

        case 0xFF41:
            gpu.regs.status = (value & 0x78) | (gpu.regs.status & 0x07);
            break;

        case 0xFF42:
            gpu.regs.scrolly = value;
            break;

        case 0xFF43:
            gpu.regs.scrollx = value;
            break;

        case 0xFF45:
            gpu.regs.coincidence = value;
            break;

        case 0xFF46:
            dma_transfer(value << 8);
            break;

        case 0xFF47:
            gpu.pal.bg = value;
            break;

        case 0xFF48:
            gpu.pal.obj0 = value;
            break;

        case 0xFF49:
            gpu.pal.obj1 = value;
            break;

        case 0xFF4A:
            gpu.regs.wndposy = value;
            break;

        case 0xFF4B:
            gpu.regs.wndposx = value;
            break;
    }
}

void gpu_step(Word ticks)
{
    gpu.modeclock += ticks;

    switch (gpu.mode)
    {
        // OAM read mode, scanline active
        case OAM_MODE:
            if (gpu.modeclock >= 20)
            {
                set_mode(VRAM_MODE);
            }
            break;

        // VRAM read mode, scanline active
        // Treat end of mode as end of scanline
        case VRAM_MODE:
            if (gpu.modeclock >= 43)
            {
                render_scanline();

                set_mode(HBLANK_MODE);
            }
            break;

        // After last hblank, update the display
        case HBLANK_MODE:
            if (gpu.modeclock >= 51)
            {
                if (gpu.regs.line == (DISPLAY_HEIGHT - 1))
                {
                    // Update screen when scanner reaches end of last line
                    SDL_Flip(gpu.screen);

                    set_mode(VBLANK_MODE);

                    mmu.iflag |= INT_VBLANK;
                }
                else
                {
                    set_mode(OAM_MODE);
                }

                next_line();
            }
            break;

        // Vblank (10 lines)
        case VBLANK_MODE:
            if (gpu.modeclock >= 114)
            {
                if (gpu.regs.line == (DISPLAY_HEIGHT + 9))
                {
                    // Restart scan
                    gpu.regs.line = 0;
                    set_mode(OAM_MODE);
                }
                else
                {
                    next_line();
                    gpu.modeclock = 0;
                }
            }
            break;

        default:
            assert(false);
    }
}

void dma_transfer(Word addr)
{
    //printf("DMA TRANSFER\n");
    // OAM DMA
    Byte *oam = gpu.oam;
    //Byte tmp1, tmp2;
    for (Byte i = 0; i < 40; i++)
    {
        *oam++ = mmu_getbyte(addr++);
        *oam++ = mmu_getbyte(addr++);
        *oam++ = mmu_getbyte(addr++);
        *oam++ = mmu_getbyte(addr++);
    }
}

Byte get_lcd_status()
{
    Byte status = gpu.regs.status;

    status |= (gpu.mode & 0x03);

    if (gpu.regs.line == gpu.regs.coincidence)
    {
        status |= 0x04;
    }

    return status;
}

void set_mode(Byte mode)
{
    gpu.mode = mode;
    gpu.modeclock = 0;

    switch (mode)
    {
        case HBLANK_MODE:
            if (gpu.regs.status & STAT_HBLANK)
            {
                mmu.iflag |= INT_LCD_STAT;
            }
            break;

        case VBLANK_MODE:
            if (gpu.regs.status & STAT_VBLANK)
            {
                mmu.iflag |= INT_LCD_STAT;
            }
            break;

        case OAM_MODE:
            if (gpu.regs.status & STAT_OAM)
            {
                mmu.iflag |= INT_LCD_STAT;
            }
            break;
    }
}

void next_line()
{
    gpu.regs.line++;

    if ((gpu.regs.status & STAT_COINCIDENCE) &&
        gpu.regs.line == gpu.regs.coincidence)
    {
        mmu.iflag |= INT_LCD_STAT;
    }
}

