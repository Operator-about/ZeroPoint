#pragma once

#include<stdint.h>

typedef struct{
    uint8_t RESERVE_1[8];
    uint8_t StartLBA[4];
    uint8_t RESERVE_2[4];
}__attribute__((packed)) MBRP;

typedef struct{
    uint8_t RESERVE_1[446];
    MBRP PartionsRecords[4];
    uint8_t RESERVE_2[3];
}__attribute__((packed)) MBR;
