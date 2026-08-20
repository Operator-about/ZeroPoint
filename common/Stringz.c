#include<Stringz.h>

int length_s(char _buffer[]){
    int _length = 0;
    while(_buffer[_length] != '\0' || _buffer[_length] != 0x0){
        _length++;
    }

    return _length;
}

int compare_s(uint8_t _comble_buffer[], char _src_buffer[]){
    int _result = 0;
    int _index = 0;
    while(1){
        if(_src_buffer[_index] == '\0'){
            break;
        }

        if(_comble_buffer[_index] == _src_buffer[_index]){
            _result = 1;
        }
        else{
            _result = 0;
            break;
        }
        _index++;
    }

    return _result;
}

void itos(int _number, char _out_buffer[]){
    int _size_number = get_number_length(_number);
    _size_number--;
    char _buffer = '\0';
    while(_number > 0){
        _buffer = (char)((_number % 10) + '0');
        _out_buffer[_size_number] = _buffer;
        _size_number--;
        _number = _number / 10;
    }
}

int stoi(char _resource_buffer[]){
    int _number = 0;
    int _buffer_size = length_s(_resource_buffer);
    for(int _index = 0; _index < _buffer_size; _index++){
        if((int)_resource_buffer[_index] >= (int)'0' && (int)_resource_buffer[_index] <= (int)'9'){
            _number = (((int)_resource_buffer[_index] - (int)'0') + (_number * 10)); //Сдвиг вправо(_number * 10)
            /*
                сдвиг + _number * 10 нужен, т.к. к примеру получено число 48 и в переменной _number уже лежало число. Умножая _number на 10 - программа сдвигает число в право. Вот так:
                48 -> 480. И теперь, когда у нас есть свободный 0 - программа может прибавить к 480 например 5
            */
        }
    }
    return _number;
}

int compare_u16_to_ASCII(uint8_t _compare_buffer[], char _src[]){
    int _compare_index = 0;
    int _src_index = 0;
    int _result = 0;

    while(1){
        if(_compare_buffer[_compare_index] == 0x0){
            _compare_index++;
        }

        if(_compare_buffer[_compare_index] == _src[_src_index]){
            _result = 1;
        }
        else{
            _result = 0;
            return _result;
        }

        if(_src[_src_index] == '\0'){
            return _result;
        }

        _compare_index++;
        _src_index++;
    }

    return _result;
}

void utf16_to_ASCII(uint8_t _utf16_buffer[], uint8_t _ASCII_out_buffer[]){
    int _end = 0;
    int _utf16_index = 0;
    int _ASCII_index = 0;

    while(_end < 2){
        if(_utf16_buffer[_utf16_index] == 0x0){
            _end++;
            _utf16_index++;
        }

        if(_utf16_buffer[_utf16_index] != 0x0 && _end < 2){
            _end = 0;
            _ASCII_out_buffer[_ASCII_index] = _utf16_buffer[_utf16_index];
            _ASCII_index++;
        }

        _utf16_index++;
    }
}

void upper(uint8_t _buffer[]){
    int _index = 0;
    while(_buffer[_index] != 0x00){
        if(_buffer[_index] >= 'a' && _buffer[_index] <= 'z'){
            _buffer[_index] -= 32;
        }
        _index+=2;
    }
}