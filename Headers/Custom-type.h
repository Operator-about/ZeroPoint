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
    volatile uint64_t* GICD_IROUTER1;
    volatile uint32_t* GICD_IPRIORITY; //Приоритет конкретному прерыванию
};

struct UART{
    volatile uint32_t* UART; //Регистер UART
    volatile uint32_t* UART_LCR_H; //Регистр настройки передачи
    volatile uint32_t* UART_TDR; //Регистр Tx линии и Rx линии
    volatile uint32_t* UART_BRR; //Регистр скорости
    volatile uint32_t* UART_CR1; //Регистр конфигурации
    volatile uint32_t* UART_FR; //Регистр состояния UART
    volatile int* UART_IBRD; //Коэффициент замедления скорости(int)
    volatile double* UART_FBRD; //Коэффициент замедление скорости(float)
    volatile uint32_t* UART_MIS; //Хранит, какое имено прерывание из 33 ID(Tx или Rx)
    volatile uint32_t* UART_ICR; //Обнуление прерывания
    volatile uint32_t* UART_IFLS; //Параметр срабатывания FIFO
    volatile uint32_t* UART_MSC; //Разрешает определённые прерывания
    int interrput;
};

struct UART_buffer{
    uint8_t buffer[SIZE];
    int head;
    int tail;
};