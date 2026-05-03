#include<Kernel-modules.h>

int get_number_length(int _number){
    int _out = 0;
    while((_number % 10) > 0){
        _out++;
        _number = _number / 10;
    }
    return _out;
}

void VBAR_set(){
    __asm__("ADR X0, vector_table_center");
    __asm__("MSR VBAR_EL1, X0");
}

void clear_buffer(char _buffer[]){
    int _length = length_s(_buffer);
    for(int _clear_index; _clear_index < _length; _clear_index++){
        _buffer[_clear_index] = '\0';
    }
}

int GIC_version_check(){
    uint64_t _GIC_version;
    __asm__("MRS %0, ID_AA64PFR0_EL1" : "=r"(_GIC_version));
    if(_GIC_version & (0b0001 << 24)){
        return 3;
    }
    return 2;
}

int MMU_IPS_check(){
    uint64_t _MMU_IPS;
    __asm__("MRS %0, ID_AA64MMFR0_EL1" : "=r"(_MMU_IPS));
    if(_MMU_IPS & (5ULL << 0)){
        return 48;
    }
    else if(_MMU_IPS & (1ULL << 0)){
        return 36;
    }
    return 32;
}