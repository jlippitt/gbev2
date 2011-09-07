#include <stdio.h>
#include "mbc.h"

#define ROM_ONLY      0x00
#define MBC1          0x01
#define MBC1_ERAM     0x02
#define MBC1_ERAM_BAT 0x03
#define MBC3          0x11
#define MBC3_ERAM     0x12
#define MBC3_ERAM_BAT 0x13

struct MBC mbc = {0, 0, 0, {0, 0, 0, 0}};

void mbc_init(Byte cart_type)
{
    printf("Cart Type: %02X\n", cart_type);

    mbc.cart_type = cart_type;

    mbc.rom_offset = 0x4000;
    mbc.ram_offset = 0x0000;

    mbc.regs.rom_bank = 0;
    mbc.regs.ram_bank = 0;
    mbc.regs.ram_on   = 0;
    mbc.regs.mode     = 0;
}

void mbc_putbyte(Word addr, Byte value)
{
    switch (addr & 0xF000)
    {
        case 0x0000:
        case 0x1000:
            switch (mbc.cart_type)
            {
                case MBC1_ERAM:
                case MBC1_ERAM_BAT:
                case MBC3_ERAM:
                case MBC3_ERAM_BAT:
                    //printf("RAM State: %02X\n", value); 
                    mbc.regs.ram_on = ((value & 0x0F) == 0x0A) ? 1 : 0;
                    break;
            }
            break;

        case 0x2000:
        case 0x3000:
            switch (mbc.cart_type)
            {
                case MBC1:
                case MBC1_ERAM:
                case MBC1_ERAM_BAT:
                    //printf("ROM Bank (Lower): %02X\n", value); 
                    // Set lower 5 bits of rom bank (skipping #0)
                    mbc.regs.rom_bank = (mbc.regs.rom_bank & 0x60) + (value & 0x1F);
                    break;

                case MBC3:
                case MBC3_ERAM:
                case MBC3_ERAM_BAT:
                    //printf("ROM Bank: %02X\n", value); 
                    // Set all 7 bits of rom bank (skipping #0)
                    mbc.regs.rom_bank = value & 0x7F;
                    break;
            }

            // Update ROM offset
            mbc.rom_offset = (mbc.regs.rom_bank ? mbc.regs.rom_bank : 1) * 0x4000;
            break;

        case 0x4000:
        case 0x5000:
            switch (mbc.cart_type)
            {
                case MBC1:
                case MBC1_ERAM:
                case MBC1_ERAM_BAT:
                    if (mbc.regs.mode)
                    {
                        //printf("RAM Bank: %02X\n", value); 
                        // RAM mode: Set bank
                        mbc.regs.ram_bank = value & 0x03;
                        mbc.ram_offset = mbc.regs.ram_bank * 0x2000;
                    }
                    else
                    {
                        //printf("ROM Bank (Upper): %02X\n", value); 
                        // ROM mode: Set upper 2 bits of bank
                        mbc.regs.rom_bank = (mbc.regs.rom_bank & 0x1F) +
                            ((value & 0x03) << 5);

                        mbc.rom_offset = mbc.regs.rom_bank * 0x4000;
                    }
                    break;

                case MBC3:
                case MBC3_ERAM:
                case MBC3_ERAM_BAT:
                    if (value <= 0x03)
                    {
                        //printf("RAM Bank: %02X\n", value); 
                        // RAM mode: Set bank
                        mbc.regs.ram_bank = value & 0x03;
                        mbc.ram_offset = mbc.regs.ram_bank * 0x2000;
                    }
                    else
                    {
                        // TODO: Set clock register
                    }
                    break;
            }
            break;

        case 0x6000:
        case 0x7000:
            switch (mbc.cart_type)
            {
                case MBC1_ERAM:
                case MBC1_ERAM_BAT:
                    //printf("MBC Mode: %02X\n", value); 
                    mbc.regs.mode = value & 0x01;
                    break;

                case MBC3:
                case MBC3_ERAM:
                case MBC3_ERAM_BAT:
                    // TODO: Latch clock data
                    break;
            }
            break;
    }
}

