#ifndef Z80_MISC_H
#define Z80_MISC_H

#include <stdio.h>
#include <stdlib.h>
#include "z80_macros.h"

// Decimal adjust A

void DAA()
{
    debug("DAA");

    if ((A & 0xF) > 0x9 || isset_flag(HALF_CARRY))
    {
        A += 0x6;
    }

    if ((A & 0xF0) > 0x90 || isset_flag(CARRY))
    {
        A += 0x60;
        set_flag(CARRY);
    }
    else
    {
        reset_flag(CARRY);
    }

    alter_flag(ZERO, A == 0);
    reset_flag(HALF_CARRY);
    tick(4);
}

// Complement A

void CPL()
{
    debug("CPL");
    A = ~A;
    set_flag(NEGATIVE);
    set_flag(HALF_CARRY);
    tick(4);
}

// Complement carry flag

void CCF()
{
    debug("CCF");
    reset_flag(NEGATIVE);
    reset_flag(HALF_CARRY);
    alter_flag(CARRY, !isset_flag(CARRY));
    tick(4);
}

// Set carry flag

void SCF()
{
    debug("SCF");
    reset_flag(NEGATIVE);
    reset_flag(HALF_CARRY);
    set_flag(CARRY);
    tick(4);
}

// No operation

void NOP()
{
    debug("NOP");
    tick(4);
}

// Power down CPU until an interrupt occurs

void HALT()
{
    debug("HALT");
    z80.halt = 1;
    tick(4);
}

// Halt CPU and LCD display until button pressed

void STOP()
{
    debug("STOP");
    // TODO
    tick(4);
}

// Disable interrupts after next instruction

void DI()
{
    debug("DI");
    z80.regs.ime = 0;
    tick(4);
}

// Enable interrupts after next instruction

void EI()
{
    debug("EI");
    z80.regs.ime = 1;
    tick(4);
}

// Invalid operation

void XX()
{
    fprintf(stderr, "Operation not implemented: %02X\n", mmu_getbyte(PC - 1));
    exit(1);
}

#endif
