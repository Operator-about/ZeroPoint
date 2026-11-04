#pragma once

#include<stdint.h>

typedef struct{
    unsigned char Indef[16];
    uint16_t Type;
    uint16_t Machine;
    uint32_t Version;
    uint64_t Entry;
    uint64_t Phoff;
    uint64_t shoff;
    uint32_t Flags;
    uint16_t Ehsize;
    uint16_t Phentsize;
    uint16_t Phum;
    uint16_t Shentsize;
    uint16_t Shnum;
    uint16_t Shstrndx;
}ELF64;
