#ifndef TIMER_H
#define TIMER_H

#include "types.h"

extern struct Timer
{
    struct
    {
        uint32_t main;
        uint32_t sub;
        uint32_t div;
    }
    clock;

    struct
    {
        Byte div;
        Byte count;
        Byte mod;
        Byte ctrl;
    }
    regs;
}
timer;

void timer_reset();

Byte timer_getbyte(Word addr);

void timer_putbyte(Word addr, Byte value);

void timer_update(Byte ticks);

#endif
