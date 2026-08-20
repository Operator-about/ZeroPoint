#include<SD-DW.h>

SDRR* SDR;

void SDR_IRQ_read(){
    File.Current_index = 0;
    File.Buffer_index = 0;
    uint32_t _tempory_buffer = 0x0;
    
    for(int _index = 0; _index < 8192; _index++){
        _tempory_buffer = SDR->BDP_SD;
        
        for(int _build = 0; _build < 4; _build++){
            File.Buffer[File.Buffer_index] = 0x0;
            File.Buffer[File.Buffer_index] = (_tempory_buffer >> (8 * _build));
            File.Buffer_index++;
        }

        _tempory_buffer = 0x0;
    }
}

void SDR_register_init(){
    SDR = (SDRR*)SD.SDAddress;
}

void SDR_block_init(){
    SDR->BS_SD = 512;
    SDR->BC_SD = SDR->BS_SD * 64;
}

void SDR_CMD_send(CMDR _CMD){
    _CMD.CMD |= (1ULL << 31);

    uint32_t _tempory_CMD = _CMD.CMD;
    _CMD.CMD = 0x0;
    _CMD.CMD |= (((_tempory_CMD >> 24) & 0x3F) << 0); //Command Index
    _CMD.CMD |= (~((_tempory_CMD >> 4) & 0x1) << 10); //Data directory
    _CMD.CMD |= (((_tempory_CMD >> 21) & 0x1) << 9); //Data Present
    _CMD.CMD |= (((_tempory_CMD >> 19) & 0x1) << 8); //CRC check
    switch((_tempory_CMD >> 16) & 0x3){
        case 1:
            _CMD.CMD |= (1ULL << 7) | (1ULL << 6); //136 Resp + wait
            break;
        case 2:
            //48 Resp - wait
            _CMD.CMD &= ~(1ULL << 7);
            _CMD.CMD |= (1ULL << 6);
            break;
        case 3:
            //48 Resp + wait
            _CMD.CMD &= ~(1ULL << 7);
            _CMD.CMD |= (1ULL << 6);
            break;
    }
    
    SDR->ARG_SD = _CMD.arg;
    SDR->CMD_SD = _CMD.CMD;
}

void SDR_wait_command(){
    while(SDR->CMD_SD & (1ULL << 31)){
        __asm__("NOP");
    }
}

const HALSDF SDRF = {
    .CMD_send = &SDR_CMD_send,
    .IRQ_read = &SDR_IRQ_read,
    .multi_read = &read_multi_sector,
    .single_read = &read_single_sector,
    .register_init = &SDR_register_init,
    .block_init = &SDR_block_init,
    .wait_command = &SDR_wait_command
};