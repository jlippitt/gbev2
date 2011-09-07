#include <stdbool.h>
#include <SDL/SDL.h>
#include "gbe.h"
#include "gpu.h"
#include "joypad.h"
#include "mbc.h"
#include "mmu.h"
#include "z80.h"

void gbe_reset(const char *filename)
{
    gpu_reset();
    mmu_reset();
    z80_reset();

    mbc_load(filename);
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
            switch (event.type)
            {
                case SDL_KEYDOWN:
                    joypad_keydown(event.key.keysym.sym);
                    break;

                case SDL_KEYUP:
                    joypad_keyup(event.key.keysym.sym);
                    break;

                case SDL_QUIT:
                    running = false;
                    break;
            }
        }
    }
}

