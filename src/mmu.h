#ifndef MMU_H
#define MMU_H

#include "types.h"

struct MMU
{
    Byte rom[65536];
}
mmu;

void mmu_reset();

Byte mmu_getbyte(Word addr);

Word mmu_getword(Word addr);

void mmu_putbyte(Word addr, Byte value);

void mmu_putword(Word addr, Word value);

#endif
