#ifndef Z80_MISC_H
#define Z80_MISC_H

#include <stdlib.h>
#include "z80_macros.h"

void NOP()
{
    debug("NOP");
    tick(4);
}

void STOP()
{
    debug("STOP");
    exit(0);
}

void EI()
{
    debug("EI");
    // TODO: Not yet implemented
    tick(4);
}

#endif
