#pragma once

#include<stdint.h>

typedef struct{
    volatile uint32_t UART_DR;
    uint32_t RESERVE_1[5];
    volatile uint32_t UART_FR;
    uint32_t RESERVE_2[6];
    volatile uint32_t UART_IFLS;
    volatile uint32_t UART_IMSC;
    volatile uint32_t UART_RIS;
    volatile uint32_t UART_MIS;
    volatile uint32_t UART_ICR;
}UARTPL011R;

typedef struct{
    volatile uint32_t UART_TRD;
    volatile uint32_t UART_DEI;
    volatile uint32_t UART_IIF;
    volatile uint32_t UART_LCR;
    volatile uint32_t UART_MCR;
    volatile uint32_t UART_LSR;
    uint32_t RESERVE_1[22];
    volatile uint32_t UART_FAR;
}UART165050R;

typedef struct{
    uint8_t* buffer; //Буфер
    volatile int head; //Размер буфера
    volatile int tail; //Текущая позиция
    volatile int end; //Означет, что передача закончена
}Ring_buffer;

typedef struct{
    void (*IRQ_handel)();
    void (*wait_transmition)();
    void (*Register_init)();
    void (*IRQ_Rx_init)();
    void (*IRQ_Tx_init)();
    void (*IRQ_disable)();
    void (*char_write)(char _char);
}HALUARTF;

typedef struct{
    uint64_t UARTAddress;
    const HALUARTF* UARTF;
}HALUART;
