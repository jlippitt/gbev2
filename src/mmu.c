#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "gpu.h"
#include "joypad.h"
#include "mmu.h"
#include "timer.h"
#include "z80.h"

struct MMU mmu = {
    true,
    {0x31, 0xFE, 0xFF, 0xAF, 0x21, 0xFF, 0x9F, 0x32, 0xCB, 0x7C, 0x20, 0xFB, 0x21, 0x26, 0xFF, 0x0E,
     0x11, 0x3E, 0x80, 0x32, 0xE2, 0x0C, 0x3E, 0xF3, 0xE2, 0x32, 0x3E, 0x77, 0x77, 0x3E, 0xFC, 0xE0,
     0x47, 0x11, 0x04, 0x01, 0x21, 0x10, 0x80, 0x1A, 0xCD, 0x95, 0x00, 0xCD, 0x96, 0x00, 0x13, 0x7B,
     0xFE, 0x34, 0x20, 0xF3, 0x11, 0xD8, 0x00, 0x06, 0x08, 0x1A, 0x13, 0x22, 0x23, 0x05, 0x20, 0xF9,
     0x3E, 0x19, 0xEA, 0x10, 0x99, 0x21, 0x2F, 0x99, 0x0E, 0x0C, 0x3D, 0x28, 0x08, 0x32, 0x0D, 0x20,
     0xF9, 0x2E, 0x0F, 0x18, 0xF3, 0x67, 0x3E, 0x64, 0x57, 0xE0, 0x42, 0x3E, 0x91, 0xE0, 0x40, 0x04,
     0x1E, 0x02, 0x0E, 0x0C, 0xF0, 0x44, 0xFE, 0x90, 0x20, 0xFA, 0x0D, 0x20, 0xF7, 0x1D, 0x20, 0xF2,
     0x0E, 0x13, 0x24, 0x7C, 0x1E, 0x83, 0xFE, 0x62, 0x28, 0x06, 0x1E, 0xC1, 0xFE, 0x64, 0x20, 0x06,
     0x7B, 0xE2, 0x0C, 0x3E, 0x87, 0xF2, 0xF0, 0x42, 0x90, 0xE0, 0x42, 0x15, 0x20, 0xD2, 0x05, 0x20,
     0x4F, 0x16, 0x20, 0x18, 0xCB, 0x4F, 0x06, 0x04, 0xC5, 0xCB, 0x11, 0x17, 0xC1, 0xCB, 0x11, 0x17,
     0x05, 0x20, 0xF5, 0x22, 0x23, 0x22, 0x23, 0xC9, 0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B,
     0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
     0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC,
     0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E, 0x3c, 0x42, 0xB9, 0xA5, 0xB9, 0xA5, 0x42, 0x4C,
     0x21, 0x04, 0x01, 0x11, 0xA8, 0x00, 0x1A, 0x13, 0xBE, 0x20, 0xFE, 0x23, 0x7D, 0xFE, 0x34, 0x20,
     0xF5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20, 0xFB, 0x86, 0x20, 0xFE, 0x3E, 0x01, 0xE0, 0x50},
    NULL,
    0,
    0
};

void mmu_reset()
{
    mmu.in_bios = true;

    free(mmu.rom);
    mmu.rom = NULL;

    mmu.ienable = 0;
    mmu.iflag   = 0;

    for (Word i = 0; i < WRAM_SIZE; i++)
    {
        mmu.wram[i] = 0;
    }

    for (Word i = 0; i < ERAM_SIZE; i++)
    {
        mmu.eram[i] = 0;
    }

    for (Word i = 0; i < ZRAM_SIZE; i++)
    {
        mmu.zram[i] = 0;
    }
}

void mmu_load(const char *path)
{
    FILE *fp = fopen(path, "rb");

    if (fp)
    {
        fseek(fp, 0, SEEK_END);
        long len = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        mmu.rom = malloc(len + 1);

        fread(mmu.rom, 1, len, fp);

        fclose(fp);
    }
    else
    {
        fprintf(stderr, "Failed to open %s\n", path);
        exit(1);
    }
}

Byte mmu_getbyte(Word addr)
{
    switch (addr & 0xF000)
    {
        // BIOS (256b) / ROM0
        case 0x0000:
            if (mmu.in_bios)
            {
                if (addr < 0x0100)
                {
                    return mmu.bios[addr];
                }
                else if (z80.regs.pc == 0x0100)
                {
                    mmu.in_bios = false;
                }
            }

            assert(mmu.rom != NULL);
            return mmu.rom[addr];

        // ROM0
        case 0x1000:
        case 0x2000:
        case 0x3000:
            assert(mmu.rom != NULL);
            return mmu.rom[addr];

        // ROM1 (unbanked) (16k)
        case 0x4000:
        case 0x5000:
        case 0x6000:
        case 0x7000:
            assert(mmu.rom != NULL);
            return mmu.rom[addr];

        // Graphics: VRAM (8k)
        case 0x8000:
        case 0x9000:
            return gpu.vram[addr & 0x1FFF];

        // External RAM (8k)
        case 0xA000:
        case 0xB000:
            return mmu.eram[addr & 0x1FFF];

        // Working RAM (8k)
        case 0xC000:
        case 0xD000:
            return mmu.wram[addr & 0x1FFF];

        // Working RAM shadow
        case 0xE000:
            return mmu.wram[addr & 0x1FFF];

        // Everthing else
        case 0xF000:
            switch (addr & 0x0F00)
            {
                // Working RAM shadow
                case 0x000: case 0x100: case 0x200: case 0x300:
                case 0x400: case 0x500: case 0x600: case 0x700:
                case 0x800: case 0x900: case 0xA00: case 0xB00:
                case 0xC00: case 0xD00:
                    return mmu.wram[addr & 0x1FFF];

                // Graphics: object attribute memory
                // OAM is 160 bytes, remaining bytes read as 0
                case 0xE00:
                    if (addr < 0xFEA0)
                    {
                        return gpu.oam[addr & 0xFF];
                    }
                    else
                    {
                        return 0;
                    }

                // Zero-page RAM, I/O, Interrupts
                case 0xF00:
                    if (addr == 0xFFFF)
                    {
                        // Interrupt enable/disable
                        return mmu.ienable;
                    }
                    else if (addr >= 0xFF80)
                    {
                        // Zero-page RAM
                        return mmu.zram[addr & 0x7F];
                    }
                    else if (addr >= 0xFF40)
                    {
                        // GPU registers
                        return gpu_getbyte(addr);
                    }
                    else
                    {
                        switch (addr)
                        {
                            // Joypad register
                            case 0xFF00:
                                return joypad_getbyte();

                            case 0xFF04:
                            case 0xFF05:
                            case 0xFF06:
                            case 0xFF07:
                                return timer_getbyte(addr);

                            // Interrupt flag
                            case 0xFF0F:
                                return mmu.iflag;

                            default:
                                return 0;
                        }
                    }
            }

        default:
            assert(false);
            return 0;
    }

}

Word mmu_getword(Word addr)
{
    return mmu_getbyte(addr) + (mmu_getbyte(addr + 1) << 8);
}

void mmu_putbyte(Word addr, Byte value)
{
    switch (addr & 0xF000)
    {
        // ROM (read only)
        case 0x0000:
        case 0x1000:
        case 0x2000:
        case 0x3000:
        case 0x4000:
        case 0x5000:
        case 0x6000:
        case 0x7000:
            break;

        // Graphics: VRAM (8k)
        case 0x8000:
        case 0x9000:
            gpu.vram[addr & 0x1FFF] = value;
            break;

        // External RAM (8k)
        case 0xA000:
        case 0xB000:
            mmu.eram[addr & 0x1FFF] = value;
            break;

        // Working RAM (8k)
        case 0xC000:
        case 0xD000:
            mmu.wram[addr & 0x1FFF] = value;
            break;

        // Working RAM shadow
        case 0xE000:
            mmu.wram[addr & 0x1FFF] = value;
            break;

        // Everything else
        case 0xF000:
            switch (addr & 0x0F00)
            {
                // Working RAM shadow
                case 0x000: case 0x100: case 0x200: case 0x300:
                case 0x400: case 0x500: case 0x600: case 0x700:
                case 0x800: case 0x900: case 0xA00: case 0xB00:
                case 0xC00: case 0xD00:
                    mmu.wram[addr & 0x1FFF] = value;
                    break;

                // Graphics: object attribute memory
                // OAM is 160 bytes, remaining bytes read as 0
                case 0xE00:
                    if (addr < 0xFEA0)
                    {
                        gpu.oam[addr & 0xFF] = value;
                    }
                    break;

                // Zero-page RAM, Interrupts, I/O
                case 0xF00:
                    if (addr == 0xFFFF)
                    {
                        // Interrupt enable/disable
                        printf("IE: %02X\n", value);
                        mmu.ienable = value;
                    }
                    if (addr >= 0xFF80)
                    {
                        // Zero page RAM
                        mmu.zram[addr & 0x7F] = value;
                    }
                    else if (addr >= 0xFF40)
                    {
                        // GPU registers
                        gpu_putbyte(addr, value);
                    }
                    else
                    {
                        switch (addr)
                        {
                            case 0xFF00:
                                // Joypad register
                                joypad_putbyte(value);
                                break;

                            case 0xFF04:
                            case 0xFF05:
                            case 0xFF06:
                            case 0xFF07:
                                timer_putbyte(addr, value);
                                break;

                            case 0xFF0F:
                                // Interrupt flag
                                mmu.iflag = value;
                                break;
                        }
                    }
                    break;

                default:
                    assert(false);
            }
            break;

        default:
            assert(false);
    }
}

void mmu_putword(Word addr, Word value)
{
    mmu_putbyte(addr, value & 0xFF);
    mmu_putbyte(addr + 1, value >> 8);
}

