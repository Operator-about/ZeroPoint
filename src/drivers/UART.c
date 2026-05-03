#include<UART.h>

struct Ring_buffer Tx_buffer;
struct Ring_buffer Rx_buffer;

void write(char _buffer[]){
    clear_buffer(Tx_buffer.buffer);
    Tx_buffer.head = length_s(_buffer); //Указание длинны сообщения
    Tx_buffer.tail = 0;
    
    for(int _buffer_index = 0; _buffer_index < length_s(_buffer); _buffer_index++){
        Tx_buffer.buffer[_buffer_index] = _buffer[_buffer_index];
    }
    
    while(!(UARTPL011->UART_FR & (1ULL << 5)) && Tx_buffer.buffer[Tx_buffer.tail+1] != '\0'){
        if(Tx_buffer.tail < Tx_buffer.head){
            UARTPL011->UART_DR = Tx_buffer.buffer[Tx_buffer.tail];
            Tx_buffer.tail++;
        }
    }
}

void read(char _buffer[]){
    while(Rx_buffer.end == 0){
        __asm__("WFI"); //Если передача не закончена, то процессор падает в WFI для ожидания прерывания
    }
    Rx_buffer.buffer[Rx_buffer.head] = '\0'; //Очистка от \n
    Rx_buffer.buffer[Rx_buffer.head-1] = '\0'; //Очистка от \r
    Rx_buffer.head--;
    for(int _index = 0; _index < Rx_buffer.head; _index++){
        _buffer[_index] = Rx_buffer.buffer[_index];
    }

    clear_buffer(Rx_buffer.buffer);
    Rx_buffer.head = -1;
    Rx_buffer.end = 0;
    Rx_buffer.tail = 0;
}

void UARTPL011_init(){
    UARTPL011->UART_IBRD = BRR_calculate(9600, 480000000).IBRD;
    UARTPL011->UART_FBRD = BRR_calculate(9600, 480000000).FBRD;
    UARTPL011->UART_LCR_H |= (1ULL << 4);
    UARTPL011->UART_IFLS |= (2ULL << 0);
    UARTPL011->UART_IFLS |= (2ULL << 3);
    UARTPL011->UART_IMSC |= (1ULL << 4) | (1ULL << 5);
    UARTPL011->UART_CR |= (1ULL << 8) | (1ULL << 9);
    UARTPL011->UART_CR |= (1ULL << 0);
}

struct BRR_UART BRR_calculate(int _BRR, int _GHZ){
    //I - основная скорость передачи в UART
    //F - можно сказать, что это стабилизатор I. Измерение в I могут быть чу-чуть не точные, так, как полсе деления остаётся остаток, который никуда не записывается
    //F работает так: берёт остаток от деления при расчёте I и округляет его. Это делается для того, чтобы скорость передачи была более стабильной
    //Формула для I показана в коде. Но вот ещё-раз: ТАКТ / ЧИСТУЮ_СКОРОСТЬ * 16
    struct BRR_UART _BRR_local = {.IBRD = (uint32_t)(_GHZ / (_BRR * 16)), 
        .FBRD = (uint32_t)(((((_GHZ % (_BRR * 16)) * 64) + 32) / 64) & 0x3F)};
    return _BRR_local;
}