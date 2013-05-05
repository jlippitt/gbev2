#include <SDL/SDL.h>
#include <stdio.h>
#include "gbe.h"
#include "mbc.h"

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        SDL_Init(SDL_INIT_VIDEO);

        gbe_reset(argv[1]);
        gbe_run();
        mbc_cleanup();

        SDL_Quit();
    }
    else
    {
        fprintf(stderr, "Syntax: gbe <rom-path>\n");
    }

    return 0;
}

