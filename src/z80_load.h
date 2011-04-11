#ifndef Z80_LOAD_H
#define Z80_LOAD_H

#include "z80_macros.h"

void LDHLnn()
{
    debug("LD HL,nn");
    HL = next_word();
    tick(12);
}

void LDSPnn()
{
    debug("LD SP,nn");
    SP = next_word();
    tick(12);
}

#endif
