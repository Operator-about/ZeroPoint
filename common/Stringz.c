#include<Stringz.h>

int length_s(char _buffer[]){
    int _length = 0;
    while(_buffer[_length] != '\0'){
        _length++;
    }

    return _length;
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

void i_to_s(int _number, char _out_buffer[]){
    int _index = 0;
    char _buffer = '\0';
    while((_number / 10) > 0){
        if((_number % 10) <= 9){
            _buffer = (char)((_number % 10) + 48); 
            _out_buffer[_index] = _buffer;
            _index++;
        }
        _number = _number % 10;
    }
    _number = 0;
    _buffer = '\0';
}