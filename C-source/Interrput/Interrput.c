#include"Interrput.h"

struct UART_buffer Tx_buffer;
struct UART_buffer Rx_buffer;

void write(char _buffer[]){

    Tx_buffer.head = length(_buffer); //Указание длинны сообщения
    Tx_buffer.tail = 1;
    
    for(int _buffer_index = 0; _buffer_index < length(_buffer); _buffer_index++){
        Tx_buffer.buffer[_buffer_index] = _buffer[_buffer_index];
    }

    UART->UART_DR = Tx_buffer.buffer[0];
}
char* read(){
    Rx_buffer.tail = 0;

    char* _buffer = "";
    while(Rx_buffer.tail < Rx_buffer.head){
        // _buffer += Rx_buffer.buffer[Rx_buffer.tail];
        // Rx_buffer.tail++;
    }

    return _buffer;
}

void send(){
    UART->UART_ICR = UART->UART_MIS;
}

void receving(){
    //! - так как идёт проверка: FIFO НЕ РАВНО = ЧТО ОНО ЗАПОЛНЕНО !(т.е. 1 << 4 FIFO НЕ РАВНО 0). А побитовое & всегда даст 0
    //4 - FIFO EMPETY
    //6 - FIFO FULL
 
}

void GIC_interrput(){
    volatile uint64_t _IAR_ID;
    __asm__("MRS %0, ICC_IAR1_EL1" : "=r"(_IAR_ID));
    if(_IAR_ID == UART->interrput){
        if(UART->UART_MIS & (1ULL << 5)){
            send();
        }
    }
    else if(_IAR_ID == 1023){
        __asm__("MSR ICC_EOIR1_EL1, %0" : : "r"(_IAR_ID));
    }
    
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