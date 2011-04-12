#ifndef Z80_JUMP_H
#define Z80_JUMP_H

#include "z80_macros.h"

void JRNZn()
{
    Byte tmp = next_byte();
    debug("JR NZ,%d", (int8_t)tmp);

    if (!isset_flag(ZERO))
    {
        PC += (int8_t)tmp;
    }

    tick(8);
}

void CALLnn()
{
    Word tmp = next_word();
    debug("CALL $%04X", tmp);
    push(PC);
    PC = tmp;
    tick(12);
}

#endif
