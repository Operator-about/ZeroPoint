#include<PL011.h>

UARTPL011R* UARTPL011;

void write_pl011(){
    while(!(UARTPL011->UART_FR & (1ULL << 5)) && Tx_buffer.tail <= Tx_buffer.head){
        UARTPL011->UART_DR = Tx_buffer.buffer[Tx_buffer.tail];
        Tx_buffer.tail++;
    }

    if(Tx_buffer.tail < Tx_buffer.head){
        UARTPL011->UART_IMSC = (1ULL << 5);
    }
    else{
        UARTPL011->UART_IMSC &= ~(1ULL << 5);
    }
}

void read_pl011(){
    while(!(UARTPL011->UART_FR & (1ULL << 4))){
        Rx_buffer.head++;
        Rx_buffer.buffer[Rx_buffer.head] = (uint8_t)(UARTPL011->UART_DR);
    }
    return;
}

void irq_switch_pl011(){
    if(UARTPL011->UART_MIS & (1ULL << 5)){
        write_pl011();
    }
    else if((UARTPL011->UART_MIS & (1ULL << 4)) || (UARTPL011->UART_MIS & (1ULL << 6))){
        read_pl011();
    }
}

void wait_for_stop(){
    while(UARTPL011->UART_FR & (1ULL << 3)){
        __asm__("NOP");
    }
}

void Reg_init(){
    UARTPL011 = (UARTPL011R*)UART.UARTAddress;
}

void Tx_init(){
    UARTPL011->UART_IMSC |= (1ULL << 5);
}

void Rx_init(){
    UARTPL011->UART_IMSC |= (1ULL << 4) | (1ULL << 6);
}

void IRQ_disable(){
    UARTPL011->UART_IMSC &= ~((1ULL << 4) | (1ULL << 6) | (1ULL << 5));
}

const HALUARTF UARTPL011F = {
    .Register_init = &Reg_init,
    .IRQ_handel = &irq_switch_pl011,
    .IRQ_Tx_init = &Tx_init,
    .IRQ_Rx_init = &Rx_init,
    .IRQ_disable = &IRQ_disable,
    .wait_transmition = &wait_for_stop
};