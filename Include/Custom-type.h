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
}__attribute__((packed)) GICCv2;

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
}__attribute__((packed)) MMU_registers;

typedef struct{
    uint8_t RESERVE_1[11];
    volatile uint16_t BPB_BytsPerSector;
    volatile uint8_t BPB_SectorsPerCluster;
    volatile uint16_t BPB_ReserverSectorCount;
    volatile uint8_t BPB_FATsTableCount;
    uint8_t RESERVE_2[15];
    volatile uint32_t BPB_FATsTableSize32;
}__attribute__((packed)) FAT32_BPB;

typedef struct{
    volatile uint8_t file_name[11];
    volatile uint8_t file_attr[1];
    uint8_t RESERVE_1[8];
    volatile uint16_t file_FirstClusterHighBit;
    uint8_t RESERVE_2[6];
    volatile uint16_t file_FirstClusterLowBit;
}__attribute__((packed)) FAT32_FILE;

typedef struct{
    volatile uint32_t arg;
    volatile uint32_t CMD;
}SDCMD;

typedef struct{
    volatile uint32_t DMA_SD;
    volatile uint16_t BS_SD;
    volatile uint16_t BC16_SD;
    volatile uint32_t ARG_SD; //Argument Register
    volatile uint32_t CMD_SD;
    volatile uint32_t RESP_SD[4]; //Respond Register
    volatile uint32_t BDP_SD; //Buffer Data Port
    volatile uint32_t PS_SD; //Present State Register
    volatile uint8_t HC_SD; //Host Control Register 1
    volatile uint8_t PWC_SD; //Power Control Register
    uint16_t RESERVE_4[1];
    volatile uint16_t CC_SD; //Clock Control Register
    volatile uint8_t TOC_SD; //Timeout Control Register
    volatile uint8_t SR_SD; //Software Reset Register
    volatile uint16_t NS_SD; //Normal Interrput Status Register 
    volatile uint16_t ERS_SD; //Error Interrput Status Register
    volatile uint16_t NSE_SD; //Normal Interrput Status Enable Register 
    volatile uint16_t ERSE_SD; //Error Interrput Status Enable Register
    volatile uint16_t NSIE_SD; //Normal Interrput Signal Enable Register
    uint32_t RESERVE_6[1];
    volatile uint16_t HC2_SD;
    volatile uint64_t CB_SD; //Capabilites Register
}__attribute__((packed)) SDR;

typedef struct{
    uint8_t RESERVE_3[8];
    volatile uint32_t StartLBA;
    uint32_t RESERVE_4[1];
}__attribute__((packed)) MBRP;

typedef struct{
    volatile uint8_t BootCode[440];
    volatile uint32_t UMBRDS;
    uint16_t RESERVE_1[1];
    MBRP PartionRecord[4];
    volatile uint16_t Signature;
}__attribute__((packed)) MBR;