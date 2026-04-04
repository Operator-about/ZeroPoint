#include<Ella.h>

struct UART_buffer Debug_buffer;

void debugf(char _info[]){
    Debug_buffer.head = 0;
    Debug_buffer.tail = 0;
    for(int _debug_index = 0; _debug_index < length(_info); _debug_index++){
        Debug_buffer.head++;
        Debug_buffer.buffer[_debug_index] = _info[_debug_index];
    }

    for(; Debug_buffer.tail < Debug_buffer.head; Debug_buffer.tail++){
        *(volatile uint32_t*)0x09000000 = Debug_buffer.buffer[Debug_buffer.tail];
    }
    *(volatile uint32_t*)0x09000000 = '\n';
}