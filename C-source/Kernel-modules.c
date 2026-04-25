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

