#include<std.h>

void print(char _buffer[]){
    while(UARTPL011->UART_FR & (1ULL << 3)){
        __asm__("NOP");
    }
    clear_buffer(Tx_buffer.buffer);
    Tx_buffer.head = length_s(_buffer);
    Tx_buffer.tail = 0;
    for(; Tx_buffer.tail <= Tx_buffer.head; Tx_buffer.tail++){
        Tx_buffer.buffer[Tx_buffer.tail] = _buffer[Tx_buffer.tail];
    }
    Tx_buffer.tail = 0;

    switch(OutJump->UART_Standart){
        case 0x504C00B0:
            UARTPL011->UART_IMSC = (1ULL << 5);
            break;
        default:
            break;
    }
}

void input(char _save_buffer[]){
    switch(OutJump->UART_Standart){
        case 0x504C00B0:
            while(UARTPL011->UART_FR & (1ULL << 3)){
                __asm__("NOP");
            }

            UARTPL011->UART_IMSC |= (1ULL << 4);
            UARTPL011->UART_IMSC |= (1ULL << 6);
            break;
        default:
            break;
    }

    while(Rx_buffer.end == 0){
        __asm__("WFI");
        if(Rx_buffer.buffer[Rx_buffer.head] == '\r'){
            Rx_buffer.end = 1;
        }
        else{
            __asm__("NOP");
        }
    }

    switch(OutJump->UART_Standart){
        case 0x504C00B0:
            UARTPL011->UART_IMSC &= ~(1ULL << 4);
            UARTPL011->UART_IMSC &= ~(1ULL << 6);
            break;
        default:
            break;
    }

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
