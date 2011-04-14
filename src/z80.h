#ifndef Z80_H
#define Z80_H

#include "types.h"

union Z80_WordReg
{
    Word word;

    struct
    {
        Byte lower;
        Byte upper;
    }
    byte;
};

extern struct Z80
{
    struct
    {
        union Z80_WordReg af;
        union Z80_WordReg bc;
        union Z80_WordReg de;
        union Z80_WordReg hl;
        Word pc;
        Word sp;
        Byte ime;
        Byte t;
    }
    regs;

    struct
    {
        uint32_t t;
    }
    clock;

    Byte halt;
}
z80;

void z80_reset();

void z80_doframe();

#endif
