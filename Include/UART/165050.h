#pragma once

#include<UART.h>
#include<Custom-type.h>

extern UART165050R* UART165050;
extern const HALUARTF UART165050F;

void write_165050();
void read_165050();

void irq_switch_165050();
void wait_for_stop_165050();

void reg_init_165050();
void Tx_init_165050();
void Rx_init_165050();
void IRQ_disable_165050();
void char_write_165050(char _char);