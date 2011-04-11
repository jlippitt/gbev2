#ifndef Z80_H
#define Z80_H

#include "types.h"

extern struct Z80
{
    struct
    {
        Word pc;
        Word sp;
    }
    regs;
}
z80;

void z80_execute();

#endif
