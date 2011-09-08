#include <SDL/SDL.h>
#include "gbe.h"
#include "mbc.h"

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    gbe_reset(argv[1]);
    gbe_run();
    mbc_cleanup();

    SDL_Quit();

    return 0;
}

