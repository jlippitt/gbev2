#include "gpu.h"

struct GPU gpu = {NULL};

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

    gpu.screen = SDL_SetVideoMode(160, 144, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
}

