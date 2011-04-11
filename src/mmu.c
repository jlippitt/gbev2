#include "mmu.h"

void mmu_reset()
{
    for (int i = 0; i < 65536; i++)
    {
        mmu.rom[i] = 0;
    }
}

Byte mmu_getbyte(Word addr)
{
    return mmu.rom[addr];
}

Word mmu_getword(Word addr)
{
    return mmu_getbyte(addr) + (mmu_getbyte(addr + 1) << 8);
}

void mmu_putbyte(Word addr, Byte value)
{
    mmu.rom[addr] = value;
}

void mmu_putword(Word addr, Word value)
{
    mmu.rom[addr] = value & 0xFF;
    mmu.rom[addr + 1] = value >> 8;
}

