#include <stdio.h>
#include <stdlib.h>
#include "z80.h"
#include "z80_load.h"
#include "z80_macros.h"
#include "z80_misc.h"

struct Z80 z80 = {{0, 0, 0}, {0}};

static void (*ops[])() = {
    // 0X
    &NOP,              &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    // 1X
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    // 2X
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    // 3X
    &STOP,             &LDSPnn,           &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
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
    &STOP,             &STOP,             &STOP,             &STOP,
    // BX
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    // CX
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    // DX
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    // EX
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    &STOP,             &STOP,             &STOP,             &STOP,
    // FX
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

        printf("%02X: ", op);

        (*ops[op])();

        z80.clock.t += z80.regs.t;

        printf("PC=%04X ", PC);
        printf("SP=%04X ", SP);
        printf("T=%d\n", z80.clock.t);
    }
}

