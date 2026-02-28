#include<Interrput.h>

void GIC_common_configure(){

    volatile uint32_t* _PMR = (volatile uint32_t*)0xFF;
    __asm__("MSR ICC_PMR_EL1, %0" : : "r"(_PMR)); //Установление порога

    GIC.GICD_IPRIORITY = (volatile uint32_t*)(0x08000000 + 0x400 + ((33 / 4) * 8)); //Запись адреса прерывания 33
    GIC.GICD_ISENABLER1 = (volatile uint32_t*)(0x08000000 + 0x0100  + ((33 / 32) * 4)); //Запись адреса прерывания 33
    GIC.GICD_IROUTER1 = (volatile uint64_t*)(0x08000000 + 0x6000 + (33 * 8)); //Запись адреса 

    *GIC.GICD_ISENABLER1 |= (1 << (33 % 32)); //Разрешение на прерывание по данному адресу
    *GIC.GICD_IROUTER1 |= 0x0; //Записывание, на какое именно ядро будет отправлено исключение 33
    *GIC.GICD_IPRIORITY |= (0xFF << ((33 / 4))); //Запись приоритета для прерывания
}


struct UART check_UART_avaible_common(){
    volatile uint32_t* UART_address = (volatile uint32_t*)0x09000000; //Адрес UART PL011
    //Проверка на то: лежит в этом адресе, что-то
    struct UART UART = {
        .UART = UART_address, 
        .UART_CR1 = (volatile uint32_t*)0x09000030, 
        .UART_TDR = (volatile uint32_t*)(UART_address + 0x00), 
        .UART_FR = (volatile uint32_t*)(UART_address + 0x18), 
        .UART_LCR_H = (volatile uint32_t*)0x0900002C, 
        .UART_MIS = (volatile uint32_t*)(UART_address + 0x040), 
        .UART_ICR = (volatile uint32_t*)UART_address + 0x044, 
        .UART_IFLS = (volatile uint32_t*)(UART_address + 0x034), 
        .UART_MSC = (volatile uint32_t*)(UART_address + 0x38), 
        .interrput = (volatile uint32_t)33};
    return UART;
}

void UART_common_configure(){
    
    //Подсчёт IBRD и FBRD
    struct BRR_UART _BRR = calculate_BRR(9600, 48000000);

    //Настройка параметров
    *UART.UART_CR1 = 0;
    *UART.UART_BRR = 9600; //Скорость передачи
    *UART.UART_IBRD = _BRR.IBRD; 
    *UART.UART_FBRD = _BRR.FBRD;
                        //Чётность  //Проверка  //FIFO     //Режим   //M
    *UART.UART_LCR_H |= (1 << 1) | (0 << 2) | (1 << 4) | (0 << 7) | (3 << 5); //Настройка параметров передачи данных
                       //Rx FIFO     //Tx FIFO
    *UART.UART_IFLS |= (0b10 << 3) | (0b10 << 0);
                      //Разрешение Rx //Разрешение Tx
    *UART.UART_MSC |= (1 << 5) | (1 << 4);
    *UART.UART_CR1 |= (1 << 0) | (1 << 8) | (1 << 9);
}