#pragma once

#include<stdint.h>

typedef struct{
    volatile uint64_t MAIR;
    volatile uint64_t TTBR0;
    volatile uint64_t TCR;
    volatile uint64_t SCTLR;
}MMU_registers;
