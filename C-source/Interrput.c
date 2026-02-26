#include<Interrput.h>

struct UART_buffer Tx_buffer;
struct UART_buffer Rx_buffer;

void write(char _buffer[]){
    Tx_buffer.head = strlen(_buffer); //Указание длинны сообщения
    Tx_buffer.tail = 1;
    
    for(int _buffer_index = 0; _buffer_index < strlen(_buffer); _buffer_index++){
        Tx_buffer.buffer[_buffer_index] = (uint8_t)_buffer[_buffer_index]; //Заполнение буфера
    }

    *UART.UART_TDR = Tx_buffer.buffer[0]; //"Зажигание" 
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

void send(){
    //Передача в FIFO
    while(*UART.UART_FR & (1 << 7) && *UART.UART_FR & (0 << 5) && Tx_buffer.tail < Tx_buffer.head){
        *UART.UART_TDR = Tx_buffer.buffer[Tx_buffer.tail];
        Tx_buffer.tail++; //Записывание текущей позиции
    }

    if(Tx_buffer.tail == Tx_buffer.head){
        *UART.UART_ICR &= (0 << 5);

        volatile uint32_t _IAR_for_EOI;

        __asm__("MRS %0, ICC_IAR1_EL1" : "=r"(_IAR_for_EOI));
        __asm__("MSR ICC_EOI1R_EL1, %0" : "r"(_IAR_for_EOI));
    }
}

void receving(){
    while(*UART.UART_FR & (0 << 4) && *UART.UART_FR & (0 << 6)){
        Rx_buffer.buffer[Rx_buffer.head] = *UART.UART_TDR;
        Rx_buffer.head++;
    }

    if(*UART.UART_TDR == (uint8_t)'\0'){
        *UART.UART_ICR &= (0 << 4);

        volatile uint32_t _IAR_for_EOI;

        __asm__("MRS %0, ICC_IAR1_EL1" : "=r"(_IAR_for_EOI));
        __asm__("MSR ICC_EOI1R_EL1, %0" : "r"(_IAR_for_EOI));
    }

}

void GIC_interpput(){
    volatile uint32_t _IAR_ID;

    __asm__("MRS %0, ICC_IAR1_EL1" : "=r"(_IAR_ID));

    if(_IAR_ID == UART.interrput){
        if(*UART.UART_MIS & (1 << 5)){
            send();
        }
        else if(*UART.UART_MIS & (1 << 4)){
            receving();
        }
    }
    else if(_IAR_ID == 1023){
        __asm__("MSR ICC_EOI1R_EL1, %0" : "r"(_IAR_ID));
    }
}

struct BRR_UART calculate_BRR(int _BRR, int _tact){
    
    double _BRDDIVF = _tact % (_BRR * 16); //Расчёт FBRD
    int _BRDDIVI = _tact / (_BRR * 16); //Расёт IBRD

    struct BRR_UART _BRR_local = {.IBRD = _BRDDIVI, .FBRD = round(_BRDDIVF * 64)};
    return _BRR_local;
}