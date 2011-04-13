#include <stdbool.h>
#include <SDL/SDL.h>
#include "gbe.h"
#include "gpu.h"
#include "mmu.h"
#include "z80.h"

void gbe_reset(const char *filename)
{
    gpu_reset();
    mmu_reset();
    z80_reset();

    mmu_load(filename);
}

void gbe_run()
{
    SDL_Event event;
    bool running = true;

    while (running)
    {
        z80_doframe();

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }
    }
}

