#include<Kstd.h>

void print(char _buffer[]){
    UART.UARTF->wait_transmition();
    Tx_buffer.head = length_s(_buffer);
    Tx_buffer.tail = 0;
    for(; Tx_buffer.tail <= Tx_buffer.head; Tx_buffer.tail++){
        Tx_buffer.buffer[Tx_buffer.tail] = 0x0;
        Tx_buffer.buffer[Tx_buffer.tail] = _buffer[Tx_buffer.tail];
    }
    Tx_buffer.tail = 0;

    UART.UARTF->IRQ_Tx_init();
}

void input(char _save_buffer[]){
    UART.UARTF->wait_transmition();
    UART.UARTF->IRQ_Rx_init();

    while(Rx_buffer.end == 0){
        __asm__("WFI");
        if(Rx_buffer.buffer[Rx_buffer.head] == '\r'){
            Rx_buffer.end = 1;
        }
        else{
            __asm__("NOP");
        }
    }

    UART.UARTF->IRQ_disable();

    Rx_buffer.buffer[Rx_buffer.head] = '\0';
    Rx_buffer.head--;

    for(; Rx_buffer.tail <= Rx_buffer.head; Rx_buffer.tail++){
        _save_buffer[Rx_buffer.tail] = Rx_buffer.buffer[Rx_buffer.tail];
        Rx_buffer.buffer[Rx_buffer.tail] = 0x0;
    }
    Rx_buffer.head = -1;
    Rx_buffer.end = 0;
    Rx_buffer.tail = 0;
}

void printh64(uint64_t _src){
    for(int _index = 7; _index >= 0; _index--){
        printh((_src >> (8 * _index)) & 0xFF);
    }

    UART.UARTF->char_write('\r');
    UART.UARTF->char_write('\n');
}

void printh(uint8_t _src){
    char _chars[] = {"0123456789ABCDEF"};

    UART.UARTF->char_write(_chars[((_src & 0xF0) >> 4)]);
    UART.UARTF->char_write(_chars[_src & 0x0F]);
}

