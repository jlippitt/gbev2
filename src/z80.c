#include <stdio.h>
#include <stdlib.h>
#include "z80.h"
#include "z80_bit.h"
#include "z80_jump.h"
#include "z80_load.h"
#include "z80_macros.h"
#include "z80_math.h"
#include "z80_misc.h"

struct Z80 z80 = {{{0}, {0}, {0}, {0}, 0, 0, 0}, {0}};

static void ext_op();

static void (*ops[])() = {
    // 0X
    &NOP,              &LDBCnn,           &LDBCA,            &STOP,
    &STOP,             &STOP,             &LDBn,             &STOP,
    &LDnnSP,           &STOP,             &LDABC,            &STOP,
    &INCC,             &STOP,             &LDCn,             &STOP,
    // 1X
    &STOP,             &LDDEnn,           &LDDEA,            &STOP,
    &STOP,             &STOP,             &LDDn,             &STOP,
    &STOP,             &STOP,             &LDADE,            &STOP,
    &STOP,             &STOP,             &LDEn,             &STOP,
    // 2X
    &JRNZn,            &LDHLnn,           &LDIHLA,           &STOP,
    &STOP,             &STOP,             &LDHn,             &STOP,
    &STOP,             &STOP,             &LDIAHL,           &STOP,
    &STOP,             &STOP,             &LDLn,             &STOP,
    // 3X
    &STOP,             &LDSPnn,           &LDDHLA,           &STOP,
    &STOP,             &STOP,             &LDHLn,            &STOP,
    &STOP,             &STOP,             &LDDAHL,           &STOP,
    &STOP,             &STOP,             &LDAn,             &STOP,
    // 4X
    &LDBB,             &LDBC,             &LDBD,             &LDBE,
    &LDBH,             &LDBL,             &LDBHL,            &LDBA,
    &LDCB,             &LDCC,             &LDCD,             &LDCE,
    &LDCH,             &LDCL,             &LDCHL,            &LDCA,
    // 5X
    &LDDB,             &LDDC,             &LDDD,             &LDDE,
    &LDDH,             &LDDL,             &LDDHL,            &LDDA,
    &LDEB,             &LDEC,             &LDED,             &LDEE,
    &LDEH,             &LDEL,             &LDEHL,            &LDEA,
    // 6X
    &LDHB,             &LDHC,             &LDHD,             &LDHE,
    &LDHH,             &LDHL,             &LDHHL,            &LDHA,
    &LDLB,             &LDLC,             &LDLD,             &LDLE,
    &LDLH,             &LDLL,             &LDLHL,            &LDLA,
    // 7X
    &LDHLB,            &LDHLC,            &LDHLD,            &LDHLE,
    &LDHLH,            &LDHLL,            &STOP,             &LDHLA,
    &LDAB,             &LDAC,             &LDAD,             &LDAE,
    &LDAH,             &LDAL,             &LDAHL,            &LDAA,
    // 8X
    &ADDAB,            &ADDAC,            &ADDAD,            &ADDAE,
    &ADDAH,            &ADDAL,            &ADDAHL,           &ADDAA,
    &ADCAB,            &ADCAC,            &ADCAD,            &ADCAE,
    &ADCAH,            &ADCAL,            &ADCAHL,           &ADCAA,
    // 9X
    &SUBAB,            &SUBAC,            &SUBAD,            &SUBAE,
    &SUBAH,            &SUBAL,            &SUBAHL,           &SUBAA,
    &SBCAB,            &SBCAC,            &SBCAD,            &SBCAE,
    &SBCAH,            &SBCAL,            &SBCAHL,           &SBCAA,
    // AX
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &XORA,
    // BX
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    // CX
    &STOP,             &POPBC,            &STOP,             &STOP,
    &STOP,             &PUSHBC,           &ADDAn,            &STOP,
    &STOP,             &STOP,             &STOP,             &ext_op,
    &STOP,             &STOP,             &ADCAn,            &STOP,
    // DX
    &STOP,             &POPDE,            &STOP,             &STOP,
    &STOP,             &PUSHDE,           &SUBAn,            &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &SBCAn,            &STOP,
    // EX
    &LDHnA,            &POPHL,            &LDHCA,            &STOP,
    &STOP,             &PUSHHL,           &STOP,             &STOP,
    &STOP,             &STOP,             &LDnnA,            &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    // FX
    &LDHAn,            &POPAF,            &LDHAC,            &STOP,
    &STOP,             &PUSHAF,           &STOP,             &STOP,
    &LDHLSPn,          &LDSPHL,           &LDAnn,            &EI,
    &STOP,             &STOP,             &STOP,             &STOP
};

static void (*ext_ops[])() = {
    // CB0X
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    // CB1X
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    // CB2X
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    // CB3X
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    // CB4X
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    // CB5X
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    // CB6X
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    // CB7X
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &BIT7H,            &STOP,             &STOP,             &STOP,
    // CB8X
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    // CB9X
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    // CBAX
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    // CBBX
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    // CBCX
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    // CBDX
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    // CBEX
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    // CBFX
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP
};

void z80_execute()
{
    while (1)
    {
        Byte op = next_byte();

        printf("%02X", op);

        (*ops[op])();

        z80.clock.t += z80.regs.t;

        printf("AF=%04X ", AF);
        printf("BC=%04X ", BC);
        printf("DE=%04X ", DE);
        printf("HL=%04X ", HL);
        printf("PC=%04X ", PC);
        printf("SP=%04X ", SP);
        printf("T=%d\n", z80.clock.t);
    }
}

void ext_op()
{
    Byte op = next_byte();

    printf("%02X", op);

    (*ext_ops[op])();
}

