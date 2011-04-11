#include <stdio.h>
#include <stdlib.h>
#include "z80.h"
#include "z80_macros.h"
#include "z80_misc.h"

struct Z80 z80 = {{0}};

static void (*ops[])() = {
    // 0X
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    // 1X
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    // 2X
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    // 3X
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    // 4X
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    // 5X
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    // 6X
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    // 7X
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    // 8X
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    // 9X
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    // AX
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    // BX
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    // CX
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    // DX
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    // EX
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    // FX
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP,
    &NOP,              &NOP,              &NOP,              &NOP
};

void z80_execute()
{
    while (1)
    {
        Byte op = next_byte();

        printf("%02x: ", op);

        (*ops[op])();

        printf("PC: %04x\n", PC);
    }
}

