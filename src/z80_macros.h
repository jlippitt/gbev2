#ifndef Z80_MACROS_H
#define Z80_MACROS_H

#include "mmu.h"
#include "types.h"

#define A  (z80.regs.af.byte.upper)
#define F  (z80.regs.af.byte.lower)
#define B  (z80.regs.bc.byte.upper)
#define C  (z80.regs.bc.byte.lower)
#define D  (z80.regs.de.byte.upper)
#define E  (z80.regs.de.byte.lower)
#define H  (z80.regs.hl.byte.upper)
#define L  (z80.regs.hl.byte.lower)
#define AF (z80.regs.af.word)
#define BC (z80.regs.bc.word)
#define DE (z80.regs.de.word)
#define HL (z80.regs.hl.word)
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
