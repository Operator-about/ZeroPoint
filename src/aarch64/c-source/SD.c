#include<SD.h>

SDCMD CMD;
uint32_t DAT_buffer[8192];
int TransmitionEnd;

void CMD_send(CMDR _CMD){
    SD_Registers->ARG_SD = _CMD.arg;
    SD_Registers->CMD_SD = _CMD.CMD;
}

void read_multi_sector(uint32_t _sector){
    CMD.CMD23.arg = 0x0;
    CMD.CMD23.arg |= 64;
    CMD.CMD23.CMD = 0x0;
    CMD.CMD23.CMD = (2ULL << 16) | (1ULL << 19) | (1ULL << 20) | (23ULL << 24);
    
    CMD.CMD18.arg = 0x0;
    CMD.CMD18.arg |= _sector;
    CMD.CMD18.CMD = 0x0;
    CMD.CMD18.CMD = (1ULL << 1) | (1ULL << 4) | (1ULL << 5) | (2ULL << 16) | (1ULL << 19) | (1ULL << 20) | (1ULL << 21) | (18ULL << 24);
    CMD_send(CMD.CMD23);
    while((SD_Registers->NS_SD & (1ULL << 0)) == 0){
        __asm__("NOP");
    }
    CMD_send(CMD.CMD18);
    sec_barrier(50);
}

void read_single_sector(uint32_t _sector){
    CMD.CMD17.arg = 0x0;
    CMD.CMD17.arg |= _sector;
    CMD.CMD17.CMD = 0x0;
    CMD.CMD17.CMD &= ~(1ULL << 0);
    CMD.CMD17.CMD &= ~(1ULL << 5);
    CMD.CMD17.CMD |= (1ULL << 1);
    CMD.CMD17.CMD |= (1ULL << 4);
    CMD.CMD17.CMD |= (2ULL << 16) | (1ULL << 19) | (1ULL << 20) | (1ULL << 21) | (17ULL << 24);
    CMD_send(CMD.CMD17);
    sec_barrier(50);
}

void IRQ_read(){
    File.Current_index = 0;
    File.Buffer_index = 0;
    uint32_t _tempory_buffer = 0x0;
    int _timeout = 100;
    for(int _index = 0; _index < 8192; _index++){
        while(_timeout != 0){
            _timeout--;
        }
        
        _tempory_buffer = SD_Registers->BDP_SD;
        _timeout = 100;
        
        for(int _build = 0; _build < 4; _build++){
            File.Buffer[File.Buffer_index] = 0x0;
            File.Buffer[File.Buffer_index] = (_tempory_buffer >> (8 * _build));
            File.Buffer_index++;
        }

        _tempory_buffer = 0x0;
    }
}