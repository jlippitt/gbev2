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
    Byte tmp = r1; \
    A += tmp; \
    alter_flag(ZERO, A == 0); \
    reset_flag(NEGATIVE); \
    alter_flag(HALF_CARRY, (A & 0xF) < (tmp & 0xF)); \
    alter_flag(CARRY, A < tmp); \
    tick(1); \
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
    tick(2);
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
    tick(2);
}

// Add r1 + carry to A

#define DEF_ADCAr(r1) \
void ADCA##r1() \
{ \
    debug("ADC A," #r1); \
    Byte tmp = r1; \
    Byte carry = isset_flag(CARRY); \
    A += tmp; \
    alter_flag(HALF_CARRY, (A & 0xF) < (tmp & 0xF)); \
    alter_flag(CARRY, A < tmp); \
    \
    if (carry) \
    { \
        A += carry; \
        F |= ((A & 0xF) == 0) ? HALF_CARRY : 0; \
        F |= (A == 0) ? CARRY : 0; \
    } \
    \
    alter_flag(ZERO, A == 0); \
    reset_flag(NEGATIVE); \
    tick(1); \
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
    Byte tmp = mmu_getbyte(HL);
    Byte carry = isset_flag(CARRY);
    A += tmp; \
    alter_flag(HALF_CARRY, (A & 0xF) < (tmp & 0xF));
    alter_flag(CARRY, A < tmp);

    if (carry)
    {
        A += carry;
        F |= ((A & 0xF) == 0) ? HALF_CARRY : 0;
        F |= (A == 0) ? CARRY : 0;
    }

    alter_flag(ZERO, A == 0);
    reset_flag(NEGATIVE);
    tick(2);
}

// ADD n + carry to A

void ADCAn()
{
    Byte tmp = next_byte();
    debug("ADC A,$%02X", tmp);
    Byte carry = isset_flag(CARRY);
    A += tmp;
    alter_flag(HALF_CARRY, (A & 0xF) < (tmp & 0xF));
    alter_flag(CARRY, A < tmp);

    if (carry)
    {
        A += carry;
        F |= ((A & 0xF) == 0) ? HALF_CARRY : 0;
        F |= (A == 0) ? CARRY : 0;
    }

    alter_flag(ZERO, A == 0);
    reset_flag(NEGATIVE);
    tick(2);
}

// Subtract r1 from A

#define DEF_SUBAr(r1) \
void SUBA##r1() \
{ \
    debug("SUB A," #r1); \
    Byte result = A - r1; \
    alter_flag(ZERO, result == 0); \
    set_flag(NEGATIVE); \
    alter_flag(HALF_CARRY, (result & 0xF) > (A & 0xF)); \
    alter_flag(CARRY, result > A); \
    A = result; \
    tick(1); \
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
    Byte result = A - tmp;
    alter_flag(ZERO, result == 0);
    set_flag(NEGATIVE);
    alter_flag(HALF_CARRY, (result & 0xF) > (A & 0xF));
    alter_flag(CARRY, result > A);
    A = result;
    tick(2);
}

// Subtract n from A

void SUBAn()
{
    Byte tmp = next_byte();
    debug("SUB A,$%02X", tmp);
    Byte result = A - tmp;
    alter_flag(ZERO, result == 0);
    set_flag(NEGATIVE);
    alter_flag(HALF_CARRY, (result & 0xF) > (A & 0xF));
    alter_flag(CARRY, result > A);
    A = result;
    tick(2);
}

// Subtract r1 + carry from A

#define DEF_SBCAr(r1) \
void SBCA##r1() \
{ \
    debug("SBC A," #r1); \
    Byte carry = isset_flag(CARRY); \
    Byte result = A - r1; \
    alter_flag(HALF_CARRY, (result & 0xF) > (A & 0xF)); \
    alter_flag(CARRY, result > A); \
    \
    if (carry) \
    { \
        result -= carry; \
        F |= ((result & 0xF) == 0xF) ? HALF_CARRY : 0; \
        F |= (result == 0xFF) ? CARRY : 0; \
    } \
    \
    A = result; \
    alter_flag(ZERO, result == 0); \
    set_flag(NEGATIVE); \
    tick(1); \
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
    Byte carry = isset_flag(CARRY);
    Byte result = A - mmu_getbyte(HL);
    alter_flag(HALF_CARRY, (result & 0xF) > (A & 0xF));
    alter_flag(CARRY, result > A);

    if (carry)
    {
        result -= carry;
        F |= ((result & 0xF) == 0xF) ? HALF_CARRY : 0;
        F |= (result == 0xFF) ? CARRY : 0;
    }

    A = result;
    alter_flag(ZERO, result == 0);
    set_flag(NEGATIVE);
    tick(2);
}

// Subtract n + carry from A

void SBCAn()
{
    Byte tmp = next_byte();
    debug("SBC A,$%02X", tmp);
    Byte carry = isset_flag(CARRY);
    Byte result = A - tmp;
    alter_flag(HALF_CARRY, (result & 0xF) > (A & 0xF));
    alter_flag(CARRY, result > A);

    if (carry)
    {
        result -= carry;
        F |= ((result & 0xF) == 0xF) ? HALF_CARRY : 0;
        F |= (result == 0xFF) ? CARRY : 0;
    }

    A = result;
    alter_flag(ZERO, result == 0);
    set_flag(NEGATIVE);
    tick(2);
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
    tick(1); \
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
    tick(2);
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
    tick(2);
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
    tick(1); \
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
    tick(2);
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
    tick(2);
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
    tick(1); \
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
    tick(2);
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
    tick(2);
}

// Compare r1 with A

#define DEF_CPAr(r1) \
void CPA##r1() \
{ \
    debug("CP A," #r1); \
    Byte result = A - r1; \
    alter_flag(ZERO, result == 0); \
    set_flag(NEGATIVE); \
    alter_flag(HALF_CARRY, (result & 0xF) > (A & 0xF)); \
    alter_flag(CARRY, result > A); \
    tick(1); \
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
    alter_flag(HALF_CARRY, (result & 0xF) > (A & 0xF));
    alter_flag(CARRY, result > A);
    tick(2);
}

// Compare n with A

void CPAn()
{
    Byte tmp = next_byte();
    debug("CP A,$%02X", tmp);
    Byte result = A - tmp;
    alter_flag(ZERO, result == 0);
    set_flag(NEGATIVE);
    alter_flag(HALF_CARRY, (result & 0xF) > (A & 0xF));
    alter_flag(CARRY, result > A);
    tick(2);
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
    tick(1); \
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
    tick(3);
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
    tick(1); \
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
    tick(3);
}

/*
 * 16-bit arithmetic
 */

// Add r1 to HL

#define DEF_ADDHLr(r1) \
void ADDHL##r1() \
{ \
    debug("ADD HL," #r1); \
    Word tmp = r1; \
    HL += tmp; \
    reset_flag(NEGATIVE); \
    alter_flag(CARRY, HL < tmp); \
    alter_flag(HALF_CARRY, (HL & 0xFFF) < (tmp & 0xFFF)); \
    tick(2); \
}

DEF_ADDHLr(BC);
DEF_ADDHLr(DE);
DEF_ADDHLr(HL);
DEF_ADDHLr(SP);

// Add n to SP

void ADDSPn()
{
    Byte tmp = next_byte();
    debug("ADD SP,%d", (int8_t)tmp);
    Byte orig = SP;
    SP = SP + (int8_t)tmp;
    reset_flag(ZERO);
    reset_flag(NEGATIVE);
    alter_flag(HALF_CARRY, ((orig & 0xF) + (tmp & 0xF)) > 0xF);
    alter_flag(CARRY, ((orig & 0xFF) + (tmp & 0xFF)) > 0xFF);
    tick(4);
}

// Increment r1

#define DEF_INCrr(r1) \
void INC##r1() \
{ \
    debug("INC" #r1); \
    r1++; \
    tick(2); \
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
    tick(2); \
}

DEF_DECrr(BC);
DEF_DECrr(DE);
DEF_DECrr(HL);
DEF_DECrr(SP);

#endif
