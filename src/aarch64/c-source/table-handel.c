#include<table-handel.h>

void IRQh_handel(){
    volatile uint32_t _IAR = GICv2.GICC->GICC_IAR;
    __asm__("MOV X25, %0" : :"r"(_IAR));
    //*UART.UART_DR = 'A';
    if((_IAR & 0x3FF) == 153){
        if(*UART.UART_MIS & (1ULL << 5)){
            send();
        }
        else if(*UART.UART_MIS & (1ULL << 4)){
            *UART.UART_DR = 'P';
            receving();
        }
        GICv2.GICC->GICC_EOIR = _IAR;
        return;
    }
    else if((_IAR & 0x3FF) == 1023){
        *UART.UART_DR = 'H';
        GICv2.GICC->GICC_EOIR = _IAR;
    } 
    else if((_IAR & 0x3FF) == 1022){
        *UART.UART_DR = 'T';
        GICv2.GICC->GICC_EOIR = _IAR;
    } 
}