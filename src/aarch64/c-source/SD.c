#include<SD.h>

SDCMD CMD;
volatile uint32_t IRQ_DAT_buffer[8192];

void CMD_send(CMDR _CMD){
    SD_Registers->ARG_SD = _CMD.arg;
    SD_Registers->CMD_SD = _CMD.CMD;
}

void read_multi_sector(uint32_t _sector){
    CMD.CMD23.arg = 0x0;
    CMD.CMD23.arg |= exFAT_attribute.SectorsPerCluster & 0x0000FFFF;
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

    __asm__("ISB");
    __asm__("DSB SY");
}

void read_single_sector(uint32_t _sector){
    CMD.CMD17.arg = 0x0;
    CMD.CMD17.arg |= _sector;
    CMD.CMD17.CMD = 0x0;
    CMD.CMD17.CMD &= ~(1ULL << 0);
    CMD.CMD17.CMD &= ~(1ULL << 5);
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

void IRQ_read(){
    for(int _clear = 0; _clear < 8192; _clear++){
        IRQ_DAT_buffer[_clear] = 0x0;
    }

    for(int _index = 0; _index < 8192; _index++){
        __asm__("ISB");
        __asm__("DSB SY");
        IRQ_DAT_buffer[_index] = SD_Registers->BDP_SD;
    }
    DAT_buffer = (uint8_t*)IRQ_DAT_buffer;  
}