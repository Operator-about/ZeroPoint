#include<Interrput.h>

struct UART check_UART_avaible_common(){
    unsigned int* _UART_address = 0x09000000; //Адрес UART PL011
    //Проверка на то: лежит в этом адресе, что-то
    if(_UART_address != NULL){
        //Если да, то записываются данные про UART PL011
        struct UART _UART = {.UART = _UART_address, .UART_CR1 = _UART_address + 0x03, .UART_TDR = _UART_address + 0x00, .UART_FR = _UART_address + 0x18, 
            .UART_LCR_H = _UART_address + 0x02C};
        return _UART;
    }
    else{
        return;
    }
}

void UART_common_configure(struct UART _UART){
    
    //Подсчёт IBRD и FBRD
    struct BRR_UART _BRR = calculate_BRR(9600, 48000000);

    //Настройка параметров
    *_UART.UART_CR1 |= (1 << 8) | (1 << 9);
                        //Чётность  //Проверка  //FIFO     //Режим   //M
    *_UART.UART_LCR_H |= (1 << 1) | (0 << 2) | (1 << 4) | (0 << 7) | (8 << 5); //Настройка параметров передачи данных
    *_UART.UART_BRR = 9600; //Скорость передачи
    *_UART.UART_IBRD = _BRR.IBRD; 
    *_UART.UART_FBRD = _BRR.FBRD;
    *_UART.UART_CR1 |= (1 << 0); //Включение
}