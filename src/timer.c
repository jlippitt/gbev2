#include "mmu.h"
#include "timer.h"

#define TIMER_RUNNING   0x04
#define TIMER_SPEED     0x03

struct Timer timer = {{0, 0, 0}, {0, 0, 0, 0}};

void timer_reset()
{
    timer.regs.div   = 0;
    timer.regs.count = 0;
    timer.regs.mod   = 0;
    timer.regs.ctrl  = 0;
}

Byte timer_getbyte(Word addr)
{
    switch (addr)
    {
        case 0xFF04:
            return timer.regs.div;

        case 0xFF05:
            return timer.regs.count;

        case 0xFF06:
            return timer.regs.mod;

        case 0xFF07:
            return timer.regs.ctrl;

        default:
            return 0;
    }
}

void timer_putbyte(Word addr, Byte value)
{
    switch (addr)
    {
        case 0xFF04:
            timer.regs.div = value;
            break;

        case 0xFF05:
            timer.regs.count = value;
            break;

        case 0xFF06:
            timer.regs.mod = value;
            break;

        case 0xFF07:
            timer.regs.ctrl = value;
            break;
    }
}

void timer_update(Byte ticks)
{
    // Increment internal clocks and 1/16 'div' register
    timer.clock.sub += ticks;

    while (timer.clock.sub >= 4)
    {
        timer.clock.main++;
        timer.clock.div++;
        timer.clock.sub -= 4;

        if (timer.clock.div == 16)
        {
            timer.regs.div++;
            timer.clock.div = 0;
        }
    }

    // Check control register
    if (timer.regs.ctrl & TIMER_RUNNING)
    {
        Byte threshold;

        switch (timer.regs.ctrl & TIMER_SPEED)
        {
            case 1:
                threshold = 1;
                break;

            case 2:
                threshold = 4;
                break;

            case 3:
                threshold = 16;
                break;

            default:
                threshold = 64;
                break;
        }

        if (timer.clock.main >= threshold)
        {
            // Step timer
            timer.clock.main = 0;
            timer.regs.count++;

            if (timer.regs.count == 0)
            {
                // On overflow, replace with modulo
                timer.regs.count = timer.regs.mod;

                // Raise interrupt
                mmu.iflag |= INT_TIMER;
            }
        }
    }
}

