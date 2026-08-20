#include<165050.h>

UART165050R* UART165050;

void write_165050(){
    while((UART165050->UART_LSR & (1ULL << 6)) && Tx_buffer.tail <= Tx_buffer.head){
        UART165050->UART_TRD = Tx_buffer.buffer[Tx_buffer.tail];
        Tx_buffer.tail++;
    }
}

void read_165050(){
    while(UART165050->UART_LSR & (1ULL << 0)){
        Rx_buffer.head++;
        Rx_buffer.buffer[Rx_buffer.head] = UART165050->UART_TRD;
    }
}

void irq_switch_165050(){
    if((UART165050->UART_IIF & 0xF) == 2){
        write_165050();
    }
    else if((UART165050->UART_IIF & 0xF) == 4 || (UART165050->UART_IIF & 0xF) == 12){
        read_165050();
    }
}

void wait_for_stop_165050(){
    while((UART165050->UART_LSR & (1ULL << 6)) == 0){
        __asm__("NOP");
    }
}

void reg_init_165050(){
    UART165050 = (UART165050R*)UART.UARTAddress;
}

void Tx_init_165050(){
    UART165050->UART_DEI |= (1ULL << 1);
}

void Rx_init_165050(){
    UART165050->UART_DEI |= (1ULL << 0);
}

void IRQ_disable_165050(){
    UART165050->UART_DEI &= ~((1ULL << 0) | (1ULL << 1));
}

const HALUARTF UART165050F = {
    .Register_init = &reg_init_165050,
    .IRQ_handel = &irq_switch_165050,
    .IRQ_Tx_init = &Tx_init_165050,
    .IRQ_Rx_init = &Rx_init_165050,
    .IRQ_disable = &IRQ_disable_165050,
    .wait_transmition = &wait_for_stop_165050
};