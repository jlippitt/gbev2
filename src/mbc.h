#ifndef MBC_H
#define MBC_H

#include "types.h"

extern struct MBC
{
    Byte cart_type;

    Word rom_offset;
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

void mbc_set_rom_bank(Byte value);

void mbc_set_ram_bank(Byte value);

void mbc_set_ram_state(Byte value);

void mbc_set_mode(Byte value);

#endif
