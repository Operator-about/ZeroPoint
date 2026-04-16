#pragma once

#include"Custom-type.h"
#include<math.h>
#include"Ella.h"

extern struct UART_buffer Tx_buffer;
extern struct UART_buffer Rx_buffer;
extern volatile struct GIC GIC;
extern volatile struct UART* UART;
extern struct GIC_registers_data GIC_Registers;

//Функции для проверки доступности UART
void check_UART_avaible_common();
struct UART check_UART_avaible_qcom();
struct UART check_UART_avaible_md();

//Заглушки
void write(char _buffer[]);
void read(char _buffer[]);

struct BRR_UART calculate_BRR(int _BRR, int _tack);

//I/O функции
void send();
void receving();

void GIC_interrput();
void GIC_common_configure_registers(struct GIC_registers_data* _registers_data);
void GICD_common_configure_EL3(struct GIC_registers_data* _registers_data);
void GICD_common_configure_EL1(struct GIC_registers_data* _registers_data);

int length(char _buffer[]);
void Tx_clear();
void Rx_clear();
void check_end_in_text_from_rx();

void UART_common_configure();
void UART_md_configure();
void UART_qcom_configure();