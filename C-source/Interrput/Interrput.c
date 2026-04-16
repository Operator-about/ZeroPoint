#include"Interrput.h"

struct UART_buffer Tx_buffer;
struct UART_buffer Rx_buffer;

void write(char _buffer[]){
    Tx_clear();
    Tx_buffer.head = length(_buffer); //Указание длинны сообщения
    Tx_buffer.tail = 0;
    
    for(int _buffer_index = 0; _buffer_index < length(_buffer); _buffer_index++){
        Tx_buffer.buffer[_buffer_index] = _buffer[_buffer_index];
    }
    UART->UART_DR = Tx_buffer.buffer[Tx_buffer.tail];
}
void read(char _buffer[]){
    while(1){
        if(Rx_buffer.buffer[0] != '\0'){
            for(int _buffer_index = 0; _buffer_index < 5; _buffer_index++){
                __asm__("MOV X16, %0" : : "r"(Rx_buffer.head));
                _buffer[_buffer_index] = Rx_buffer.buffer[_buffer_index];
            }
            break;
        }
    }
}

void send(){
    UART->UART_ICR = (1ULL << 5); //Сброс прерывания
    if(Tx_buffer.tail < Tx_buffer.head && Tx_buffer.buffer[Tx_buffer.tail+1] != '\0'){
        Tx_buffer.tail++;
        UART->UART_DR = Tx_buffer.buffer[Tx_buffer.tail]; //Запись значения
    }
}

void receving(){
    UART->UART_ICR = (1ULL << 4);
    if(UART->UART_DR != '\r' && UART->UART_DR != '\n'){
        __asm__("ADD X14, X14, #1");
        Rx_buffer.head++; 
        Rx_buffer.buffer[Rx_buffer.head] = UART->UART_DR; 
    }
}

void GIC_interrput(){
    volatile uint64_t _IAR_ID;
    __asm__("MRS %0, ICC_IAR1_EL1" : "=r"(_IAR_ID));
    if(_IAR_ID == 33){
        if(UART->UART_MIS & (1ULL << 5)){
            send();
        }
        else if(UART->UART_MIS & (1ULL << 4)){
            receving();
        }
    }
    //DIR используется из-за включённого в CTLR бита EOImode1NS
    __asm__("MSR ICC_EOIR1_EL1, %0" : : "r"(_IAR_ID)); //Завершение прерывание
    __asm__("MSR ICC_DIR_EL1, %0" : : "r"(_IAR_ID)); //Сброс прерывания
}

struct BRR_UART calculate_BRR(int _BRR, int _tact){
    
    //I - основная скорость передачи в UART
    //F - можно сказать, что это стабилизатор I. Измерение в I могут быть чу-чуть не точные, так, как полсе деления остаётся остаток, который никуда не записывается
    //F работает так: берёт остаток от деления при расчёте I и округляет его. Это делается для того, чтобы скорость передачи была более стабильной
    //Формула для I показана в коде. Но вот ещё-раз: ТАКТ / ЧИСТУЮ_СКОРОСТЬ * 16
    uint32_t _BRDDIVF = (uint32_t)(((((_tact % (_BRR * 16)) * 64) + 32) / 64) & 0x3F); //Расчёт FBRD
    uint32_t _BRDDIVI = (uint32_t)(_tact / (_BRR * 16)); //Расёт IBRD

    struct BRR_UART _BRR_local = {.IBRD = _BRDDIVI, .FBRD = _BRDDIVF};
    return _BRR_local;
}

int length(char _buffer[]){
    int _length = 0;
    while(_buffer[_length] != '\0'){
        _length++;
    }

    return _length;
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
}
