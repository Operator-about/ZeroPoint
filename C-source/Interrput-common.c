#include<Interrput.h>

struct UART check_UART_avaible(){
    unsigned int* _UART_address = 0x09000000;
    if(_UART_address != NULL){
        struct UART _UART = {.UART = _UART_address, .UART_CR1 = _UART_address + 0x03, .UART_TDR = _UART_address + 0x00, .UART_FR = _UART_address + 0x18};
        return _UART;
    }
    else{
        return;
    }
}

void UART_common_configure(struct UART _UART){
    
    *_UART.UART_CR1 |= (1 << 8) | (1 << 9) | (8 << 5);
    *_UART.UART_BRR = 9600;
    *_UART.UART_CR1 |= (1 << 0);
}