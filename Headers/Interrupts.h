#pragma once

#include<Custom-type.h>
#include<Ella.h>

extern struct Ring_buffer Tx_buffer;
extern struct Ring_buffer Rx_buffer;
extern volatile struct GIC GIC;
extern volatile struct UART* UART;
extern struct GIC_registers_data GIC_Registers;

//Заглушки
void write(char _buffer[]);
void read(char _buffer[]);

struct BRR_UART calculate_BRR(int _BRR, int _tack);

//I/O функции
void send();
void receving();

void GIC_interrupts();
void GIC_common_configure_registers(struct GIC_registers_data* _registers_data);
void GICD_common_configure_EL3(struct GIC_registers_data* _registers_data);
void GICD_common_configure_EL1(struct GIC_registers_data* _registers_data);

void Tx_clear();
void Rx_clear();
void check_text_from_Rx();

void UART_common_configure();