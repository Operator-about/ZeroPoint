#include<Kernel-modules.h>

Ring_buffer Tx_buffer;
Ring_buffer Rx_buffer;

int get_number_length(int _number){
    int _out = 0;
    while((_number % 10) > 0){
        _out++;
        _number = _number / 10;
    }
    return _out;
}

void VBAR_set(){
    __asm__("ADR X0, vector_table_center");
    __asm__("MSR VBAR_EL1, X0");
}

void clear_buffer(char _buffer[]){
    int _index = 0;
    while(_buffer[_index] != '\0'){
        _buffer[_index] = '\0';
        _index++;
    }
}

int GIC_version_check(){
    uint64_t _GIC_version;
    __asm__("MRS %0, ID_AA64PFR0_EL1" : "=r"(_GIC_version));
    if(_GIC_version & (0b0001 << 24)){
        return 3;
    }
    return 2;
}

int MMU_IPS_check(){
    uint64_t _MMU_IPS;
    __asm__("MRS %0, ID_AA64MMFR0_EL1" : "=r"(_MMU_IPS));
    if(_MMU_IPS & (5ULL << 0)){
        return 48;
    }
    else if(_MMU_IPS & (1ULL << 0)){
        return 36;
    }
    return 32;
}

void send(){
    *UART.UART_IMSC &= ~(1ULL << 5);
    while(!(*UART.UART_FR & (1ULL << 5)) && Tx_buffer.tail <= Tx_buffer.head){
        *UART.UART_DR = Tx_buffer.buffer[Tx_buffer.tail];
        Tx_buffer.tail++;
    }
    if(Tx_buffer.tail < Tx_buffer.head){
        *UART.UART_IMSC = (1ULL << 5);
    }
}

void receving(){
    while(!(*UART.UART_FR & (1ULL << 4))){
        Rx_buffer.head++;
        Rx_buffer.buffer[Rx_buffer.head] = (uint8_t)(*UART.UART_DR);
    }
    return;
}
void init_t_buffer(){
    for(int _index = 0; _index < SIZE; _index++){
        Rx_buffer.buffer[_index] = '\0';
        Tx_buffer.buffer[_index] = '\0';
    }
}