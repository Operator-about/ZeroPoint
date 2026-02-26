#pragma once
#define SIZE 1024

#include<stdint.h>
#include<string.h>
#include<stdbool.h>

struct BRR_UART{
    int IBRD;
    float FBRD; 
};

struct GIC{
    volatile uint32_t* GICD_ISENABLER1;
    volatile uint32_t* GICD_IROUTER1;
};

struct UART{
    volatile unsigned int* UART; //Регистер UART
    volatile unsigned int* UART_LCR_H; //Регистр настройки передачи
    volatile uint8_t* UART_TDR; //Регистр Tx линии и Rx линии
    volatile unsigned int* UART_BRR; //Регистр скорости
    volatile unsigned int* UART_CR1; //Регистр конфигурации
    volatile unsigned int* UART_FR; //Регистр состояния UART
    volatile int* UART_IBRD; //Коэффициент замедления скорости(int)
    volatile double* UART_FBRD; //Коэффициент замедление скорости(float)
    volatile unsigned int* UART_MIS; //Хранит, какое имено прерывание из 33 ID(Tx или Rx)
    volatile unsigned int* UART_ICR; //Обнуление прерывания
    volatile unsigned int* UART_IFLS; //Параметр срабатывания FIFO
    int interrput;
};

struct UART_buffer{
    uint8_t buffer[SIZE];
    int head;
    int tail;
};