#ifndef Z80_BIT_H
#define Z80_BIT_H

#include "z80_macros.h"

// Test nth bit of r1

#define DEF_BITnr(n, r1) \
void BIT##n##r1() \
{ \
    debug("BIT " #n "," #r1); \
    alter_flag(ZERO, (r1 & (1 << n)) == 0); \
    reset_flag(NEGATIVE); \
    set_flag(HALF_CARRY); \
    tick(2); \
}

DEF_BITnr(0, A);
DEF_BITnr(0, B);
DEF_BITnr(0, C);
DEF_BITnr(0, D);
DEF_BITnr(0, E);
DEF_BITnr(0, H);
DEF_BITnr(0, L);
DEF_BITnr(1, A);
DEF_BITnr(1, B);
DEF_BITnr(1, C);
DEF_BITnr(1, D);
DEF_BITnr(1, E);
DEF_BITnr(1, H);
DEF_BITnr(1, L);
DEF_BITnr(2, A);
DEF_BITnr(2, B);
DEF_BITnr(2, C);
DEF_BITnr(2, D);
DEF_BITnr(2, E);
DEF_BITnr(2, H);
DEF_BITnr(2, L);
DEF_BITnr(3, A);
DEF_BITnr(3, B);
DEF_BITnr(3, C);
DEF_BITnr(3, D);
DEF_BITnr(3, E);
DEF_BITnr(3, H);
DEF_BITnr(3, L);
DEF_BITnr(4, A);
DEF_BITnr(4, B);
DEF_BITnr(4, C);
DEF_BITnr(4, D);
DEF_BITnr(4, E);
DEF_BITnr(4, H);
DEF_BITnr(4, L);
DEF_BITnr(5, A);
DEF_BITnr(5, B);
DEF_BITnr(5, C);
DEF_BITnr(5, D);
DEF_BITnr(5, E);
DEF_BITnr(5, H);
DEF_BITnr(5, L);
DEF_BITnr(6, A);
DEF_BITnr(6, B);
DEF_BITnr(6, C);
DEF_BITnr(6, D);
DEF_BITnr(6, E);
DEF_BITnr(6, H);
DEF_BITnr(6, L);
DEF_BITnr(7, A);
DEF_BITnr(7, B);
DEF_BITnr(7, C);
DEF_BITnr(7, D);
DEF_BITnr(7, E);
DEF_BITnr(7, H);
DEF_BITnr(7, L);

// Test nth bit of (HL)

#define DEF_BITnHL(n) \
void BIT##n##HL() \
{ \
    debug("BIT " #n ",(HL)"); \
    alter_flag(ZERO, (mmu_getbyte(HL) & (1 << n)) == 0); \
    reset_flag(NEGATIVE); \
    set_flag(HALF_CARRY); \
    tick(4); \
}

DEF_BITnHL(0);
DEF_BITnHL(1);
DEF_BITnHL(2);
DEF_BITnHL(3);
DEF_BITnHL(4);
DEF_BITnHL(5);
DEF_BITnHL(6);
DEF_BITnHL(7);

// Reset nth bit of r1

#define DEF_RESnr(n, r1) \
void RES##n##r1() \
{ \
    debug("RES " #n "," #r1); \
    r1 &= ~(1 << n); \
    tick(2); \
}

DEF_RESnr(0, A);
DEF_RESnr(0, B);
DEF_RESnr(0, C);
DEF_RESnr(0, D);
DEF_RESnr(0, E);
DEF_RESnr(0, H);
DEF_RESnr(0, L);
DEF_RESnr(1, A);
DEF_RESnr(1, B);
DEF_RESnr(1, C);
DEF_RESnr(1, D);
DEF_RESnr(1, E);
DEF_RESnr(1, H);
DEF_RESnr(1, L);
DEF_RESnr(2, A);
DEF_RESnr(2, B);
DEF_RESnr(2, C);
DEF_RESnr(2, D);
DEF_RESnr(2, E);
DEF_RESnr(2, H);
DEF_RESnr(2, L);
DEF_RESnr(3, A);
DEF_RESnr(3, B);
DEF_RESnr(3, C);
DEF_RESnr(3, D);
DEF_RESnr(3, E);
DEF_RESnr(3, H);
DEF_RESnr(3, L);
DEF_RESnr(4, A);
DEF_RESnr(4, B);
DEF_RESnr(4, C);
DEF_RESnr(4, D);
DEF_RESnr(4, E);
DEF_RESnr(4, H);
DEF_RESnr(4, L);
DEF_RESnr(5, A);
DEF_RESnr(5, B);
DEF_RESnr(5, C);
DEF_RESnr(5, D);
DEF_RESnr(5, E);
DEF_RESnr(5, H);
DEF_RESnr(5, L);
DEF_RESnr(6, A);
DEF_RESnr(6, B);
DEF_RESnr(6, C);
DEF_RESnr(6, D);
DEF_RESnr(6, E);
DEF_RESnr(6, H);
DEF_RESnr(6, L);
DEF_RESnr(7, A);
DEF_RESnr(7, B);
DEF_RESnr(7, C);
DEF_RESnr(7, D);
DEF_RESnr(7, E);
DEF_RESnr(7, H);
DEF_RESnr(7, L);

// Reset nth bit of (HL)

#define DEF_RESnHL(n) \
void RES##n##HL() \
{ \
    debug("RES " #n ",(HL)"); \
    mmu_putbyte(HL, mmu_getbyte(HL) & ~(1 << n)); \
    tick(4); \
}

DEF_RESnHL(0);
DEF_RESnHL(1);
DEF_RESnHL(2);
DEF_RESnHL(3);
DEF_RESnHL(4);
DEF_RESnHL(5);
DEF_RESnHL(6);
DEF_RESnHL(7);

// Set nth bit of r1

#define DEF_SETnr(n, r1) \
void SET##n##r1() \
{ \
    debug("SET " #n "," #r1); \
    r1 |= (1 << n); \
    tick(2); \
}

DEF_SETnr(0, A);
DEF_SETnr(0, B);
DEF_SETnr(0, C);
DEF_SETnr(0, D);
DEF_SETnr(0, E);
DEF_SETnr(0, H);
DEF_SETnr(0, L);
DEF_SETnr(1, A);
DEF_SETnr(1, B);
DEF_SETnr(1, C);
DEF_SETnr(1, D);
DEF_SETnr(1, E);
DEF_SETnr(1, H);
DEF_SETnr(1, L);
DEF_SETnr(2, A);
DEF_SETnr(2, B);
DEF_SETnr(2, C);
DEF_SETnr(2, D);
DEF_SETnr(2, E);
DEF_SETnr(2, H);
DEF_SETnr(2, L);
DEF_SETnr(3, A);
DEF_SETnr(3, B);
DEF_SETnr(3, C);
DEF_SETnr(3, D);
DEF_SETnr(3, E);
DEF_SETnr(3, H);
DEF_SETnr(3, L);
DEF_SETnr(4, A);
DEF_SETnr(4, B);
DEF_SETnr(4, C);
DEF_SETnr(4, D);
DEF_SETnr(4, E);
DEF_SETnr(4, H);
DEF_SETnr(4, L);
DEF_SETnr(5, A);
DEF_SETnr(5, B);
DEF_SETnr(5, C);
DEF_SETnr(5, D);
DEF_SETnr(5, E);
DEF_SETnr(5, H);
DEF_SETnr(5, L);
DEF_SETnr(6, A);
DEF_SETnr(6, B);
DEF_SETnr(6, C);
DEF_SETnr(6, D);
DEF_SETnr(6, E);
DEF_SETnr(6, H);
DEF_SETnr(6, L);
DEF_SETnr(7, A);
DEF_SETnr(7, B);
DEF_SETnr(7, C);
DEF_SETnr(7, D);
DEF_SETnr(7, E);
DEF_SETnr(7, H);
DEF_SETnr(7, L);

// Set nth bit of (HL)

#define DEF_SETnHL(n) \
void SET##n##HL() \
{ \
    debug("SET " #n ",(HL)"); \
    mmu_putbyte(HL, mmu_getbyte(HL) | (1 << n)); \
    tick(4); \
}

DEF_SETnHL(0);
DEF_SETnHL(1);
DEF_SETnHL(2);
DEF_SETnHL(3);
DEF_SETnHL(4);
DEF_SETnHL(5);
DEF_SETnHL(6);
DEF_SETnHL(7);

#endif
