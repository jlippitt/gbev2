#include "mmu.h"
#include "z80.h"

int main(int argc, char *argv[])
{
    mmu_reset();
    z80_execute();

    return 0;
}

