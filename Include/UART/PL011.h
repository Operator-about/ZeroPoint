#pragma once
#include<Kernel-modules.h>
#include<Custom-type.h>
#include<UART.h>

extern UARTPL011R* UARTPL011;
const extern HALUARTF UARTPL011F;

void write_pl011();
void read_pl011();

void irq_switch_pl011();
void wait_for_stop_pl011();

void reg_init_pl011();
void Tx_init_pl011();
void Rx_init_pl011();
void IRQ_disable_pl011();
void char_write_pl011(char _char);