#ifndef Z80_LOAD_H
#define Z80_LOAD_H

#include "z80_macros.h"

#define DEF_LDrn(r1) \
void LD##r1##n() \
{ \
    debug("LD " #r1 ",n"); \
    r1 = next_byte(); \
    tick(8); \
}

DEF_LDrn(A);
DEF_LDrn(C);

void LDHCA()
{
    debug("LD ($FF00+C),A");
    mmu_putbyte(0xFF00 + C, A);
    tick(8);
}

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

void LDDHLA()
{
    debug("LDD (HL),A");
    mmu_putbyte(HL--, A);
    tick(8);
}

#endif
