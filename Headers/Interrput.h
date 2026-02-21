#pragma once

#include"Custom-type.h"
#include<math.h>

extern struct UART_buffer Tx_buffer;
extern struct UART_buffer Rx_buffer;

//Функции для проверки доступности UART
struct UART check_UART_avaible_common();
struct UART check_UART_avaible_qcom();
struct UART check_UART_avaible_md();

//Заглушки
void write(char _buffer[], struct UART _UART);
char* read();

struct BRR_UART calculate_BRR(int _BRR, int _tack);

//I/O функции
void send(struct UART _UART);
void reciving(struct UART _UART);

void UART_common_configure(struct UART _UART);
void UART_md_configure(struct UART _UART);
void UART_qcom_configure(struct UART _UART);