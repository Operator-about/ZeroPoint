#include<SD.h>

SDCMD CMD;

void CMD_send(CMDR _CMD){
    SD_Registers->ARG_SD = _CMD.arg;
    SD_Registers->CMD_SD = _CMD.CMD;
}

void read_multi_sector(uint32_t _sector){
    CMD.CMD23.arg = 0x0;
    CMD.CMD23.arg |= 64;
    CMD.CMD23.CMD = 0x0;
    CMD.CMD23.CMD |= (2ULL << 16) | (1ULL << 19) | (1ULL << 20) | (23ULL << 24);
    CMD_send(CMD.CMD23);
    sec_barrier(50);
    
    CMD.CMD18.arg = 0x0;
    CMD.CMD18.arg |= _sector;
    CMD.CMD18.CMD = 0x0;
    CMD.CMD18.CMD &= ~(1ULL << 0);
    CMD.CMD18.CMD |= (1ULL << 1) | (1ULL << 4) | (1ULL << 5);
    CMD.CMD18.CMD |= (2ULL << 16) | (1ULL << 19) | (1ULL << 20) | (1ULL << 21) | (18ULL << 24);
    CMD_send(CMD.CMD18);
    sec_barrier(50);
}

void read_single_sector(uint32_t _sector){
    CMD.CMD17.arg = 0x0;
    CMD.CMD17.arg |= _sector;
    CMD.CMD17.CMD = 0x0;
    CMD.CMD17.CMD &= ~(1ULL << 0);
    CMD.CMD17.CMD |= (1ULL << 1) | (1ULL << 4);
    CMD.CMD17.CMD |= (2ULL << 16) | (1ULL << 19) | (1ULL << 20) | (1ULL << 21) | (17ULL << 24);
    CMD_send(CMD.CMD17);
    sec_barrier(50);
}

void SD_mode(){
    CMD.CMD13.arg = 0x0;
    CMD.CMD13.arg |= SD;
    CMD.CMD13.CMD = 0x0;
    CMD.CMD13.CMD |= (2ULL << 16) | (1ULL << 19) | (1ULL << 20) | (13ULL << 24);
    CMD_send(CMD.CMD13);
}