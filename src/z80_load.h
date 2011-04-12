#ifndef Z80_LOAD_H
#define Z80_LOAD_H

#include "z80_macros.h"

/*
 * 8-bit loads
 */

// Load r2 into r1

#define DEF_LDrr(r1, r2) \
void LD##r1##r2() \
{ \
    debug("LD " #r1 "," #r2); \
    r1 = r2; \
    tick(4); \
}

DEF_LDrr(A, A);
DEF_LDrr(A, B);
DEF_LDrr(A, C);
DEF_LDrr(A, D);
DEF_LDrr(A, E);
DEF_LDrr(A, H);
DEF_LDrr(A, L);
DEF_LDrr(B, A);
DEF_LDrr(B, B);
DEF_LDrr(B, C);
DEF_LDrr(B, D);
DEF_LDrr(B, E);
DEF_LDrr(B, H);
DEF_LDrr(B, L);
DEF_LDrr(C, A);
DEF_LDrr(C, B);
DEF_LDrr(C, C);
DEF_LDrr(C, D);
DEF_LDrr(C, E);
DEF_LDrr(C, H);
DEF_LDrr(C, L);
DEF_LDrr(D, A);
DEF_LDrr(D, B);
DEF_LDrr(D, C);
DEF_LDrr(D, D);
DEF_LDrr(D, E);
DEF_LDrr(D, H);
DEF_LDrr(D, L);
DEF_LDrr(E, A);
DEF_LDrr(E, B);
DEF_LDrr(E, C);
DEF_LDrr(E, D);
DEF_LDrr(E, E);
DEF_LDrr(E, H);
DEF_LDrr(E, L);
DEF_LDrr(H, A);
DEF_LDrr(H, B);
DEF_LDrr(H, C);
DEF_LDrr(H, D);
DEF_LDrr(H, E);
DEF_LDrr(H, H);
DEF_LDrr(H, L);
DEF_LDrr(L, A);
DEF_LDrr(L, B);
DEF_LDrr(L, C);
DEF_LDrr(L, D);
DEF_LDrr(L, E);
DEF_LDrr(L, H);
DEF_LDrr(L, L);

// Load n into r1

#define DEF_LDrn(r1) \
void LD##r1##n() \
{ \
    r1 = next_byte(); \
    debug("LD " #r1 ",$%02X", r1); \
    tick(8); \
}

DEF_LDrn(A);
DEF_LDrn(B);
DEF_LDrn(C);
DEF_LDrn(D);
DEF_LDrn(E);
DEF_LDrn(H);
DEF_LDrn(L);

// Load (r2) into r1

#define DEF_LDrm(r1, r2) \
void LD##r1##r2() \
{ \
    debug("LD " #r1 ",(" #r2 ")"); \
    r1 = mmu_getbyte(r2); \
    tick(8); \
}

DEF_LDrm(A, BC);
DEF_LDrm(A, DE);
DEF_LDrm(A, HL);
DEF_LDrm(B, HL);
DEF_LDrm(C, HL);
DEF_LDrm(D, HL);
DEF_LDrm(E, HL);
DEF_LDrm(H, HL);
DEF_LDrm(L, HL);

// Load r2 into (r1)

#define DEF_LDmr(r1, r2) \
void LD##r1##r2() \
{ \
    debug("LD (" #r1 ")," #r2); \
    mmu_putbyte(r1, r2); \
    tick(8); \
}

DEF_LDmr(BC, A);
DEF_LDmr(DE, A);
DEF_LDmr(HL, A);
DEF_LDmr(HL, B);
DEF_LDmr(HL, C);
DEF_LDmr(HL, D);
DEF_LDmr(HL, E);
DEF_LDmr(HL, H);
DEF_LDmr(HL, L);

// Load n into (HL)

void LDHLn()
{
    Byte tmp = next_byte();
    debug("LD (HL),$%02X", tmp);
    mmu_putbyte(HL, tmp);
    tick(12);
}

// Load (nn) into A

void LDAnn()
{
    Word tmp = next_word();
    debug("LD A,($%04X)", tmp);
    A = mmu_getbyte(tmp);
    tick(16);
}

// Load A into (nn)

void LDnnA()
{
    Word tmp = next_word();
    debug("LD ($%04X),A", tmp);
    mmu_putbyte(tmp, A);
    tick(16);
}

// Load ($FF00 + C) into A

void LDHAC()
{
    debug("LD A,($FF00+C)");
    A = mmu_getbyte(0xFF00 + C);
    tick(8);
}

// Load A into ($FF00 + C)

void LDHCA()
{
    debug("LD ($FF00+C),A");
    mmu_putbyte(0xFF00 + C, A);
    tick(8);
}

// Load (HL) into A and decrement HL

void LDDAHL()
{
    debug("LDD A,(HL)");
    A = mmu_getbyte(HL--);
    tick(8);
}

// Load A into (HL) and decrement HL

void LDDHLA()
{
    debug("LDD (HL),A");
    mmu_putbyte(HL--, A);
    tick(8);
}

// Load (HL) into A and increment HL

void LDIAHL()
{
    debug("LDD A,(HL)");
    A = mmu_getbyte(HL++);
    tick(8);
}

// Load A into (HL) and increment HL

void LDIHLA()
{
    debug("LDD (HL),A");
    mmu_putbyte(HL++, A);
    tick(8);
}

// Load A into ($FF00 + n)

void LDHnA()
{
    Byte tmp = next_byte();
    debug("LDH ($FF00+$%02X),A", tmp);
    mmu_putbyte(0xFF00 + tmp, A);
    tick(12);
}

// Load ($FF00 + n) into A

void LDHAn()
{
    Byte tmp = next_byte();
    debug("LDH A,($FF00+$%02X)", tmp);
    A = mmu_getbyte(0xFF00 + tmp);
    tick(12);
}

/*
 * 16-bit loads
 */

// Load nn into r1

#define DEF_LDrrnn(r1) \
void LD##r1##nn() \
{ \
    r1 = next_word(); \
    debug("LD " #r1 ",$%04X", r1); \
    tick(12); \
}

DEF_LDrrnn(BC);
DEF_LDrrnn(DE);
DEF_LDrrnn(HL);
DEF_LDrrnn(SP);

// Load HL into SP

void LDSPHL()
{
    debug("LD SP,HL");
    SP = HL;
    tick(8);
}

// Load SP + n into HL

void LDHLSPn()
{
    Byte tmp = next_byte(); \
    debug("LD HL,SP+%d", (int8_t)tmp);
    int32_t result = SP + (int8_t)tmp; \
    HL = result & 0x0000FFFF; \
    reset_flag(ZERO);
    reset_flag(NEGATIVE);
    alter_flag(HALF_CARRY, ((SP & 0xF) + (tmp & 0xF)) > 0xF);
    alter_flag(CARRY, result < 0 || result > 0xFFFF);
    tick(12);
}

// Load SP into (nn)

void LDnnSP()
{
    Word tmp = next_word();
    debug("LD ($%04X),SP", tmp);
    mmu_putbyte(tmp, SP);
    tick(20);
}

// Push r1 onto stack

#define DEF_PUSHrr(r1) \
void PUSH##r1() \
{ \
    debug("PUSH " #r1); \
    push(r1); \
    tick(16); \
}

DEF_PUSHrr(AF);
DEF_PUSHrr(BC);
DEF_PUSHrr(DE);
DEF_PUSHrr(HL);

// Pop r1 from stack

#define DEF_POPrr(r1) \
void POP##r1() \
{ \
    debug("POP " #r1); \
    r1 = pop(); \
    tick(12); \
}

DEF_POPrr(AF);
DEF_POPrr(BC);
DEF_POPrr(DE);
DEF_POPrr(HL);

#endif
