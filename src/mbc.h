#ifndef MBC_H
#define MBC_H

#include <stdio.h>
#include "types.h"

extern struct MBC
{
    Byte cart_type;

    Byte *rom;
    uint32_t rom_offset;

    Byte *ram;
    uint32_t ram_size;
    uint32_t ram_offset;

    FILE* ram_file;

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

void mbc_load(const char *path);

void mbc_cleanup();

Byte mbc_getbyte(Word addr);

void mbc_putbyte(Word addr, Byte value);

#endif
