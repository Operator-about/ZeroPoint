#include<SD-Standart.h>

SDAR* SDA;

void SDA_CMD_send(CMDR _CMD){
    SDA->ARG_SD = _CMD.arg;
    SDA->CMD_SD = _CMD.CMD;
}

void SDA_IRQ_read(){
    File.Current_index = 0;
    File.Buffer_index = 0;
    uint32_t _tempory_buffer = 0x0;
    
    for(int _index = 0; _index < 8192; _index++){
        _tempory_buffer = SDA->BDP_SD;
        
        for(int _build = 0; _build < 4; _build++){
            File.Buffer[File.Buffer_index] = 0x0;
            File.Buffer[File.Buffer_index] = (_tempory_buffer >> (8 * _build));
            File.Buffer_index++;
        }

        _tempory_buffer = 0x0;
    }
    
    SDA->NS_SD |= (1ULL << 5);
}

void SDA_register_init(){
    SDA = (SDAR*)SD.SDAddress;
}

void SDA_block_init(){
    SDA->BC_SD = 64;
    SDA->BS_SD = 512;
}

void SDA_wait_command(){
    while((SDA->NS_SD & (1ULL << 0)) == 0){
        __asm__("NOP");
    }
}

const HALSDF SDAF = {
    .CMD_send = &SDA_CMD_send,
    .IRQ_read = &SDA_IRQ_read,
    .multi_read = &read_multi_sector,
    .single_read = &read_single_sector,
    .register_init = &SDA_register_init,
    .block_init = &SDA_block_init,
    .wait_command = &SDA_wait_command
};