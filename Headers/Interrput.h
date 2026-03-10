#pragma once

#include"Custom-type.h"
#include<math.h>

extern struct UART_buffer Tx_buffer;
extern struct UART_buffer Rx_buffer;
extern volatile struct GIC GIC;
extern volatile struct UART UART;
extern volatile struct GIC_registers_data GIC_Registers;

//Функции для проверки доступности UART
void check_UART_avaible_common(volatile struct UART* _UART);
struct UART check_UART_avaible_qcom();
struct UART check_UART_avaible_md();

//Заглушки
void write(char _buffer[]);
char* read();

struct BRR_UART calculate_BRR(int _BRR, int _tack);

//I/O функции
void send();
void receving();

void GIC_interrput();
void GIC_common_configure(volatile struct GIC_registers_data* _Registers_data, volatile struct GIC* _GIC);
void GICD_common_configure_EL3(volatile struct GIC_registers_data* _Registers_data, volatile struct GIC* _GIC);
void GICD_common_configure_EL1(volatile struct GIC* _GIC);

int length(char _buffer[]);
void Tx_clear();

void UART_common_configure(volatile struct UART* _UART);
void UART_md_configure();
void UART_qcom_configure();