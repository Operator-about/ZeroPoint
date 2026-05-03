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

void itos(int _number, char _out_buffer[]){
    int _index = 0;
    int _size_number = get_number_length(_number);
    _size_number--;
    char _buffer = '\0';
    while((_number % 10) > 0){
        if((_number % 10) <= 9){
            _buffer = (char)((_number % 10) + 48); 
            _out_buffer[_size_number - _index] = _buffer;
            _index++;
        }
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