#include<std.h>

void print(char _buffer[]){
    while(*UART.UART_FR & (1ULL << 3)){
        __asm__("NOP");
    }
    clear_buffer(Tx_buffer.buffer);
    Tx_buffer.head = length_s(_buffer);
    Tx_buffer.tail = 0;
    for(; Tx_buffer.tail <= Tx_buffer.head; Tx_buffer.tail++){
        Tx_buffer.buffer[Tx_buffer.tail] = _buffer[Tx_buffer.tail];
    }
    Tx_buffer.tail = 0;
    *UART.UART_IMSC = (1ULL << 5);
}

void input(char _save_buffer[]){
    while(*UART.UART_FR & (1ULL << 3)){
        __asm__("NOP");
    }

    *UART.UART_IMSC |= (1ULL << 4);
    *UART.UART_IMSC |= (1ULL << 6);
    while(Rx_buffer.end == 0){
        __asm__("WFI");
        if(Rx_buffer.buffer[Rx_buffer.head] == '\r'){
            Rx_buffer.end = 1;
        }
        else{
            __asm__("NOP");
        }
    }

    *UART.UART_ICR = (1ULL << 6);
    *UART.UART_IMSC &= ~(1ULL << 4);
    *UART.UART_IMSC &= ~(1ULL << 6);

    Rx_buffer.buffer[Rx_buffer.head] = '\0';
    Rx_buffer.head--;

    for(; Rx_buffer.tail <= Rx_buffer.head; Rx_buffer.tail++){
        _save_buffer[Rx_buffer.tail] = Rx_buffer.buffer[Rx_buffer.tail];
    }
    clear_buffer(Rx_buffer.buffer);
    Rx_buffer.head = -1;
    Rx_buffer.end = 0;
    Rx_buffer.tail = 0;
}

void println(char _buffer[]){
    
}
