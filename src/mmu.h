#ifndef MMU_H
#define MMU_H

#include <stdbool.h>
#include "types.h"

#define WRAM_SIZE 8192
#define ERAM_SIZE 32768
#define ZRAM_SIZE 127

extern struct MMU
{
    bool in_bios;
    Byte bios[256];
    Byte *rom;
    Byte ienable;
    Byte iflag;
    Byte wram[WRAM_SIZE];
    Byte eram[ERAM_SIZE];
    Byte zram[ZRAM_SIZE];
}
mmu;

void mmu_reset();

void mmu_load(const char *path);

Byte mmu_getbyte(Word addr);

Word mmu_getword(Word addr);

void mmu_putbyte(Word addr, Byte value);

void mmu_putword(Word addr, Word value);

#endif
