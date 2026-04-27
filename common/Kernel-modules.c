#include<Kernel-modules.h>



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