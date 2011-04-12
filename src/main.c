#include <SDL/SDL.h>
#include "gpu.h"
#include "mmu.h"
#include "z80.h"

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    gpu_reset();
    mmu_reset();
    mmu_load(argv[1]);
    z80_execute();

    SDL_Quit();

    return 0;
}

