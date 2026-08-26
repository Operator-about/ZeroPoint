#include<SD.h>

SDCMD CMD;
FileBuffer File;
int ByteCount;

void read_multi_sector(uint32_t _sector){
    SD.SDF->block_init(exFAT_attribute.BytsPerSector, exFAT_attribute.SectorsPerCluster);
    
    CMD.CMD23.arg = 0x0;
    CMD.CMD23.arg |= exFAT_attribute.SectorsPerCluster;
    CMD.CMD23.CMD = 0x0;
    CMD.CMD23.CMD = (2ULL << 16) | (1ULL << 19) | (1ULL << 20) | (23ULL << 24);
    
    CMD.CMD18.arg = 0x0;
    CMD.CMD18.arg |= _sector;
    CMD.CMD18.CMD = 0x0;
    CMD.CMD18.CMD = (1ULL << 1) | (1ULL << 4) | (1ULL << 5) | (2ULL << 16) | (1ULL << 19) | (1ULL << 20) | (1ULL << 21) | (18ULL << 24);
    SD.SDF->CMD_send(CMD.CMD23);
    SD.SDF->wait_command();
    SD.SDF->CMD_send(CMD.CMD18);
    sec_barrier(50);
}

void read_single_sector(uint32_t _sector){
    SD.SDF->block_init(512, 1);
    
    CMD.CMD17.arg = 0x0;
    CMD.CMD17.arg |= _sector;
    CMD.CMD17.CMD = 0x0;
    CMD.CMD17.CMD &= ~(1ULL << 0);
    CMD.CMD17.CMD &= ~(1ULL << 5);
    CMD.CMD17.CMD |= (1ULL << 1);
    CMD.CMD17.CMD |= (1ULL << 4);
    CMD.CMD17.CMD |= (2ULL << 16) | (1ULL << 19) | (1ULL << 20) | (1ULL << 21) | (17ULL << 24);
    SD.SDF->CMD_send(CMD.CMD17);
    sec_barrier(50);
}