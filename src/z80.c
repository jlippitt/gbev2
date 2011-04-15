#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "gpu.h"
#include "z80.h"
#include "z80_bit.h"
#include "z80_jump.h"
#include "z80_load.h"
#include "z80_macros.h"
#include "z80_math.h"
#include "z80_misc.h"
#include "z80_shift.h"

struct Z80 z80 = {{{0}, {0}, {0}, {0}, 0, 0, 0, 0}, {0}, 0};

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
    &INCH,             &DECH,             &LDHn,             &DAA,
    &JRZn,             &ADDHLHL,          &LDIAHL,           &DECHL,
    &INCL,             &DECL,             &LDLn,             &CPL,
    // 3X
    &JRNCn,            &LDSPnn,           &LDDHLA,           &INCSP,
    &INCHLm,           &DECHLm,           &LDHLn,            &SCF,
    &JRCn,             &ADDHLSP,          &LDDAHL,           &DECSP,
    &INCA,             &DECA,             &LDAn,             &CCF,
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
    &LDHLH,            &LDHLL,            &HALT,             &LDHLA,
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
    &RETNC,            &POPDE,            &JPNCnn,           &XX,
    &CALLNCnn,         &PUSHDE,           &SUBAn,            &RST10,
    &RETC,             &RETI,             &JPCnn,            &XX,
    &CALLCnn,          &XX,               &SBCAn,            &RST18,
    // EX
    &LDHnA,            &POPHL,            &LDHCA,            &XX,
    &XX,               &PUSHHL,           &ANDAn,            &RST20,
    &ADDSPn,           &JPHL,             &LDnnA,            &XX,
    &XX,               &XX,               &XORAn,            &RST28,
    // FX
    &LDHAn,            &POPAF,            &LDHAC,            &DI,
    &XX,               &PUSHAF,           &ORAn,             &RST30,
    &LDHLSPn,          &LDSPHL,           &LDAnn,            &EI,
    &XX,               &XX,               &CPAn,             &RST38
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
    &SLAB,             &SLAC,             &SLAD,             &SLAE,
    &SLAH,             &SLAL,             &SLAHL,            &SLAA,
    &SRAB,             &SRAC,             &SRAD,             &SRAE,
    &SRAH,             &SRAL,             &SRAHL,            &SRAA,
    // CB3X
    &SWAPB,            &SWAPC,            &SWAPD,            &SWAPE,
    &SWAPH,            &SWAPL,            &SWAPHL,           &SWAPA,
    &SRLB,             &SRLC,             &SRLD,             &SRLE,
    &SRLH,             &SRLL,             &SRLHL,            &SRLA,
    // CB4X
    &BIT0B,            &BIT0C,            &BIT0D,            &BIT0E,
    &BIT0H,            &BIT0L,            &BIT0HL,           &BIT0A,
    &BIT1B,            &BIT1C,            &BIT1D,            &BIT1E,
    &BIT1H,            &BIT1L,            &BIT1HL,           &BIT1A,
    // CB5X
    &BIT2B,            &BIT2C,            &BIT2D,            &BIT2E,
    &BIT2H,            &BIT2L,            &BIT2HL,           &BIT2A,
    &BIT3B,            &BIT3C,            &BIT3D,            &BIT3E,
    &BIT3H,            &BIT3L,            &BIT3HL,           &BIT3A,
    // CB6X
    &BIT4B,            &BIT4C,            &BIT4D,            &BIT4E,
    &BIT4H,            &BIT4L,            &BIT4HL,           &BIT4A,
    &BIT5B,            &BIT5C,            &BIT5D,            &BIT5E,
    &BIT5H,            &BIT5L,            &BIT5HL,           &BIT5A,
    // CB7X
    &BIT6B,            &BIT6C,            &BIT6D,            &BIT6E,
    &BIT6H,            &BIT6L,            &BIT6HL,           &BIT6A,
    &BIT7B,            &BIT7C,            &BIT7D,            &BIT7E,
    &BIT7H,            &BIT7L,            &BIT7HL,           &BIT7A,
    // CB8X
    &RES0B,            &RES0C,            &RES0D,            &RES0E,
    &RES0H,            &RES0L,            &RES0HL,           &RES0A,
    &RES1B,            &RES1C,            &RES1D,            &RES1E,
    &RES1H,            &RES1L,            &RES1HL,           &RES1A,
    // CB9X
    &RES2B,            &RES2C,            &RES2D,            &RES2E,
    &RES2H,            &RES2L,            &RES2HL,           &RES2A,
    &RES3B,            &RES3C,            &RES3D,            &RES3E,
    &RES3H,            &RES3L,            &RES3HL,           &RES3A,
    // CBAX
    &RES4B,            &RES4C,            &RES4D,            &RES4E,
    &RES4H,            &RES4L,            &RES4HL,           &RES4A,
    &RES5B,            &RES5C,            &RES5D,            &RES5E,
    &RES5H,            &RES5L,            &RES5HL,           &RES5A,
    // CBBX
    &RES6B,            &RES6C,            &RES6D,            &RES6E,
    &RES6H,            &RES6L,            &RES6HL,           &RES6A,
    &RES7B,            &RES7C,            &RES7D,            &RES7E,
    &RES7H,            &RES7L,            &RES7HL,           &RES7A,
    // CBCX
    &SET0B,            &SET0C,            &SET0D,            &SET0E,
    &SET0H,            &SET0L,            &SET0HL,           &SET0A,
    &SET1B,            &SET1C,            &SET1D,            &SET1E,
    &SET1H,            &SET1L,            &SET1HL,           &SET1A,
    // CBDX
    &SET2B,            &SET2C,            &SET2D,            &SET2E,
    &SET2H,            &SET2L,            &SET2HL,           &SET2A,
    &SET3B,            &SET3C,            &SET3D,            &SET3E,
    &SET3H,            &SET3L,            &SET3HL,           &SET3A,
    // CBEX
    &SET4B,            &SET4C,            &SET4D,            &SET4E,
    &SET4H,            &SET4L,            &SET4HL,           &SET4A,
    &SET5B,            &SET5C,            &SET5D,            &SET5E,
    &SET5H,            &SET5L,            &SET5HL,           &SET5A,
    // CBFX
    &SET6B,            &SET6C,            &SET6D,            &SET6E,
    &SET6H,            &SET6L,            &SET6HL,           &SET6A,
    &SET7B,            &SET7C,            &SET7D,            &SET7E,
    &SET7H,            &SET7L,            &SET7HL,           &SET7A
};

void z80_reset()
{
    z80.regs.af.word = 0;
    z80.regs.bc.word = 0;
    z80.regs.de.word = 0;
    z80.regs.hl.word = 0;
    z80.regs.pc = 0;
    z80.regs.sp = 0;
    z80.regs.t  = 0;

    z80.clock.t = 0;

    z80.halt = 0;
}

void z80_doframe()
{
    uint32_t frame_time = z80.clock.t + 70224;

    do
    {
        if (z80.halt)
        {
            z80.regs.t = 4;
        }
        else
        {
            Byte op = next_byte();

            //printf("%02X", op);

            (*ops[op])();
        }

        if (z80.regs.ime && mmu.ienable)
        {
            z80.halt = 0;

            Byte ifired = mmu.ienable & mmu.iflag;

            if (ifired & INT_VBLANK)
            {
                printf("INT_VBLANK\n");
                mmu.iflag &= INT_VBLANK;
                z80.regs.ime = 0;
                RST40();
            }
            else if (ifired & INT_LCD_STAT)
            {
                printf("INT_LCD_STAT\n");
                mmu.iflag &= INT_LCD_STAT;
                z80.regs.ime = 0;
                RST48();
            }
            else if (ifired & INT_TIMER)
            {
                printf("INT_TIMER\n");
                mmu.iflag &= INT_TIMER;
                z80.regs.ime = 0;
                RST50();
            }
            else if (ifired & INT_SERIAL)
            {
                printf("INT_SERIAL\n");
                mmu.iflag &= INT_SERIAL;
                z80.regs.ime = 0;
                RST58();
            }
            else if (ifired & INT_JOYPAD)
            {
                printf("INT_JOYPAD\n");
                mmu.iflag &= INT_JOYPAD;
                z80.regs.ime = 0;
                RST60();
            }
        }

        z80.clock.t += z80.regs.t;

        gpu_step(z80.regs.t);

        //printf("AF=%04X ", AF);
        //printf("BC=%04X ", BC);
        //printf("DE=%04X ", DE);
        //printf("HL=%04X ", HL);
        //printf("PC=%04X ", PC);
        //printf("SP=%04X ", SP);
        //printf("T=%d\n", z80.clock.t);
    }
    while (z80.clock.t < frame_time);
}

void ext_op()
{
    Byte op = next_byte();

    //printf("%02X", op);

    (*ext_ops[op])();
}

