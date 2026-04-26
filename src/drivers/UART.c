#include<UART.h>

struct Ring_buffer Tx_buffer;
struct Ring_buffer Rx_buffer;

void write(char _buffer[]){
    Tx_clear();
    Tx_buffer.head = length(_buffer); //Указание длинны сообщения
    Tx_buffer.tail = 0;
    
    for(int _buffer_index = 0; _buffer_index < length(_buffer); _buffer_index++){
        Tx_buffer.buffer[_buffer_index] = _buffer[_buffer_index];
    }
    UARTPL011->UART_DR = Tx_buffer.buffer[Tx_buffer.tail];
}

void read(char _buffer[]){
    while(Rx_buffer.end == 0){
        __asm__("WFI"); //Если передача не закончена, то процессор падает в WFI для ожидания прерывания
    }
    check_text_from_Rx();
    for(int _index = 0; _index < Rx_buffer.head; _index++){
        _buffer[_index] = Rx_buffer.buffer[_index];
    }
}

void UARTPL011_init(){
    UARTPL011->UART_IBRD = BRR_calculate(9600, 480000000).IBRD;
    UARTPL011->UART_FBRD = BRR_calculate(9600, 480000000).FBRD;
    UARTPL011->UART_LCR_H |= (1ULL << 4);
    UARTPL011->UART_IFLS &= ~(1ULL << 0);
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
    uint32_t _BRDDIVF = (uint32_t)(((((_GHZ % (_BRR * 16)) * 64) + 32) / 64) & 0x3F); //Расчёт FBRD
    uint32_t _BRDDIVI = (uint32_t)(_GHZ / (_BRR * 16)); //Расёт IBRD

    struct BRR_UART _BRR_local = {.IBRD = _BRDDIVI, .FBRD = _BRDDIVF};
    return _BRR_local;
}

void check_text_from_Rx(){
    int _need_cleared = 0;
    int _need_add = 0;
    for(int _check_index = 0; _check_index < Rx_buffer.head; _check_index++){
        if(Rx_buffer.buffer[_check_index] == '\r' || Rx_buffer.buffer[_check_index] == '\n'){
            Rx_buffer.buffer[_check_index] = '\0';
            _need_cleared++;
        }
    }
    Rx_buffer.head-=_need_cleared;
    Rx_buffer.head+=1;
    Rx_buffer.buffer[Rx_buffer.head] = '\0';
}

void Tx_clear(){
    Tx_buffer.tail = 0;
    for(; Tx_buffer.tail < Tx_buffer.head; Tx_buffer.tail++){
        Tx_buffer.buffer[Tx_buffer.tail] = '\0';
    }
    Tx_buffer.tail = 0;
    Tx_buffer.head = 0;
}

void Rx_clear(){
    Rx_buffer.tail = 0;
    for(; Rx_buffer.tail < Rx_buffer.head; Rx_buffer.tail++){
        Rx_buffer.buffer[Rx_buffer.tail] = '\0';
    }
    Rx_buffer.tail = 0;
    Rx_buffer.head = -1;
    Rx_buffer.end = 0;
}