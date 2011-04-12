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
    debug("LD " #r1 ",n"); \
    r1 = next_byte(); \
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
    debug("LD (HL),n");
    mmu_putbyte(HL, next_byte());
    tick(12);
}

// Load (nn) into A

void LDAnn()
{
    debug("LD A,(nn)");
    A = mmu_getbyte(next_word());
    tick(16);
}

// Load A into (nn)

void LDnnA()
{
    debug("LD (nn),A");
    mmu_putbyte(next_word(), A);
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
    debug("LDH ($FF00+n),A");
    mmu_putbyte(0xFF00 + next_byte(), A);
    tick(12);
}

// Load ($FF00 + n) into A

void LDHAn()
{
    debug("LDH A,($FF00+n)");
    A = mmu_getbyte(0xFF00 + next_byte());
    tick(12);
}

/*
 * 16-bit loads
 */

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

#endif
