#include<Mathz.h>

int power_two(int _src){
    int _result = 1;
    for(int _power_index = 1; _power_index <= _src; _power_index++){
        _result = _result * 2;
    }
    return _result;
}

int get_number_length(int _number){
    int _out = 0;
    while(_number > 0){
        _out++;
        _number = _number / 10;
    }
    return _out;
}