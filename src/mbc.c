#include <stdio.h>
#include "mbc.h"

struct MBC mbc = {0, 0, 0, {0, 0, 0, 0}};

void mbc_init(Byte cart_type)
{
    printf("Cart Type: %d\n", cart_type);

    mbc.cart_type = cart_type;

    mbc.rom_offset = 0x4000;
    mbc.ram_offset = 0x0000;

    mbc.regs.rom_bank = 0;
    mbc.regs.ram_bank = 0;
    mbc.regs.ram_on   = 0;
    mbc.regs.mode     = 0;
}

void mbc_set_rom_bank(Byte value)
{
    switch (mbc.cart_type)
    {
        case 1:
        case 2:
        case 3:
            printf("ROM Bank (Lower): %02X\n", value); 
            // Set lower 5 bits of rom bank (skipping #0)
            value &= 0x1F;
            value = value ? value : 1;
            mbc.regs.rom_bank = (mbc.regs.rom_bank & 0x60) + value;

            // Update ROM offset
            mbc.rom_offset = mbc.regs.rom_bank * 0x4000;
            break;
    }
}

void mbc_set_ram_bank(Byte value)
{
    switch (mbc.cart_type)
    {
        case 1:
        case 2:
        case 3:
            if (mbc.regs.mode)
            {
                printf("RAM Bank: %02X\n", value); 
                // RAM mode: Set bank
                mbc.regs.ram_bank = value & 0x03;
                mbc.ram_offset = mbc.regs.ram_bank * 0x2000;
            }
            else
            {
                printf("ROM Bank (Upper): %02X\n", value); 
                // ROM mode: Set upper 2 bits of bank
                mbc.regs.rom_bank = (mbc.regs.rom_bank & 0x1F) +
                    ((value & 0x03) << 5);

                mbc.rom_offset = mbc.regs.rom_bank * 0x4000;
            }
            break;
    }
}

void mbc_set_ram_state(Byte value)
{
    switch (mbc.cart_type)
    {
        case 2:
        case 3:
            printf("RAM State: %02X\n", value); 
            mbc.regs.ram_on = ((value & 0x0F) == 0x0A) ? 1 : 0;
            break;
    }
}

void mbc_set_mode(Byte value)
{
    switch (mbc.cart_type)
    {
        case 2:
        case 3:
            printf("MBC Mode: %02X\n", value); 
            mbc.regs.mode = value & 0x01;
            break;
    }
}

