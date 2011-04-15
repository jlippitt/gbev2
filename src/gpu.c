#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include "gpu.h"
#include "gpu_render.h"
#include "mmu.h"

static void dma_transfer(Word addr);

struct GPU gpu = {NULL, HBLANK_MODE, 0, {0, 0, 0, 0}, {0, 0, 0}};

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
    gpu.regs.raster  = 0;

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
            return (gpu.regs.line == gpu.regs.raster ? 0x04 : 0) | gpu.mode;

        case 0xFF42:
            return gpu.regs.scrolly;

        case 0xFF43:
            return gpu.regs.scrollx;

        case 0xFF44:
            return gpu.regs.line;

        case 0xFF45:
            return gpu.regs.raster;

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

        case 0xFF42:
            gpu.regs.scrolly = value;
            break;

        case 0xFF43:
            gpu.regs.scrollx = value;
            break;

        case 0xFF45:
            gpu.regs.raster = value;
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
                gpu.mode = VRAM_MODE;
                gpu.modeclock = 0;
            }
            break;

        // VRAM read mode, scanline active
        // Treat end of mode as end of scanline
        case VRAM_MODE:
            if (gpu.modeclock >= 43)
            {
                render_scanline();

                gpu.mode = HBLANK_MODE;
                gpu.modeclock = 0;
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

                    gpu.mode = VBLANK_MODE;
                    gpu.modeclock = 0;

                    mmu.iflag |= INT_VBLANK;
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
            if (gpu.modeclock >= 114)
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

void dma_transfer(Word addr)
{
    printf("DMA TRANSFER\n");
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

void gpu_dump_oam()
{
    FILE *fp = fopen("oam.txt", "w");

    Byte *oam = gpu.oam;

    for (int i = 0; i < 40; i++)
    {
        fprintf(fp, "0x%04X:\n", i * 4);
        fprintf(fp, "XPOS=%02X\n", *oam++);
        fprintf(fp, "YPOS=%02X\n", *oam++);
        fprintf(fp, "TILE=%02X\n", *oam++);
        fprintf(fp, "FLAG=%02X\n", *oam++);
        fprintf(fp, "\n");
    }

    fclose(fp);
}

void gpu_dump_vram()
{
    FILE *fp = fopen("vram.txt", "w");

    int i, j, k;

    size_t offset = 0;

    for (i = 0; i < 384; i++)
    {
        if (i < 128)
        {
            fprintf(fp, "TILE %d (0x%02X)\n", i, i);  
        }
        else if (i < 256)
        {
            fprintf(fp, "TILE %d (0x%02X/0x%02X)\n", i, i, (Byte)i - 128);  
        }
        else
        {
            fprintf(fp, "TILE %d (0x%02X)\n", -(384 - i), (Byte)i - 128);  
        }
        
        for (j = 0; j < 8; j++)
        {
            for (k = 0; k < 8; k++)
            {
                Byte colour = ((gpu.vram[offset] & (1 << (7 - k))) ? 0x02 : 0) +
                              ((gpu.vram[offset + 1] & (1 << (7 - k))) ? 0x01 : 0);

                switch (colour)
                {
                    case 0:
                        fprintf(fp, "0");
                        break;
                         
                    case 1:
                        fprintf(fp, "1");
                        break;

                    case 2:
                        fprintf(fp, "2");
                        break;

                    case 3:
                        fprintf(fp, "3");
                        break;
                }
            }

            offset += 2;;

            fprintf(fp, "\n");
        }

        fprintf(fp, "\n");
    }

    for (i = 0; i < 2; i++)
    {
        fprintf(fp, "BG MAP %d\n", i);

        for (j = 0; j < 32; j++)
        {
            for (k = 0; k < 32; k++)
            {
                fprintf(fp, "%02X ", gpu.vram[offset++]);
            }

            fprintf(fp, "\n");
        }

        fprintf(fp, "\n");
    }

    fclose(fp);
}

