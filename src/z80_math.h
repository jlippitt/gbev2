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
    Byte tmp = next_byte();
    debug("ADD A,$%02X", tmp);
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
    Byte tmp = next_byte();
    debug("ADC A,$%02X", tmp);
    tmp += isset_flag(CARRY);
    A += tmp; \
    alter_flag(ZERO, A == 0);
    reset_flag(NEGATIVE);
    alter_flag(HALF_CARRY, (A & 0xF) < (tmp & 0xF));
    alter_flag(CARRY, A < tmp);
    tick(8);
}

// Subtract r1 from A

#define DEF_SUBAr(r1) \
void SUBA##r1() \
{ \
    debug("SUB A," #r1); \
    A -= r1; \
    alter_flag(ZERO, A == 0); \
    set_flag(NEGATIVE); \
    alter_flag(HALF_CARRY, (A & 0xF) > (r1 & 0xF)); \
    alter_flag(CARRY, A > r1); \
    tick(4); \
}

DEF_SUBAr(A);
DEF_SUBAr(B);
DEF_SUBAr(C);
DEF_SUBAr(D);
DEF_SUBAr(E);
DEF_SUBAr(H);
DEF_SUBAr(L);

// Subtract (HL) from A

void SUBAHL()
{
    debug("SUB A,HL");
    Byte tmp = mmu_getbyte(HL);
    A -= tmp;
    alter_flag(ZERO, A == 0);
    set_flag(NEGATIVE);
    alter_flag(HALF_CARRY, (A & 0xF) > (tmp & 0xF));
    alter_flag(CARRY, A > tmp);
    tick(8);
}

// Subtract n from A

void SUBAn()
{
    Byte tmp = next_byte();
    debug("SUB A,$%02X", tmp);
    A -= tmp;
    alter_flag(ZERO, A == 0);
    set_flag(NEGATIVE);
    alter_flag(HALF_CARRY, (A & 0xF) > (tmp & 0xF));
    alter_flag(CARRY, A > tmp);
    tick(8);
}

// Subtract r1 + carry from A

#define DEF_SBCAr(r1) \
void SBCA##r1() \
{ \
    debug("SBC A," #r1); \
    Byte tmp = r1 + isset_flag(CARRY); \
    A -= tmp; \
    alter_flag(ZERO, A == 0); \
    set_flag(NEGATIVE); \
    alter_flag(HALF_CARRY, (A & 0xF) > (tmp & 0xF)); \
    alter_flag(CARRY, A > tmp); \
    tick(4); \
}

DEF_SBCAr(A);
DEF_SBCAr(B);
DEF_SBCAr(C);
DEF_SBCAr(D);
DEF_SBCAr(E);
DEF_SBCAr(H);
DEF_SBCAr(L);

// Subtract (HL) + carry from A

void SBCAHL()
{
    debug("SBC A,HL");
    Byte tmp = mmu_getbyte(HL) + isset_flag(CARRY);
    A -= tmp;
    alter_flag(ZERO, A == 0);
    set_flag(NEGATIVE);
    alter_flag(HALF_CARRY, (A & 0xF) > (tmp & 0xF));
    alter_flag(CARRY, A > tmp);
    tick(8);
}

// Subtract n + carry from A

void SBCAn()
{
    Byte tmp = next_byte();
    debug("SBC A,$%02X", tmp);
    tmp += isset_flag(CARRY);
    A -= tmp;
    alter_flag(ZERO, A == 0);
    set_flag(NEGATIVE);
    alter_flag(HALF_CARRY, (A & 0xF) > (tmp & 0xF));
    alter_flag(CARRY, A > tmp);
    tick(8);
}

// AND r1 with A

#define DEF_ANDAr(r1) \
void ANDA##r1() \
{ \
    debug("AND A," #r1); \
    A &= r1; \
    alter_flag(ZERO, A == 0); \
    reset_flag(NEGATIVE); \
    set_flag(HALF_CARRY); \
    reset_flag(CARRY); \
    tick(4); \
}

DEF_ANDAr(A);
DEF_ANDAr(B);
DEF_ANDAr(C);
DEF_ANDAr(D);
DEF_ANDAr(E);
DEF_ANDAr(H);
DEF_ANDAr(L);

// AND A with (HL)

void ANDAHL()
{
    debug("AND A,(HL)");
    A &= mmu_getbyte(HL);
    alter_flag(ZERO, A == 0);
    reset_flag(NEGATIVE);
    set_flag(HALF_CARRY);
    reset_flag(CARRY);
    tick(8);
}

// AND A with n

void ANDAn()
{
    Byte tmp = next_byte();
    debug("AND A,$%02X", tmp);
    A &= tmp;
    alter_flag(ZERO, A == 0);
    reset_flag(NEGATIVE);
    set_flag(HALF_CARRY);
    reset_flag(CARRY);
    tick(8);
}

// OR r1 with A

#define DEF_ORAr(r1) \
void ORA##r1() \
{ \
    debug("OR A," #r1); \
    A |= r1; \
    alter_flag(ZERO, A == 0); \
    reset_flag(NEGATIVE); \
    reset_flag(HALF_CARRY); \
    reset_flag(CARRY); \
    tick(4); \
}

DEF_ORAr(A);
DEF_ORAr(B);
DEF_ORAr(C);
DEF_ORAr(D);
DEF_ORAr(E);
DEF_ORAr(H);
DEF_ORAr(L);

// OR A with (HL)

void ORAHL()
{
    debug("OR A,(HL)");
    A |= mmu_getbyte(HL);
    alter_flag(ZERO, A == 0);
    reset_flag(NEGATIVE);
    reset_flag(HALF_CARRY);
    reset_flag(CARRY);
    tick(8);
}

// OR A with n

void ORAn()
{
    Byte tmp = next_byte();
    debug("OR A,$%02X", tmp);
    A |= tmp;
    alter_flag(ZERO, A == 0);
    reset_flag(NEGATIVE);
    reset_flag(HALF_CARRY);
    reset_flag(CARRY);
    tick(8);
}

// XOR r1 with A

#define DEF_XORAr(r1) \
void XORA##r1() \
{ \
    debug("XOR A," #r1); \
    A ^= r1; \
    alter_flag(ZERO, A == 0); \
    reset_flag(NEGATIVE); \
    reset_flag(HALF_CARRY); \
    reset_flag(CARRY); \
    tick(4); \
}

DEF_XORAr(A);
DEF_XORAr(B);
DEF_XORAr(C);
DEF_XORAr(D);
DEF_XORAr(E);
DEF_XORAr(H);
DEF_XORAr(L);

// XOR A with (HL)

void XORAHL()
{
    debug("XOR A,(HL)");
    A ^= mmu_getbyte(HL);
    alter_flag(ZERO, A == 0);
    reset_flag(NEGATIVE);
    reset_flag(HALF_CARRY);
    reset_flag(CARRY);
    tick(8);
}

// XOR A with n

void XORAn()
{
    Byte tmp = next_byte();
    debug("XOR A,$%02X", tmp);
    A ^= tmp;
    alter_flag(ZERO, A == 0);
    reset_flag(NEGATIVE);
    reset_flag(HALF_CARRY);
    reset_flag(CARRY);
    tick(8);
}

// Compare r1 with A

#define DEF_CPAr(r1) \
void CPA##r1() \
{ \
    debug("CP A," #r1); \
    Byte result = A - r1; \
    alter_flag(ZERO, result == 0); \
    set_flag(NEGATIVE); \
    alter_flag(HALF_CARRY, (result & 0xF) > (r1 & 0xF)); \
    alter_flag(CARRY, result > r1); \
    tick(4); \
}

DEF_CPAr(A);
DEF_CPAr(B);
DEF_CPAr(C);
DEF_CPAr(D);
DEF_CPAr(E);
DEF_CPAr(H);
DEF_CPAr(L);

// Compare (HL) with A

void CPAHL()
{
    debug("CP A,HL");
    Byte tmp = mmu_getbyte(HL);
    Byte result = A - tmp;
    alter_flag(ZERO, result == 0);
    set_flag(NEGATIVE);
    alter_flag(HALF_CARRY, (result & 0xF) > (tmp & 0xF));
    alter_flag(CARRY, result > tmp);
    tick(8);
}

// Compare n with A

void CPAn()
{
    Byte tmp = next_byte();
    debug("CP A,$%02X", tmp);
    Byte result = A - tmp;
    alter_flag(ZERO, result == 0);
    set_flag(NEGATIVE);
    alter_flag(HALF_CARRY, (result & 0xF) > (tmp & 0xF));
    alter_flag(CARRY, result > tmp);
    tick(8);
}

// Increment r1

#define DEF_INCr(r1) \
void INC##r1() \
{ \
    debug("INC " #r1); \
    r1++; \
    alter_flag(ZERO, r1 == 0); \
    reset_flag(NEGATIVE); \
    alter_flag(HALF_CARRY, (r1 & 0xF) == 0); \
    tick(4); \
}

DEF_INCr(A);
DEF_INCr(B);
DEF_INCr(C);
DEF_INCr(D);
DEF_INCr(E);
DEF_INCr(H);
DEF_INCr(L);

// Increment (HL)

void INCHLm()
{
    debug("INC (HL)");
    Byte tmp = mmu_getbyte(HL) + 1;
    mmu_putbyte(HL, tmp);
    alter_flag(ZERO, tmp == 0);
    reset_flag(NEGATIVE);
    alter_flag(HALF_CARRY, (tmp & 0xF) == 0);
    tick(12);
}

// Decrement r1

#define DEF_DECr(r1) \
void DEC##r1() \
{ \
    debug("DEC " #r1); \
    r1--; \
    alter_flag(ZERO, r1 == 0); \
    set_flag(NEGATIVE); \
    alter_flag(HALF_CARRY, (r1 & 0xF) == 0xF); \
    tick(4); \
}

DEF_DECr(A);
DEF_DECr(B);
DEF_DECr(C);
DEF_DECr(D);
DEF_DECr(E);
DEF_DECr(H);
DEF_DECr(L);

// Decrement (HL)

void DECHLm()
{
    debug("DEC (HL)");
    Byte tmp = mmu_getbyte(HL) - 1;
    mmu_putbyte(HL, tmp);
    alter_flag(ZERO, tmp == 0);
    set_flag(NEGATIVE);
    alter_flag(HALF_CARRY, (tmp & 0xF) == 0xF);
    tick(12);
}

/*
 * 16-bit arithmetic
 */

// Add r1 to HL

#define DEF_ADDHLr(r1) \
void ADDHL##r1() \
{ \
    debug("ADD HL," #r1); \
    HL += r1; \
    reset_flag(NEGATIVE); \
    alter_flag(CARRY, HL < r1); \
    alter_flag(HALF_CARRY, (HL & 0xFFF) < (r1 & 0xFFF)); \
    tick(8); \
}

DEF_ADDHLr(BC);
DEF_ADDHLr(DE);
DEF_ADDHLr(HL);
DEF_ADDHLr(SP);

// Add n to SP

void ADDSPn()
{
    Byte tmp = next_byte(); \
    debug("ADD SP,%d", (int8_t)tmp);
    int32_t result = SP + (int8_t)tmp; \
    SP = result & 0x0000FFFF; \
    reset_flag(ZERO);
    reset_flag(NEGATIVE);
    alter_flag(HALF_CARRY, ((SP & 0xF) + (tmp & 0xF)) > 0xF);
    alter_flag(CARRY, result < 0 || result > 0xFFFF);
    tick(16);
}

// Increment r1

#define DEF_INCrr(r1) \
void INC##r1() \
{ \
    debug("INC" #r1); \
    r1++; \
    tick(8); \
}

DEF_INCrr(BC);
DEF_INCrr(DE);
DEF_INCrr(HL);
DEF_INCrr(SP);

// Decrement r1

#define DEF_DECrr(r1) \
void DEC##r1() \
{ \
    debug("DEC" #r1); \
    r1--; \
    tick(8); \
}

DEF_DECrr(BC);
DEF_DECrr(DE);
DEF_DECrr(HL);
DEF_DECrr(SP);

#endif
