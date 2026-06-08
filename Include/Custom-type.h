#pragma once
#define SIZE 4096

#include<stdint.h>
#include<string.h>
#include<stdbool.h>
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
    volatile uint32_t* UART_DR;
    volatile uint32_t* UART_FR;
    volatile uint32_t* UART_MIS;
    volatile uint32_t* UART_IMSC;
    volatile uint32_t* UART_ICR;
    volatile uint32_t* UART_RIS;
}UART0;

typedef struct{
    uint8_t buffer[SIZE]; //Буфер
    volatile int head; //Размер буфера
    volatile int tail; //Текущая позиция
    volatile int end; //Означет, что передача закончена
}Ring_buffer;

typedef struct{
    volatile uint64_t MAIR;
    volatile uint64_t TTBR0;
    volatile uint64_t TCR;
    volatile uint64_t SCTLR;
}MMU_registers;

typedef struct{
    volatile uint16_t BPB_BytsPerSector;
    volatile uint8_t BPB_SectorsPerCluster;
    volatile uint16_t BPB_ReserverSectorCount;
    volatile uint8_t BPB_FATsTableCount;
    volatile uint32_t BPB_FATsTableSize32;
}FAT32_BPB;

typedef struct{
    volatile uint8_t file_name[11];
    volatile uint8_t file_attr[1];
    uint8_t RESERVE_1[8];
    volatile uint16_t file_FirstClusterHighBit;
    uint8_t RESERVE_2[6];
    volatile uint16_t file_FirstClusterLowBit;
}FAT32_FILE;

typedef struct{
    volatile uint32_t arg;
    volatile uint16_t CMD;
}CMD;

typedef struct{

}SDR;

typedef struct{
    volatile uint32_t* UART;
    volatile uint32_t* UART_FR;
    volatile uint32_t* UART_ICR;
    volatile uint32_t* UART_IMSC;
    volatile uint32_t* UART_MIS;
    volatile uint32_t* GICCv2;
}Periphery;