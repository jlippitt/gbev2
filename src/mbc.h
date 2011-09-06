#ifndef MBC_H
#define MBC_H

#include "types.h"

extern struct MBC
{
    Byte cart_type;

    uint32_t rom_offset;
    Word ram_offset;

    struct
    {
        Byte rom_bank;
        Byte ram_bank;
        Byte ram_on;
        Byte mode;
    }
    regs;
}
mbc;

void mbc_init(Byte cart_type);

void mbc_putbyte(Word addr, Byte value);

#endif
