#ifndef Z80_MACROS_H
#define Z80_MACROS_H

#include <stdbool.h>
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

#define ZERO        0x80
#define NEGATIVE    0x40
#define HALF_CARRY  0x20
#define CARRY       0x10

static inline void debug(const char *opstr)
{
    printf(": %s\n", opstr);
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

static inline bool isset_flag(Byte flag)
{
    return (F & flag) ? true : false;
}

static inline void set_flag(Byte flag)
{
    F |= flag;
}

static inline void reset_flag(Byte flag)
{
    F &= ~flag;
}

static inline void alter_flag(Byte flag, bool state)
{
    if (state)
    {
        set_flag(flag);
    }
    else
    {
        reset_flag(flag);
    }
}

static inline void tick(Word n)
{
    z80.regs.t = n;
}

#endif
