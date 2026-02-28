#include<Interrput.h>

void GIC_common_configure(){

    volatile uint64_t _IGROUPE1_ENABLED = (volatile uint64_t)0b1;
    __asm__("MSR ICC_IGRPEN1_EL1, %0" : : "r"(_IGROUPE1_ENABLED)); //Включение группы 1 прерываний

    volatile uint32_t _PMR = (volatile uint32_t)0xFF;
    __asm__("MSR ICC_PMR_EL1, %0" : : "r"(_PMR)); //Установление порога


    volatile int _index_ENABLER = 0;
    volatile int _index_PRIORITYR = 0;
    GIC.GICD_CTLR = (volatile uint32_t*)(0x08000000 + 0x0000);
    *GIC.GICD_CTLR |= (0b1 << 6) | (0b1 << 1);

    for(int _ID = 0; _ID > 128; _ID++){

        _index_ENABLER = (_ID / 32);
        _index_PRIORITYR = (_ID / 4);

        GIC.GICD_ISENABLER[_index_ENABLER] = (volatile uint32_t*)(0x08000000 + 0x0100 + ((_ID / 32) * 4));
        GIC.GICD_IPRIORITY[_index_PRIORITYR] = (volatile uint32_t*)(0x08000000 + 0x400 + ((_ID / 4) * 4));
        GIC.GICD_IGROUPR[_index_ENABLER] = (volatile uint32_t*)(0x08000000 + 0x0080 + ((_ID / 32) * 4));
        GIC.GICD_IROUTER[_ID] = (volatile uint64_t*)(0x08000000 + 0x6000 + (_ID * 8));

        *GIC.GICD_ISENABLER[_index_ENABLER] |= (1 << (_ID % 32));
        *GIC.GICD_IPRIORITY[_index_PRIORITYR] |= (0xFF << (_ID % 4) * 8);
        *GIC.GICD_IROUTER[_ID] = 0x0;
        *GIC.GICD_IGROUPR[_index_ENABLER] |= (1 << (_ID % 32));

    }
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