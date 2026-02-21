#include<Interrput.h>

struct UART_buffer Tx_buffer;
struct UART_buffer Rx_buffer;

void write(char _buffer[], struct UART _UART){
    Tx_buffer.head = strlen(_buffer); //Указание длинны сообщения
    Tx_buffer.tail = 1;
    
    for(int _buffer_index = 0; _buffer_index < strlen(_buffer); _buffer_index++){
        Tx_buffer.buffer[_buffer_index] = (uint8_t)_buffer[_buffer_index]; //Заполнение буфера
    }

    *_UART.UART_TDR = Tx_buffer.buffer[0]; //"Зажигание" 
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
    //Передача в FIFO
    while(*_UART.UART_FR & (1 << 7) && *_UART.UART_FR & (0 << 5) && Tx_buffer.tail < Tx_buffer.head){
        *_UART.UART_TDR = Tx_buffer.buffer[Tx_buffer.tail];
        Tx_buffer.tail++; //Записывание текущей позиции
    }
}

void reciving(struct UART _UART){
    if(*_UART.UART_FR & (0 << 4)){
        Rx_buffer.buffer[Rx_buffer.head] = *_UART.UART_TDR;
        Rx_buffer.head++;
    }
}

struct BRR_UART calculate_BRR(int _BRR, int _tact){
    
    double _BRDDIVF = _tact % (_BRR * 16); //Расчёт FBRD
    int _BRDDIVI = _tact / (_BRR * 16); //Расёт IBRD

    struct BRR_UART _BRR_local = {.IBRD = _BRDDIVI, .FBRD = round(_BRDDIVF * 64)};
    return _BRR_local;
}