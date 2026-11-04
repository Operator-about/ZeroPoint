#pragma once

#include<stdint.h>

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
    uint8_t Name[260];
    uint32_t FirstCluster;
    uint8_t NoFATChain;
    uint64_t FileLength;
    int SecondaryCount;
    int NameIndex;
    uint16_t FileAttribute;
}FileInfo;
