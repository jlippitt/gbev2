#ifndef Z80_MATH_H
#define Z80_MATH_H

#include "z80_macros.h"

void XORA()
{
    debug("XOR A");
    A ^= A;
    tick(4);
}

#endif
