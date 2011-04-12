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
    &NOP,              &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &INCC,             &STOP,             &LDCn,             &STOP,
    // 1X
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    // 2X
    &JRNZn,            &LDHLnn,           &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    // 3X
    &STOP,             &LDSPnn,           &LDDHLA,           &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &LDAn,             &STOP,
    // 4X
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    // 5X
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    // 6X
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    // 7X
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    // 8X
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    // 9X
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
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
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &ext_op,
    &STOP,             &STOP,             &STOP,             &STOP,
    // DX
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    // EX
    &STOP,             &STOP,             &LDHCA,            &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    // FX
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &EI,
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

