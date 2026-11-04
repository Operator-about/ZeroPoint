#pragma once

#include<stdint.h>

typedef struct{
    volatile uint32_t GICC_CTLR;
    volatile uint32_t GICC_PMR;
    uint32_t RESERVE_1[1];
    volatile uint32_t GICC_IAR;
    volatile uint32_t GICC_EOIR;
    uint32_t RESERVE_2[1019];
    volatile uint32_t GICC_DIR;
}GICCv2;

typedef struct{
    volatile uint64_t SD;
    volatile uint64_t GICv2;
    volatile uint64_t UART;
    uint32_t UART_Standart;
    uint32_t SD_Standart;
    int UART_ID;
    int SD_ID;
}JumpData;
