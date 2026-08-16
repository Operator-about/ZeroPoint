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

void wait_for_stop_pl011(){
    while(UARTPL011->UART_FR & (1ULL << 3)){
        __asm__("NOP");
    }
}

void reg_init_pl011(){
    UARTPL011 = (UARTPL011R*)UART.UARTAddress;
}

void Tx_init_pl011(){
    UARTPL011->UART_IMSC |= (1ULL << 5);
}

void Rx_init_pl011(){
    UARTPL011->UART_IMSC |= (1ULL << 4) | (1ULL << 6);
}

void IRQ_disable_pl011(){
    UARTPL011->UART_IMSC &= ~((1ULL << 4) | (1ULL << 6) | (1ULL << 5));
}

const HALUARTF UARTPL011F = {
    .Register_init = &reg_init_pl011,
    .IRQ_handel = &irq_switch_pl011,
    .IRQ_Tx_init = &Tx_init_pl011,
    .IRQ_Rx_init = &Rx_init_pl011,
    .IRQ_disable = &IRQ_disable_pl011,
    .wait_transmition = &wait_for_stop_pl011
};