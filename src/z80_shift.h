#ifndef Z80_SHIFT_H
#define Z80_SHIFT_H

#include "z80_macros.h"

// Rotate A left

void RLCA()
{
    debug("RLCA");
    Byte carry = A & 0x80;
    A = (A << 1) + (carry ? 1 : 0);
    alter_flag(ZERO, A == 0);
    reset_flag(NEGATIVE);
    reset_flag(HALF_CARRY);
    alter_flag(CARRY, carry);
    tick(4);
}

// Rotate A left through carry flag

void RLA()
{
    debug("RLA");
    Byte carry = A & 0x80;
    A = (A << 1) + isset_flag(CARRY);
    alter_flag(ZERO, A == 0);
    reset_flag(NEGATIVE);
    reset_flag(HALF_CARRY);
    alter_flag(CARRY, carry);
    tick(4);
}

// Rotate A right

void RRCA()
{
    debug("RRCA");
    Byte carry = A & 1;
    A = (A >> 1) + (carry ? 0x80 : 0);
    alter_flag(ZERO, A == 0);
    reset_flag(NEGATIVE);
    reset_flag(HALF_CARRY);
    alter_flag(CARRY, carry);
    tick(4);
}

// Rotate A right through carry

void RRA()
{
    debug("RRA");
    Byte carry = A & 1;
    A = (A >> 1) + (isset_flag(CARRY) ? 0x80 : 0);
    alter_flag(ZERO, A == 0);
    reset_flag(NEGATIVE);
    reset_flag(HALF_CARRY);
    alter_flag(CARRY, carry);
    tick(4);
}

// Rotate r1 left

#define DEF_RLCr(r1) \
void RLCr##r1() \
{ \
    debug("RLC " #r1); \
    Byte carry = r1 & 0x80; \
    r1 = (r1 << 1) + (carry ? 1 : 0); \
    alter_flag(ZERO, r1 == 0); \
    reset_flag(NEGATIVE); \
    reset_flag(HALF_CARRY); \
    alter_flag(CARRY, carry); \
    tick(8); \
}

DEF_RLCr(A);
DEF_RLCr(B);
DEF_RLCr(C);
DEF_RLCr(D);
DEF_RLCr(E);
DEF_RLCr(H);
DEF_RLCr(L);

// Rotate (HL) left

void RLCHL()
{
    debug("RLC (HL)");
    Byte tmp = mmu_getbyte(HL);
    Byte carry = tmp & 0x80;
    tmp = (tmp << 1) + (carry ? 1 : 0);
    alter_flag(ZERO, tmp == 0);
    mmu_putbyte(HL, tmp);
    reset_flag(NEGATIVE);
    reset_flag(HALF_CARRY);
    alter_flag(CARRY, carry);
    tick(16);
}

// Rotate r1 left through carry

#define DEF_RLr(r1) \
void RLr##r1() \
{ \
    debug("RL " #r1); \
    Byte carry = r1 & 0x80; \
    r1 = (r1 << 1) + isset_flag(CARRY); \
    alter_flag(ZERO, r1 == 0); \
    reset_flag(NEGATIVE); \
    reset_flag(HALF_CARRY); \
    alter_flag(CARRY, carry); \
    tick(8); \
}

DEF_RLr(A);
DEF_RLr(B);
DEF_RLr(C);
DEF_RLr(D);
DEF_RLr(E);
DEF_RLr(H);
DEF_RLr(L);

// Rotate (HL) left through carry

void RLHL()
{
    debug("RL (HL)");
    Byte tmp = mmu_getbyte(HL);
    Byte carry = tmp & 0x80;
    tmp = (tmp << 1) + isset_flag(CARRY);
    alter_flag(ZERO, tmp == 0);
    mmu_putbyte(HL, tmp);
    reset_flag(NEGATIVE);
    reset_flag(HALF_CARRY);
    alter_flag(CARRY, carry);
    tick(16);
}

// Rotate r1 right

#define DEF_RRCr(r1) \
void RRCr##r1() \
{ \
    debug("RRC " #r1); \
    Byte carry = r1 & 1; \
    r1 = (r1 >> 1) + (carry ? 0x80 : 0); \
    alter_flag(ZERO, r1 == 0); \
    reset_flag(NEGATIVE); \
    reset_flag(HALF_CARRY); \
    alter_flag(CARRY, carry); \
    tick(8); \
}

DEF_RRCr(A);
DEF_RRCr(B);
DEF_RRCr(C);
DEF_RRCr(D);
DEF_RRCr(E);
DEF_RRCr(H);
DEF_RRCr(L);

// Rotate (HL) right

void RRCHL()
{
    debug("RRC (HL)");
    Byte tmp = mmu_getbyte(HL);
    Byte carry = tmp & 1;
    tmp = (tmp >> 1) + (carry ? 0x80 : 0);
    alter_flag(ZERO, tmp == 0);
    mmu_putbyte(HL, tmp);
    reset_flag(NEGATIVE);
    reset_flag(HALF_CARRY);
    alter_flag(CARRY, carry);
    tick(16);
}

// Rotate r1 right through carry

#define DEF_RRr(r1) \
void RRr##r1() \
{ \
    debug("RR " #r1); \
    Byte carry = r1 & 1; \
    r1 = (r1 >> 1) + (isset_flag(CARRY) ? 0x80 : 0); \
    alter_flag(ZERO, r1 == 0); \
    reset_flag(NEGATIVE); \
    reset_flag(HALF_CARRY); \
    alter_flag(CARRY, carry); \
    tick(8); \
}

DEF_RRr(A);
DEF_RRr(B);
DEF_RRr(C);
DEF_RRr(D);
DEF_RRr(E);
DEF_RRr(H);
DEF_RRr(L);

// Rotate (HL) right through carry

void RRHL()
{
    debug("RR (HL)");
    Byte tmp = mmu_getbyte(HL);
    Byte carry = tmp & 1;
    tmp = (tmp >> 1) + (isset_flag(CARRY) ? 0x80 : 0);
    alter_flag(ZERO, tmp == 0);
    mmu_putbyte(HL, tmp);
    reset_flag(NEGATIVE);
    reset_flag(HALF_CARRY);
    alter_flag(CARRY, carry);
    tick(16);
}

#endif
