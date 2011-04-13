#include <SDL/SDL.h>
#include "gbe.h"

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    gbe_reset(argv[1]);
    gbe_run();

    SDL_Quit();

    return 0;
}

