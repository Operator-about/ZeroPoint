#include<Kernel-modules.h>

Ring_buffer Tx_buffer;
Ring_buffer Rx_buffer;

void VBAR_set(){
    __asm__("ADR X0, vector_table_center");
    __asm__("MSR VBAR_EL1, X0");
}

void clear_buffer(uint8_t _buffer[]){
    int _index = 0;
    while(_buffer[_index] != 0x0){
        _buffer[_index] = 0x0;
        _index++;
    }

    _index = 0;
}

int GIC_version_check(){
    uint64_t _GIC_version;
    __asm__("MRS %0, ID_AA64PFR0_EL1" : "=r"(_GIC_version));
    if(((_GIC_version >> 24) & 0xF) & (1ULL << 0)){
        
        _GIC_version = 0x0;
        return 3;
    }
    _GIC_version = 0x0;
    return 2;
}

int MMU_IPS_check(){
    uint64_t _MMU_IPS;

    __asm__("MRS %0, ID_AA64MMFR0_EL1" : "=r"(_MMU_IPS));
    if((_MMU_IPS & 0xF) == 0){
        _MMU_IPS = 0x0;
        return 32;
    }
    else if((_MMU_IPS & 0xF) == 1){
        _MMU_IPS = 0x0;
        return 36;
    }
}

int MMU_TG_check(){
    uint64_t _MMU_TG;

    __asm__("MRS %0, ID_AA64MMFR0_EL1" : "=r"(_MMU_TG));
    _MMU_TG = ((_MMU_TG >> 28) & 0xF);
    if((_MMU_TG & (0xF << 0)) == 0x0){
        return 4;
    }
}

void sec_barrier(int _second){
    for(int _wait = 0; _wait < 60 * 60 * _second; _wait++){
        __asm__("NOP");
    }
}

void uint32_t_build(uint32_t* _variable, uint8_t _data[]){
    for(int _build = 0; _build < 4; _build++){
        *_variable |= (_data[_build] << (8 * _build));
    }
}

uint64_t min_uint64_t(uint64_t _buffer[]){
    int _index = 1;
    uint64_t _current_uint64_t = _buffer[0];
    while(_buffer[_index] != 0x0){
        if(_buffer[_index] < _current_uint64_t){
            _current_uint64_t = _buffer[_index];
        }

        _index++;
    }

    return _current_uint64_t;
}