#pragma once
#include<Kernel-modules.h>
#include<Custom-type.h>

extern UARTPL011R* UARTPL011;

void write_pl011();
void read_pl011();

void irq_switch_pl011();