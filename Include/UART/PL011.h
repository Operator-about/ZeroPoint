#pragma once
#include<Kernel-modules.h>
#include<Custom-type.h>

extern UARTPL011R* UARTPL011;
const extern HALUARTF UARTPL011F;

void write_pl011();
void read_pl011();

void irq_switch_pl011();
void wait_for_stop();

void Reg_init();
void Tx_init();
void Rx_init();
void IRQ_disable();