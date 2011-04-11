#ifndef Z80_BIT_H
#define Z80_BIT_H

#include "z80_macros.h"

void BIT7H()
{
    debug("BIT 7,H");
    alter_flag(ZERO, (H & (1 << 7)) == 0);
    reset_flag(NEGATIVE);
    set_flag(HALF_CARRY);
    tick(8);
}

#endif
