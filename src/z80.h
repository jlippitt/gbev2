#ifndef Z80_H
#define Z80_H

#include "types.h"

#define ZERO        0x80
#define NEGATIVE    0x40
#define HALF_CARRY  0x20
#define CARRY       0x10

extern struct Z80
{
    struct
    {
        union
        {
            Word word;

            struct
            {
                Byte upper;
                Byte lower;
            }
            byte;
        }
        af;

        union
        {
            Word word;

            struct
            {
                Byte upper;
                Byte lower;
            }
            byte;
        }
        bc;

        union
        {
            Word word;

            struct
            {
                Byte upper;
                Byte lower;
            }
            byte;
        }
        de;

        union
        {
            Word word;

            struct
            {
                Byte upper;
                Byte lower;
            }
            byte;
        }
        hl;

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
