#pragma once

#include"Custom-type.h"
#include<math.h>

extern struct UART_buffer Tx_buffer;
extern struct UART_buffer Rx_buffer;
extern struct UART UART;
extern struct GIC GIC;

//Функции для проверки доступности UART
struct UART check_UART_avaible_common();
struct UART check_UART_avaible_qcom();
struct UART check_UART_avaible_md();

//Заглушки
void write(char _buffer[]);
char* read();

struct BRR_UART calculate_BRR(int _BRR, int _tack);

//I/O функции
void send();
void receving();

void GIC_interpput();
void GIC_common_configure();

void UART_common_configure();
void UART_md_configure();
void UART_qcom_configure();