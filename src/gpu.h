#ifndef GPU_H
#define GPU_H

#include "types.h"

#define VRAM_SIZE 8192
#define OAM_SIZE  160

extern struct GPU
{
    Byte vram[VRAM_SIZE];
    Byte oam[OAM_SIZE];
}
gpu;

void gpu_reset();

#endif
