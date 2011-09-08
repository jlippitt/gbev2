#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "mbc.h"

#define ROM_ONLY      0x00
#define MBC1          0x01
#define MBC1_ERAM     0x02
#define MBC1_ERAM_BAT 0x03
#define MBC3          0x11
#define MBC3_ERAM     0x12
#define MBC3_ERAM_BAT 0x13

struct MBC mbc = {0, NULL, 0x4000, NULL, 0, 0x0000, NULL, {0, 0, 0, 0}};

static inline bool is_battery()
{
    switch (mbc.cart_type)
    {
        case MBC1_ERAM_BAT:
        case MBC3_ERAM_BAT:
            return true;

        default:
            return false;
    }
}

static void load_ram(const char *rom_path)
{
    // Work out ram file path
    const char *separator = strrchr(rom_path, '.');

    char *ram_path;

    if (separator)
    {
        size_t len = separator - rom_path;
        ram_path = malloc(len + 1);
        strncpy(ram_path, rom_path, len);
        ram_path[len] = '\0';
    }
    else
    {
        ram_path = malloc(strlen(rom_path) + 4);
        strcpy(ram_path, rom_path);
    }

    strcat(ram_path, ".ram");

    mbc.ram_file = fopen(ram_path, "r+b");

    if (mbc.ram_file)
    {
        // Read existing RAM into memory
        fseek(mbc.ram_file, 0, SEEK_END);
        long len = ftell(mbc.ram_file);
        fseek(mbc.ram_file, 0, SEEK_SET);
        fread(mbc.ram, 1, len, mbc.ram_file);
    }
    else
    {
        mbc.ram_file = fopen(ram_path, "w+b");

        // Fill up RAM file with blank data
        for (uint32_t i = 0; i < mbc.ram_size; i++)
        {
            fputc('\0', mbc.ram_file);
        }
    }
}

void mbc_load(const char *path)
{
    FILE *fp = fopen(path, "rb");

    if (fp)
    {
        // Read entire ROM into memory
        fseek(fp, 0, SEEK_END);
        long len = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        mbc.rom = malloc(len);
        fread(mbc.rom, 1, len, fp);
        fclose(fp);

        mbc.rom_offset = 0x4000;
        mbc.cart_type = mbc.rom[0x147];

        printf("Cart Type: %02X\n", mbc.cart_type);

        // Initialise RAM
        switch (mbc.rom[0x149])
        {
            case 0:
                mbc.ram_size = 0;
                break;

            case 1:
                mbc.ram_size = 2048;
                break;

            case 2:
                mbc.ram_size = 8192;
                break;

            case 3:
                mbc.ram_size = 32768;
                break;

            case 4:
                mbc.ram_size = 131072;
                break;
        }

        printf("RAM Size: %u\n", mbc.ram_size);

        if (mbc.ram_size > 0)
        {
            mbc.ram = malloc(mbc.ram_size);
        }
        else
        {
            mbc.ram = NULL;
        }

        mbc.ram = malloc(32768);
        mbc.ram_offset = 0x0000;

        if (is_battery())
        {
            load_ram(path);
        }

        // Initialise bank switch settings
        mbc.regs.rom_bank = 0;
        mbc.regs.ram_bank = 0;
        mbc.regs.ram_on   = 0;
        mbc.regs.mode     = 0;
    }
    else
    {
        fprintf(stderr, "Failed to open %s\n", path);
        exit(1);
    }

}

void mbc_cleanup()
{
    if (is_battery())
    {
        fclose(mbc.ram_file);
    }

    free(mbc.ram);
    free(mbc.rom);
}

Byte mbc_getbyte(Word addr)
{
    switch (addr & 0xF000)
    {
        // ROM0
        case 0x0000:
        case 0x1000:
        case 0x2000:
        case 0x3000:
            return mbc.rom[addr & 0x3FFF];

        // ROM (switchable bank)
        case 0x4000:
        case 0x5000:
        case 0x6000:
        case 0x7000:
            return mbc.rom[mbc.rom_offset + (addr & 0x3FFF)];

        // External RAM
        case 0xA000:
        case 0xB000:
            assert((mbc.ram_offset + (addr & 0x1FFF)) < mbc.ram_size);
            return mbc.ram[mbc.ram_offset + (addr & 0x1FFF)];

        default:
            return 0;
    }
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
        
        // External RAM
        case 0xA000:
        case 0xB000:
            {
                uint32_t offset = mbc.ram_offset + (addr & 0x1FFF);

                assert(offset < mbc.ram_size);

                mbc.ram[offset] = value;

                if (is_battery())
                {
                    fseek(mbc.ram_file, offset, SEEK_SET);
                    fputc(value, mbc.ram_file);
                }
            }
            break;
    }
}

