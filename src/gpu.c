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

    gpu.screen = SDL_SetVideoMode(160, 144, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

    SDL_Rect rect = {0, 0, 160, 144};

    SDL_FillRect(gpu.screen, &rect, 0xFFFFFFFF);

    SDL_Flip(gpu.screen);
}

