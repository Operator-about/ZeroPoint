#pragma once
#define DEBUG 1
#define VERISON 002
#include<Custom-type.h>
#include<stdalign.h>
#include<Stringz.h>

extern UART0 UART;
extern struct GICv2 GICv2;
extern struct GICv3 GICv3;
extern struct Ring_buffer Tx_buffer;
extern struct Ring_buffer Rx_buffer;

void VBAR_set();
void clear_buffer(char _buffer[]);
int GIC_version_check();
int MMU_IPS_check();
int get_number_length(int _number);
void send();
void receving();