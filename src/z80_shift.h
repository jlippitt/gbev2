#ifndef Z80_SHIFT_H
#define Z80_SHIFT_H

#include "z80_macros.h"

// Swap upper and lower nibbles of r1

#define DEF_SWAPr(r1) \
void SWAP##r1() \
{ \
    debug("SWAP " #r1); \
    r1 = (r1 << 4) + (r1 >> 4); \
    alter_flag(ZERO, r1 == 0); \
    reset_flag(NEGATIVE); \
    reset_flag(HALF_CARRY); \
    reset_flag(CARRY); \
    tick(8); \
}

DEF_SWAPr(A);
DEF_SWAPr(B);
DEF_SWAPr(C);
DEF_SWAPr(D);
DEF_SWAPr(E);
DEF_SWAPr(H);
DEF_SWAPr(L);

// Swap upper and lower nibbles of (HL)

void SWAPHL()
{
    debug("SWAP (HL)");
    Byte tmp = mmu_getbyte(HL);
    tmp = (tmp << 4) + (tmp >> 4);
    mmu_putbyte(HL, tmp);
    alter_flag(ZERO, tmp == 0);
    reset_flag(NEGATIVE);
    reset_flag(HALF_CARRY);
    reset_flag(CARRY);
    tick(16);
}

// Rotate A left

void RLCA()
{
    debug("RLCA");
    Byte carry = A & 0x80;
    A = (A << 1) + (carry ? 1 : 0);
    reset_flag(ZERO);
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
    reset_flag(ZERO);
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
    reset_flag(ZERO);
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
    reset_flag(ZERO);
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
    mmu_putbyte(HL, tmp);
    alter_flag(ZERO, tmp == 0);
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
    mmu_putbyte(HL, tmp);
    alter_flag(ZERO, tmp == 0);
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
    mmu_putbyte(HL, tmp);
    alter_flag(ZERO, tmp == 0);
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
    mmu_putbyte(HL, tmp);
    alter_flag(ZERO, tmp == 0);
    reset_flag(NEGATIVE);
    reset_flag(HALF_CARRY);
    alter_flag(CARRY, carry);
    tick(16);
}

// Shift A left, LSB set to 0

#define DEF_SLAr(r1) \
void SLA##r1() \
{ \
    debug("SLA " #r1); \
    alter_flag(CARRY, r1 & 0x80); \
    r1 <<= 1; \
    alter_flag(ZERO, r1 == 0); \
    reset_flag(NEGATIVE); \
    reset_flag(HALF_CARRY); \
    tick(8); \
}

DEF_SLAr(A);
DEF_SLAr(B);
DEF_SLAr(C);
DEF_SLAr(D);
DEF_SLAr(E);
DEF_SLAr(H);
DEF_SLAr(L);

// Shift (HL) left, LSB set to 0

void SLAHL()
{
    debug("SLA (HL)");
    Byte tmp = mmu_getbyte(HL);
    alter_flag(CARRY, tmp & 0x80);
    tmp <<= 1;
    mmu_putbyte(HL, tmp);
    alter_flag(ZERO, tmp == 0);
    reset_flag(NEGATIVE);
    reset_flag(HALF_CARRY);
    tick(16);
}

// Shift A right, MSB doesn't change

#define DEF_SRAr(r1) \
void SRA##r1() \
{ \
    debug("SRA " #r1); \
    alter_flag(CARRY, r1 & 1); \
    r1 = (r1 & 0x80) + (r1 >> 1); \
    alter_flag(ZERO, r1 == 0); \
    reset_flag(NEGATIVE); \
    reset_flag(HALF_CARRY); \
    tick(8); \
}

DEF_SRAr(A);
DEF_SRAr(B);
DEF_SRAr(C);
DEF_SRAr(D);
DEF_SRAr(E);
DEF_SRAr(H);
DEF_SRAr(L);

// Shift (HL) right, MSB doesn't change

void SRAHL()
{
    debug("SRA (HL)");
    Byte tmp = mmu_getbyte(HL);
    alter_flag(CARRY, tmp & 1);
    tmp = (tmp & 0x80) + (tmp >> 1);
    mmu_putbyte(HL, tmp);
    alter_flag(ZERO, tmp == 0);
    reset_flag(NEGATIVE);
    reset_flag(HALF_CARRY);
    tick(16);
}

// Shift A right, MSB set to 0

#define DEF_SRLr(r1) \
void SRL##r1() \
{ \
    debug("SRL " #r1); \
    alter_flag(CARRY, r1 & 1); \
    r1 >>= 1; \
    alter_flag(ZERO, r1 == 0); \
    reset_flag(NEGATIVE); \
    reset_flag(HALF_CARRY); \
    tick(8); \
}

DEF_SRLr(A);
DEF_SRLr(B);
DEF_SRLr(C);
DEF_SRLr(D);
DEF_SRLr(E);
DEF_SRLr(H);
DEF_SRLr(L);

// Shift (HL) right, MSB set to 0

void SRLHL()
{
    debug("SRL (HL)");
    Byte tmp = mmu_getbyte(HL);
    alter_flag(CARRY, tmp & 1);
    tmp >>= 1;
    mmu_putbyte(HL, tmp);
    alter_flag(ZERO, tmp == 0);
    reset_flag(NEGATIVE);
    reset_flag(HALF_CARRY);
    tick(16);
}

#endif
