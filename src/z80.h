#ifndef Z80_H
#define Z80_H

#include "types.h"

union Z80_WordReg
{
    Word word;

    struct
    {
        Byte upper;
        Byte lower;
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
        Word t;
    }
    regs;

    struct
    {
        Word t;
    }
    clock;
}
z80;

void z80_execute();

#endif
