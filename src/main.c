#include "gpu.h"
#include "mmu.h"
#include "z80.h"

int main(int argc, char *argv[])
{
    gpu_reset();
    mmu_reset();
    mmu_load(argv[1]);
    z80_execute();

    return 0;
}

