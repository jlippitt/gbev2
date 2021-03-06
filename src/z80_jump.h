#ifndef Z80_JUMP_H
#define Z80_JUMP_H

#include "z80_macros.h"

// Jump to nn

void JPnn()
{
    Word tmp = next_word();
    debug("JP $%04X", tmp);
    PC = tmp;
    tick(3);
}

// Jump to nn if zero not set

void JPNZnn()
{
    Word tmp = next_word();
    debug("JP NZ,$%04X", tmp);

    if (!isset_flag(ZERO))
    {
        PC = tmp;
    }

    tick(3);
}

// Jump to nn if zero set

void JPZnn()
{
    Word tmp = next_word();
    debug("JP Z,$%04X", tmp);

    if (isset_flag(ZERO))
    {
        PC = tmp;
    }

    tick(3);
}

// Jump to nn if carry not set

void JPNCnn()
{
    Word tmp = next_word();
    debug("JP NC,$%04X", tmp);

    if (!isset_flag(CARRY))
    {
        PC = tmp;
    }

    tick(3);
}

// Jump to nn if carry set

void JPCnn()
{
    Word tmp = next_word();
    debug("JP C,$%04X", tmp);

    if (isset_flag(CARRY))
    {
        PC = tmp;
    }

    tick(3);
}

// Jump to HL

void JPHL()
{
    debug("JP (HL)");
    PC = HL;
    tick(1);
}

// Jump to offset n

void JRn()
{
    Byte tmp = next_byte();
    debug("JR %d", (int8_t)tmp);
    PC += (int8_t)tmp;
    tick(2);
}

// Jump to offset n if zero not set

void JRNZn()
{
    Byte tmp = next_byte();
    debug("JR NZ,%d", (int8_t)tmp);

    if (!isset_flag(ZERO))
    {
        PC += (int8_t)tmp;
    }

    tick(2);
}

// Jump to offset n if zero set

void JRZn()
{
    Byte tmp = next_byte();
    debug("JR Z,%d", (int8_t)tmp);

    if (isset_flag(ZERO))
    {
        PC += (int8_t)tmp;
    }

    tick(2);
}

// Jump to offset n if carry not set

void JRNCn()
{
    Byte tmp = next_byte();
    debug("JR NC,%d", (int8_t)tmp);

    if (!isset_flag(CARRY))
    {
        PC += (int8_t)tmp;
    }

    tick(2);
}

// Jump to offset n if carry set

void JRCn()
{
    Byte tmp = next_byte();
    debug("JR C,%d", (int8_t)tmp);

    if (isset_flag(CARRY))
    {
        PC += (int8_t)tmp;
    }

    tick(2);
}

// Call nn

void CALLnn()
{
    Word tmp = next_word();
    debug("CALL $%04X", tmp);
    push(PC);
    PC = tmp;
    tick(3);
}

// Call nn if zero not set

void CALLNZnn()
{
    Word tmp = next_word();
    debug("CALL NZ,$%04X", tmp);

    if (!isset_flag(ZERO))
    {
        push(PC);
        PC = tmp;
    }

    tick(3);
}

// Call nn if zero set

void CALLZnn()
{
    Word tmp = next_word();
    debug("CALL Z,$%04X", tmp);

    if (isset_flag(ZERO))
    {
        push(PC);
        PC = tmp;
    }

    tick(3);
}

// Call nn if carry not set

void CALLNCnn()
{
    Word tmp = next_word();
    debug("CALL NC,$%04X", tmp);

    if (!isset_flag(CARRY))
    {
        push(PC);
        PC = tmp;
    }

    tick(3);
}

// Call nn if carry set

void CALLCnn()
{
    Word tmp = next_word();
    debug("CALL C,$%04X", tmp);

    if (isset_flag(CARRY))
    {
        push(PC);
        PC = tmp;
    }

    tick(3);
}

// Restart at offset n

#define DEF_RSTn(n) \
void RST##n() \
{ \
    debug("RST $" #n); \
    push(PC); \
    PC = 0x##n; \
    tick(8); \
}

DEF_RSTn(00);
DEF_RSTn(08);
DEF_RSTn(10);
DEF_RSTn(18);
DEF_RSTn(20);
DEF_RSTn(28);
DEF_RSTn(30);
DEF_RSTn(38);
DEF_RSTn(40);
DEF_RSTn(48);
DEF_RSTn(50);
DEF_RSTn(58);
DEF_RSTn(60);

// Return

void RET()
{
    debug("RET");
    PC = pop();
    tick(2);
}

// Return if zero not set

void RETNZ()
{
    debug("RET NZ");

    if (!isset_flag(ZERO))
    {
        PC = pop();
    }

    tick(2);
}

// Return if zero set

void RETZ()
{
    debug("RET Z");

    if (isset_flag(ZERO))
    {
        PC = pop();
    }

    tick(2);
}

// Return if carry not set

void RETNC()
{
    debug("RET NC");

    if (!isset_flag(CARRY))
    {
        PC = pop();
    }

    tick(2);
}

// Return if carry set

void RETC()
{
    debug("RET C");

    if (isset_flag(CARRY))
    {
        PC = pop();
    }

    tick(2);
}

// Return and enable interrupts

void RETI()
{
    debug("RETI");
    PC = pop();
    z80.regs.ime = 1;
    tick(2);
}

#endif
