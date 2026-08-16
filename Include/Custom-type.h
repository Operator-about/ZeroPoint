#pragma once

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
    volatile uint32_t UART_DR;
    uint32_t RESERVE_1[5];
    volatile uint32_t UART_FR;
    uint32_t RESERVE_2[6];
    volatile uint32_t UART_IFLS;
    volatile uint32_t UART_IMSC;
    volatile uint32_t UART_RIS;
    volatile uint32_t UART_MIS;
    volatile uint32_t UART_ICR;
}UARTPL011R;

typedef struct{
    volatile uint32_t UART_TRD;
    volatile uint32_t UART_DEI;
    volatile uint32_t UART_IIF;
    volatile uint32_t UART_LCR;
    volatile uint32_t UART_MCR;
    volatile uint32_t UART_LSR;
    uint32_t RESERVE_1[22];
    volatile uint32_t UART_FAR;
}UART165050R;

typedef struct{
    uint8_t* buffer; //Буфер
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
    uint8_t RESERVE_1[8];
    uint8_t StartLBA[4];
    uint8_t RESERVE_2[4];
}__attribute__((packed)) MBRP;

typedef struct{
    uint8_t RESERVE_1[446];
    MBRP PartionsRecords[4];
    uint8_t RESERVE_2[3];
}__attribute__((packed)) MBR;

typedef struct{
    uint8_t RESERVE_1[3];
    volatile uint8_t FileSystemName[8];
    volatile uint8_t MustZero[53];
    uint8_t RESERVE_2[16];
    volatile uint32_t FATOffset;
    uint8_t RESERVE_3[4];
    volatile uint32_t ClusterHeapOffset;
    uint8_t RESERVE_4[4];
    volatile uint32_t FirstRootCluster;
    uint8_t RESERVE_5[8];
    volatile uint8_t BytsPerSector;
    volatile uint8_t SectorsPerCluster;
    uint8_t RESERVE_6[400];
    volatile uint8_t Signature[2];
} __attribute__((packed)) exFAT;

typedef struct{
    volatile uint32_t BytsPerSector;
    volatile uint32_t SectorsPerCluster;
    volatile uint32_t DATALBA;
    volatile uint32_t FATLBA;
    volatile uint32_t FirstRootCluster;
    volatile uint32_t CurrentRoot;
}exFATAttrubute;

typedef struct{
    volatile uint8_t EntryType;
    volatile uint8_t BitmapFlags;
    uint8_t RESERVE_1[18];
    volatile uint32_t FirstCluster;
    volatile uint64_t DataLength;
}__attribute__((packed))BitMapAllocationDescriptor;

typedef struct{
    volatile uint8_t EntryType;
    volatile uint8_t SecondaryCount;
    uint8_t RESERVE_1[2];
    volatile uint16_t FileAttributes;
}FileEntryDescriptor;

typedef struct{
    volatile uint8_t EntryType;
    volatile uint8_t GenralSecondaryFlags;
    uint8_t RESERVE_1[18];
    volatile uint32_t FirstCluster;
    volatile uint64_t DataLength;    
}__attribute__((packed))StreamExtensionsDescriptor;

typedef struct{
    volatile uint8_t EntryType;
    volatile uint8_t GenralSecondaryFlags;
    volatile uint8_t FileName[30];
}FileNameDescriptor;

typedef struct{
    uint8_t Buffer[32768];
    int Buffer_index;
    int Current_index;
}FileBuffer;

typedef struct{
    uint8_t Name[260];
    uint32_t FirstCluster;
    uint8_t NoFATChain;
    uint64_t FileLength;
    int SecondaryCount;
    int NameIndex;
    uint16_t FileAttribute;
}FileInfo;

typedef struct{
    volatile uint64_t SD;
    volatile uint64_t GICv2;
    volatile uint64_t UART;
    uint32_t UART_Standart;
    uint32_t SD_Standart;
    int UART_ID;
    int SD_ID;
}JumpData;

typedef struct{
    void (*IRQ_handel)();
    void (*wait_transmition)();
    void (*Register_init)();
    void (*IRQ_Rx_init)();
    void (*IRQ_Tx_init)();
    void (*IRQ_disable)();
}HALUARTF;

typedef struct{
    uint64_t UARTAddress;
    const HALUARTF* UARTF;
}HALUART;

typedef struct{
    void (*IRQ_read)();
    void (*CMD_send)(CMDR _CMD);
    void (*single_read)(uint32_t _sector);
    void (*multi_read)(uint32_t _sector);
    void (*block_init)();
    void (*register_init)();
    void (*wait_command)();
}HALSDF;

typedef struct{
    uint64_t SDAddress;
    const HALSDF* SDF;
}HALSD;