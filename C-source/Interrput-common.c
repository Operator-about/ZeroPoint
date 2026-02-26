#include<Interrput.h>

void GIC_common_configure(){

    volatile unsigned int* _PMR = 0xFF;
    __asm__("MSR ICC_PMR_EL1, %0" : "r"(_PMR)); //Установление порога

    GIC.GICD_ISENABLER1 = 0x08000000 + 0x0100  + (33 / 32 * 4); //Запись адреса прерывания 33
    GIC.GICD_IROUTER1 = 0x08000000 + 0x6000 + (33 / 8); //Запись адреса 

    //Формула для вычесления адреса ID прерывания: 0x0100 + (n(номер регистра прерывания) * 4)
    //Формула для вычесление номера регистра прерывания: ID / 32  

    *GIC.GICD_ISENABLER1 |= (1 << (33 / 32)); //Разрешение на прерывание по данному адресу
    *GIC.GICD_IROUTER1 |= (1 << 0x0); //Записывание, на какое именно ядро будет отправлено исключение 33
}


struct UART check_UART_avaible_common(){
    unsigned int* _UART_address = 0x09000000; //Адрес UART PL011
    //Проверка на то: лежит в этом адресе, что-то
    if(_UART_address != NULL){
        //Если да, то записываются данные про UART PL011
        struct UART _UART = {.UART = _UART_address, .UART_CR1 = _UART_address + 0x03, .UART_TDR = _UART_address + 0x00, .UART_FR = _UART_address + 0x18, 
            .UART_LCR_H = _UART_address + 0x02C, .UART_MIS = _UART_address + 0x040, .UART_ICR = _UART_address + 0x044, 
            .UART_IFLS = _UART_address + 0x034, .interrput = 33};
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
    *_UART.UART_LCR_H |= (1 << 1) | (0 << 2) | (1 << 4) | (0 << 7) | (3 << 5); //Настройка параметров передачи данных
    *_UART.UART_BRR = 9600; //Скорость передачи
    *_UART.UART_IBRD = _BRR.IBRD; 
    *_UART.UART_FBRD = _BRR.FBRD;
                       //Rx FIFO     //Tx FIFO
    *_UART.UART_IFLS |= (0b10 << 3) | (0b10 << 0);
    *_UART.UART_CR1 |= (1 << 0); //Включение
}