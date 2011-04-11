#ifndef Z80_MISC_H
#define Z80_MISC_H

#include <stdlib.h>
#include "z80_macros.h"

void NOP()
{
    debug("NOP");
}

void STOP()
{
    debug("STOP");
    exit(0);
}

#endif
