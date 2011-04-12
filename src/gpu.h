#ifndef GPU_H
#define GPU_H

#include <SDL/SDL.h>
#include "types.h"

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
    Byte line;
    Byte vram[VRAM_SIZE];
    Byte oam[OAM_SIZE];
}
gpu;

void gpu_reset();

void gpu_step(Word ticks);

#endif
