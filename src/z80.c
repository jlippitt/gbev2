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
    &INCB,             &DECB,             &LDBn,             &STOP,
    &LDnnSP,           &STOP,             &LDABC,            &STOP,
    &INCC,             &DECC,             &LDCn,             &STOP,
    // 1X
    &STOP,             &LDDEnn,           &LDDEA,            &STOP,
    &INCD,             &DECD,             &LDDn,             &STOP,
    &STOP,             &STOP,             &LDADE,            &STOP,
    &INCE,             &DECE,             &LDEn,             &STOP,
    // 2X
    &JRNZn,            &LDHLnn,           &LDIHLA,           &STOP,
    &INCH,             &DECH,             &LDHn,             &STOP,
    &STOP,             &STOP,             &LDIAHL,           &STOP,
    &INCL,             &DECL,             &LDLn,             &STOP,
    // 3X
    &STOP,             &LDSPnn,           &LDDHLA,           &STOP,
    &INCHL,            &DECHL,            &LDHLn,            &STOP,
    &STOP,             &STOP,             &LDDAHL,           &STOP,
    &INCA,             &DECA,             &LDAn,             &STOP,
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
    &ANDAB,            &ANDAC,            &ANDAD,            &ANDAE,
    &ANDAH,            &ANDAL,            &ANDAHL,           &ANDAA,
    &XORAB,            &XORAC,            &XORAD,            &XORAE,
    &XORAH,            &XORAL,            &XORAHL,           &XORAA,
    // BX
    &ORAB,             &ORAC,             &ORAD,             &ORAE,
    &ORAH,             &ORAL,             &ORAHL,            &ORAA,
    &CPAB,             &CPAC,             &CPAD,             &CPAE,
    &CPAH,             &CPAL,             &CPAHL,            &CPAA,
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
    &STOP,             &PUSHHL,           &ANDAn,            &STOP,
    &STOP,             &STOP,             &LDnnA,            &STOP,
    &STOP,             &STOP,             &XORAn,            &STOP,
    // FX
    &LDHAn,            &POPAF,            &LDHAC,            &STOP,
    &STOP,             &PUSHAF,           &ORAn,             &STOP,
    &LDHLSPn,          &LDSPHL,           &LDAnn,            &EI,
    &STOP,             &STOP,             &CPAn,             &STOP
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

