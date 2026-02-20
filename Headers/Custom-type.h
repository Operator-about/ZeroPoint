#pragma once
#define SIZE 1024

#include<stdint.h>
#include<string.h>
#include<stdbool.h>

struct UART{
    volatile unsigned int* UART; //Регистер UART
    volatile uint8_t* UART_TDR; //Регистр Tx линии и Rx линии
    volatile unsigned int* UART_BRR; //Регистр скорости
    volatile unsigned int* UART_CR1; //Регистр конфигурации
    volatile unsigned int* UART_FR; //Регистр состояния UART
    volatile int* UART_IBRD;
    volatile float* UART_FBRD;
};

struct UART_buffer{
    uint8_t buffer[SIZE];
    int head;
    int tail;
};