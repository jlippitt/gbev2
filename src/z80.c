#include <stdio.h>
#include <stdlib.h>
#include "z80.h"
#include "z80_bit.h"
#include "z80_jump.h"
#include "z80_load.h"
#include "z80_macros.h"
#include "z80_math.h"
#include "z80_misc.h"
#include "z80_shift.h"

struct Z80 z80 = {{{0}, {0}, {0}, {0}, 0, 0, 0}, {0}};

static void ext_op();

static void (*ops[])() = {
    // 0X
    &NOP,              &LDBCnn,           &LDBCA,            &INCBC,
    &INCB,             &DECB,             &LDBn,             &RLCA,
    &LDnnSP,           &ADDHLBC,          &LDABC,            &DECBC,
    &INCC,             &DECC,             &LDCn,             &RRCA,
    // 1X
    &STOP,             &LDDEnn,           &LDDEA,            &INCDE,
    &INCD,             &DECD,             &LDDn,             &RLA,
    &JRn,              &ADDHLDE,          &LDADE,            &DECDE,
    &INCE,             &DECE,             &LDEn,             &RRA,
    // 2X
    &JRNZn,            &LDHLnn,           &LDIHLA,           &INCHL,
    &INCH,             &DECH,             &LDHn,             &STOP,
    &JRZn,             &ADDHLHL,          &LDIAHL,           &DECHL,
    &INCL,             &DECL,             &LDLn,             &STOP,
    // 3X
    &JRNCn,            &LDSPnn,           &LDDHLA,           &INCSP,
    &INCHLm,           &DECHLm,           &LDHLn,            &STOP,
    &JRCn,             &ADDHLSP,          &LDDAHL,           &DECSP,
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
    &RETNZ,            &POPBC,            &JPNZnn,           &JPnn,
    &CALLNZnn,         &PUSHBC,           &ADDAn,            &RST00,
    &RETZ,             &RET,              &JPZnn,            &ext_op,
    &CALLZnn,          &CALLnn,           &ADCAn,            &RST08,
    // DX
    &RETNC,            &POPDE,            &JPNCnn,           &STOP,
    &CALLNCnn,         &PUSHDE,           &SUBAn,            &RST10,
    &RETC,             &RETI,             &JPCnn,            &STOP,
    &CALLCnn,          &STOP,             &SBCAn,            &RST18,
    // EX
    &LDHnA,            &POPHL,            &LDHCA,            &STOP,
    &STOP,             &PUSHHL,           &ANDAn,            &RST20,
    &ADDSPn,           &JPHL,             &LDnnA,            &STOP,
    &STOP,             &STOP,             &XORAn,            &RST28,
    // FX
    &LDHAn,            &POPAF,            &LDHAC,            &STOP,
    &STOP,             &PUSHAF,           &ORAn,             &RST30,
    &LDHLSPn,          &LDSPHL,           &LDAnn,            &EI,
    &STOP,             &STOP,             &CPAn,             &RST38
};

static void (*ext_ops[])() = {
    // CB0X
    &RLCrB,            &RLCrC,            &RLCrD,            &RLCrE,
    &RLCrH,            &RLCrL,            &RLCHL,            &RLCrA,
    &RRCrB,            &RRCrC,            &RRCrD,            &RRCrE,
    &RRCrH,            &RRCrL,            &RRCHL,            &RRCrA,
    // CB1X
    &RLrB,             &RLrC,             &RLrD,             &RLrE,
    &RLrH,             &RLrL,             &RLHL,             &RLrA,
    &RRrB,             &RRrC,             &RRrD,             &RRrE,
    &RRrH,             &RRrL,             &RRHL,             &RRrA,
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

