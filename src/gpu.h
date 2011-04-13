#ifndef GPU_H
#define GPU_H

#include <SDL/SDL.h>
#include "types.h"

#define DISPLAY_WIDTH  160
#define DISPLAY_HEIGHT 144

#define VRAM_SIZE 8192
#define OAM_SIZE  160

#define HBLANK_MODE 0
#define VBLANK_MODE 1
#define OAM_MODE    2
#define VRAM_MODE   3

extern struct GPU
{
    SDL_Surface *screen;
    Byte mode;
    Word modeclock;

    struct
    {
        Byte control;
        Byte scrollx;
        Byte scrolly;
        Byte line;
        Byte palette;
    }
    regs;

    Byte vram[VRAM_SIZE];
    Byte oam[OAM_SIZE];
}
gpu;

void gpu_reset();

Byte gpu_getbyte(Word addr);

void gpu_putbyte(Word addr, Byte value);

void gpu_step(Word ticks);

#endif
