#ifndef Z80_MACROS_H
#define Z80_MACROS_H

#include "mmu.h"
#include "types.h"

#define A  (z80.regs.a)
#define PC (z80.regs.pc)
#define SP (z80.regs.sp)

static inline void debug(const char *opstr)
{
    printf("%s\n", opstr);
}

static inline Byte next_byte()
{
    return mmu_getbyte(PC++);
}

static inline Word next_word()
{
    Word tmp = mmu_getword(PC);
    PC += 2;
    return tmp;
}

static void tick(Word n)
{
    z80.regs.t = n;
}

#endif
