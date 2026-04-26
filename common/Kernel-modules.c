#include<Kernel-modules.h>

int length(char _buffer[]){
    int _length = 0;
    while(_buffer[_length] != '\0'){
        _length++;
    }

    return _length;
}

void clear_buffer(char _buffer[]){
    int _length = length(_buffer);
    for(int _clear_index; _clear_index < _length; _clear_index++){
        _buffer[_clear_index] = '\0';
    }
}

int compare_s(char _comble_buffer[], char _src_buffer[]){
    int _result = 0;
    int _index = 0;
    while(1){
        if(_comble_buffer[_index] == _src_buffer[_index]){
            _result = 1;
        }
        else{
            _result = 0;
            break;
        }

        if(_comble_buffer[_index] == '\0'){
            break;
        }
        _index++;
    }

    return _result;
}

int GIC_version_check(){
    uint64_t _GIC_version;
    __asm__("MRS %0, ID_AA64PFR0_EL1" : "=r"(_GIC_version));
    if(_GIC_version & (0b0001 << 24)){
        return 3;
    }
    return 2;
}