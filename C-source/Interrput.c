#include<Interrput.h>

struct UART_buffer Tx_buffer;
struct UART_buffer Rx_buffer;

void write(char _buffer[]){
    Tx_buffer.head = strlen(_buffer);
    Tx_buffer.tail = 0;
    
    for(int _buffer_index = 0; _buffer_index < strlen(_buffer); _buffer_index++){
        Tx_buffer.buffer[_buffer_index] = (uint8_t)_buffer[_buffer_index];
    }
}

char* read(){
    Rx_buffer.tail = 0;

    char* _buffer = "";
    while(Rx_buffer.tail < Rx_buffer.head){
        _buffer += Rx_buffer.buffer[Rx_buffer.tail];
        Rx_buffer.tail++;
    }

    return _buffer;
}

void send(struct UART _UART){
    if(*_UART.UART_FR & (1 << 7) && Tx_buffer.tail < Tx_buffer.head){
        *_UART.UART_TDR = Tx_buffer.buffer[Tx_buffer.tail];
        Tx_buffer.tail++;
    }
}

void reciving(struct UART _UART){
    if(*_UART.UART_FR & (0 << 5)){
        Rx_buffer.buffer[Rx_buffer.head] = *_UART.UART_TDR;
        Rx_buffer.head++;
    }
}