#ifndef Z80_LOAD_H
#define Z80_LOAD_H

#include "z80_macros.h"

void LDSPnn()
{
    debug("LD SP,nn");
    SP = next_word();
}

#endif
