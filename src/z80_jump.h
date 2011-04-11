#ifndef Z80_JUMP_H
#define Z80_JUMP_H

#include "z80_macros.h"

void JRNZn()
{
    debug("JR NZ,n");

    if (!isset_flag(ZERO))
    {
        Byte tmp = next_byte();
        PC += (int8_t)tmp;
    }

    tick(8);
}

#endif
