#ifndef Z80_MATH_H
#define Z80_MATH_H

#include "z80_macros.h"

void XORA()
{
    debug("XOR A");
    A ^= A;
    tick(4);
}

void INCC()
{
    debug("INC C");
    C++;
    alter_flag(ZERO, C);
    reset_flag(NEGATIVE);
    alter_flag(HALF_CARRY, (C & 0xF) == 0);
    tick(4);
}

#endif
