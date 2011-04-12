#ifndef Z80_MATH_H
#define Z80_MATH_H

#include "z80_macros.h"

/*
 * 8-bit ALU
 */

// Add r1 to A

#define DEF_ADDAr(r1) \
void ADDA##r1() \
{ \
    debug("ADD A," #r1); \
    A += r1; \
    alter_flag(ZERO, A == 0); \
    reset_flag(NEGATIVE); \
    alter_flag(HALF_CARRY, (A & 0xF) < (r1 & 0xF)); \
    alter_flag(CARRY, A < r1); \
    tick(4); \
}

DEF_ADDAr(A);
DEF_ADDAr(B);
DEF_ADDAr(C);
DEF_ADDAr(D);
DEF_ADDAr(E);
DEF_ADDAr(H);
DEF_ADDAr(L);

// Add (HL) to A

void ADDAHL()
{
    debug("ADD A,(HL)");
    Byte tmp = mmu_getbyte(HL);
    A += tmp; \
    alter_flag(ZERO, A == 0);
    reset_flag(NEGATIVE);
    alter_flag(HALF_CARRY, (A & 0xF) < (tmp & 0xF));
    alter_flag(CARRY, A < tmp);
    tick(8);
}

// ADD n to A

void ADDAn()
{
    debug("ADD A,n");
    Byte tmp = next_byte();
    A += tmp; \
    alter_flag(ZERO, A == 0);
    reset_flag(NEGATIVE);
    alter_flag(HALF_CARRY, (A & 0xF) < (tmp & 0xF));
    alter_flag(CARRY, A < tmp);
    tick(8);
}

// Add r1 + carry to A

#define DEF_ADCAr(r1) \
void ADCA##r1() \
{ \
    debug("ADC A," #r1); \
    Byte tmp = r1 + isset_flag(CARRY); \
    A += tmp; \
    alter_flag(ZERO, A == 0); \
    reset_flag(NEGATIVE); \
    alter_flag(HALF_CARRY, (A & 0xF) < (tmp & 0xF)); \
    alter_flag(CARRY, A < tmp); \
    tick(4); \
}

DEF_ADCAr(A);
DEF_ADCAr(B);
DEF_ADCAr(C);
DEF_ADCAr(D);
DEF_ADCAr(E);
DEF_ADCAr(H);
DEF_ADCAr(L);

// Add (HL) + carry to A

void ADCAHL()
{
    debug("ADC A,(HL)");
    Byte tmp = mmu_getbyte(HL) + isset_flag(CARRY);
    A += tmp; \
    alter_flag(ZERO, A == 0);
    reset_flag(NEGATIVE);
    alter_flag(HALF_CARRY, (A & 0xF) < (tmp & 0xF));
    alter_flag(CARRY, A < tmp);
    tick(8);
}

// ADD n + carry to A

void ADCAn()
{
    debug("ADC A,n");
    Byte tmp = next_byte() + isset_flag(CARRY);
    A += tmp; \
    alter_flag(ZERO, A == 0);
    reset_flag(NEGATIVE);
    alter_flag(HALF_CARRY, (A & 0xF) < (tmp & 0xF));
    alter_flag(CARRY, A < tmp);
    tick(8);
}

void XORA()
{
    debug("XOR A");
    A ^= A;
    tick(4);
}

void INCC()
{
    debug("INC C");
    C++;
    alter_flag(ZERO, C);
    reset_flag(NEGATIVE);
    alter_flag(HALF_CARRY, (C & 0xF) == 0);
    tick(4);
}

#endif
