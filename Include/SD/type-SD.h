#pragma once

#include<stdint.h>

typedef struct{
    volatile uint32_t arg;
    volatile uint32_t CMD;
}CMDR;

typedef struct{
    volatile uint32_t DMA_SD;
    volatile uint16_t BS_SD;
    volatile uint16_t BC_SD;
    volatile uint32_t ARG_SD;
    volatile uint32_t CMD_SD;
    volatile uint32_t RESP_SD[4];
    volatile uint32_t BDP_SD;
    volatile uint32_t PS_SD;
    volatile uint8_t HC_SD;
    volatile uint8_t PWC_SD;
    uint16_t RESERVE_4[1];
    volatile uint16_t CC_SD;
    uint8_t RESERVE_5[1];
    volatile uint8_t SR_SD;
    volatile uint16_t NS_SD;
    volatile uint16_t ERS_SD;
    volatile uint16_t NSE_SD;
    volatile uint16_t ERSE_SD;
    volatile uint16_t NSIE_SD;
    volatile uint16_t ACMDE_SD;
    volatile uint16_t HC2_SD;
    volatile uint64_t CB_SD;
}SDAR;

typedef struct{
    volatile uint32_t CTLR_SD;
    volatile uint32_t PWC_SD;
    volatile uint32_t CLKDIV_SD;
    volatile uint32_t CLKS_SD;
    volatile uint32_t CLKE_SD;
    volatile uint32_t TIME_SD;
    volatile uint32_t CTYPE_SD;
    volatile uint32_t BS_SD;
    volatile uint32_t BC_SD;
    volatile uint32_t IM_SD;
    volatile uint32_t ARG_SD;
    volatile uint32_t CMD_SD;
    volatile uint32_t RESP_SD[4];
    volatile uint32_t IMS_SD;
    uint32_t RESERVE_2[111];
    volatile uint32_t BDP_SD;
}SDRR;

typedef struct{
    CMDR CMD0;
    CMDR CMD8;
    CMDR CMD55;
    CMDR ACMD41;
    CMDR CMD2;
    CMDR CMD3;
    CMDR CMD7;
    CMDR CMD17;
    CMDR CMD18;
    CMDR CMD12;
    CMDR CMD23;
    CMDR CMD13;
    CMDR ACMD6;
}SDCMD;

typedef struct{
    uint8_t Buffer[131072];
    int Buffer_index;
    int Current_index;
}FileBuffer;

typedef struct{
    void (*IRQ_read)();
    void (*CMD_send)(CMDR _CMD);
    void (*single_read)(uint32_t _sector);
    void (*multi_read)(uint32_t _sector);
    void (*block_init)(int _block_size, int _block_count);
    void (*register_init)();
    void (*wait_command)();
}HALSDF;

typedef struct{
    uint64_t SDAddress;
    const HALSDF* SDF;
}HALSD;
