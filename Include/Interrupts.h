#pragma once

#include<Custom-type.h>
#include<Ella.h>
#include<Kernel-modules.h>

extern struct Ring_buffer Tx_buffer;
extern struct Ring_buffer Rx_buffer;

//I/O функции
void send();
void receving();

void GIC_interrupts();